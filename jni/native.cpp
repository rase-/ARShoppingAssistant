#include <android_native_app_glue.h>

#include <errno.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <pthread.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <zxing/common/Counted.h>

#define  LOG_TAG    "OCV:libnative_activity"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define DETECTION_FRAME 15

using namespace cv;

int frame_num = 0;
Mat current_frame;
pthread_t compute_thread;

static void detect_and_draw_features(Mat& image)
{
    vector<KeyPoint> keypoints;
    Mat surf_descriptors;
    Mat freak_descriptors;
    
    int min_hessian = 400;
    SurfFeatureDetector surf_detector(min_hessian);
    FREAK freak_descriptor;
    surf_detector.detect(image, keypoints);
    surf_detector.compute(image, keypoints, surf_descriptors);
    freak_descriptor.compute(image, keypoints, freak_descriptors);
    
    LOGI("Number of SURF keypoints %i\n", keypoints.size());
    for (int i = 0; i < keypoints.size(); i++) {
        LOGI("keypoints[%i]: %f %f %f\n", i, keypoints[i].pt.x, keypoints[i].pt.y, keypoints[i].size);
    }
    
    //Scalar keypointColor = Scalar(231, 84, 128);
    //drawKeypoints(image, keypoints, target_img, keypointColor, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
}

void* compute_features(void*) {
    detect_and_draw_features(current_frame);
    pthread_exit(NULL);
}

struct Engine
{
    android_app* app;
    Ptr<VideoCapture> capture;
};

static Size calc_optimal_camera_resolution(const char* supported, int width, int height)
{
    int frame_width = 0;
    int frame_height = 0;

    size_t prev_idx = 0;
    size_t idx = 0;
    float min_diff = FLT_MAX;

    do
    {
        int tmp_width;
        int tmp_height;

        prev_idx = idx;
        while ((supported[idx] != '\0') && (supported[idx] != ','))
            idx++;

        sscanf(&supported[prev_idx], "%dx%d", &tmp_width, &tmp_height);

        int w_diff = width - tmp_width;
        int h_diff = height - tmp_height;
        if ((h_diff >= 0) && (w_diff >= 0))
        {
            if ((h_diff <= min_diff) && (tmp_height <= 720))
            {
                frame_width = tmp_width;
                frame_height = tmp_height;
                min_diff = h_diff;
            }
        }

        idx++; // to skip comma symbol

    } while(supported[idx-1] != '\0');

    return Size(frame_width, frame_height);
}

static void engine_draw_frame(Engine* engine, const Mat& frame)
{
    if (engine->app->window == NULL)
        return; // No window.

    ANativeWindow_Buffer buffer;
    if (ANativeWindow_lock(engine->app->window, &buffer, NULL) < 0)
    {
        LOGW("Unable to lock window buffer");
        return;
    }

    void* pixels = buffer.bits;

    int left_indent = (buffer.width-frame.cols)/2;
    int top_indent = (buffer.height-frame.rows)/2;

    for (int yy = top_indent; yy < std::min(frame.rows+top_indent, buffer.height); yy++)
    {
        unsigned char* line = (unsigned char*)pixels + left_indent*4*sizeof(unsigned char);
        size_t line_size = std::min(frame.cols, buffer.width)*4*sizeof(unsigned char);
        memcpy(line, frame.ptr<unsigned char>(yy), line_size);
        // go to next line
        pixels = (int32_t*)pixels + buffer.stride;
    }
    ANativeWindow_unlockAndPost(engine->app->window);
}

static void engine_handle_cmd(android_app* app, int32_t cmd)
{
    Engine* engine = (Engine*)app->userData;
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL)
            {
                LOGI("APP_CMD_INIT_WINDOW");

                engine->capture = new VideoCapture(0);

                union {double prop; const char* name;} u;
                u.prop = engine->capture->get(CV_CAP_PROP_SUPPORTED_PREVIEW_SIZES_STRING);

                int view_width = ANativeWindow_getWidth(app->window);
                int view_height = ANativeWindow_getHeight(app->window);

                Size camera_resolution;
                if (u.name)
                    camera_resolution = calc_optimal_camera_resolution(u.name, 640, 480);
                else
                {
                    LOGE("Cannot get supported camera camera_resolutions");
                    camera_resolution = Size(ANativeWindow_getWidth(app->window),
                                          ANativeWindow_getHeight(app->window));
                }

                if ((camera_resolution.width != 0) && (camera_resolution.height != 0))
                {
                    engine->capture->set(CV_CAP_PROP_FRAME_WIDTH, camera_resolution.width);
                    engine->capture->set(CV_CAP_PROP_FRAME_HEIGHT, camera_resolution.height);
                }

                float scale = std::min((float)view_width/camera_resolution.width,
                                       (float)view_height/camera_resolution.height);

                if (ANativeWindow_setBuffersGeometry(app->window, (int)(view_width/scale),
                    int(view_height/scale), WINDOW_FORMAT_RGBA_8888) < 0)
                {
                    LOGE("Cannot set pixel format!");
                    return;
                }

                LOGI("Camera initialized at resolution %dx%d", camera_resolution.width, camera_resolution.height);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");

            engine->capture->release();
            break;
    }
}

void android_main(android_app* app)
{
    Engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    size_t engine_size = sizeof(engine); // for Eclipse CDT parser
    memset((void*)&engine, 0, engine_size);
    app->userData = &engine;
    app->onAppCmd = engine_handle_cmd;
    engine.app = app;

    float fps = 0;
    Mat drawing_frame;
    std::queue<int64> time_queue;

    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        android_poll_source* source;

        // Process system events
        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(app, source);
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0)
            {
                LOGI("Engine thread destroy requested!");
                return;
            }
        }

        int64 then;
        int64 now = getTickCount();
        time_queue.push(now);

        // Capture frame from camera and draw it
        if (!engine.capture.empty())
        {
            if (engine.capture->grab())
                engine.capture->retrieve(drawing_frame, CV_CAP_ANDROID_COLOR_FRAME_RGBA);

             char buffer[256];
             sprintf(buffer, "Display performance: %dx%d @ %.3f", drawing_frame.cols, drawing_frame.rows, fps);
             putText(drawing_frame, std::string(buffer), Point(8,64),
                         FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,255,0,255));
             Mat target_img;
             if (frame_num == 0) {
                drawing_frame.convertTo(target_img, CV_8UC3);
                //auto cb_func = [&drawing_frame,&target_img] () { detect_and_draw_features(drawing_frame, target_img); };
                //boost::thread t(cb_func, "Computation");
                //pthread_join(compute_thread, NULL);
                current_frame = drawing_frame;
                pthread_create(&compute_thread, NULL, &compute_features, &drawing_frame);
             }
             engine_draw_frame(&engine, drawing_frame);
             frame_num = (frame_num + 1) % DETECTION_FRAME;
        }

        if (time_queue.size() >= 2)
            then = time_queue.front();
        else
            then = 0;

        if (time_queue.size() >= 25)
            time_queue.pop();

        fps = time_queue.size() * (float)getTickFrequency() / (now-then);
    }
}
