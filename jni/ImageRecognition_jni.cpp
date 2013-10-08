#include "ImageRecognition_jni.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <string>
#include <vector>

#include <android/log.h>

#define LOG_TAG "ImageProcessor/LogoRecognition"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

// In this method we could actually initialize an android_app instance and add
// a video capture to its capture field. This should work quite nicely so that
// we don't need to do any magic passing frames here from java or anything...
// though the native example segfaults like hell so dno how useful that is as
// reference
JNIEXPORT jlong JNICALL Java_org_opencv_imageprocess_LogoRecognizer_nativeCreateObject
(JNIEnv * jenv, jclass, jstring jFileName)
{
    LOGD("Java_org_opencv_imageprocess_LogoRecognizer_nativeCreateObject enter");
    const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
    string stdFileName(jnamestr);
    jlong result;
    
    try
    {
        int min_hessian = 400;
        FastFeatureDetector detector( min_hessian );

        // Images should be initialized here
        Mat reference_img = imread("pics/ref.jpg", CV_LOAD_IMAGE_GRAYSCALE);
        Mat new_img = imread("pics/img.jpg", CV_LOAD_IMAGE_GRAYSCALE);

        std::vector<KeyPoint> keypoints_ref, keypoints_new;

        detector.detect(reference_img, keypoints_ref);
        detector.detect(new_img, keypoints_new);

        //-- Draw keypoints
        Mat img_keypoints_1; Mat img_keypoints_2;
        drawKeypoints( reference_img, keypoints_ref, img_keypoints_1, Scalar::all(-1),
        DrawMatchesFlags::DEFAULT );
        drawKeypoints( new_img, keypoints_new, img_keypoints_2, Scalar::all(-1),
        DrawMatchesFlags::DEFAULT );
        //-- Show detected (drawn) keypoints
        imshow("Keypoints 1", img_keypoints_1 );
        imshow("Keypoints 2", img_keypoints_2 );
        
        waitKey(0);

        return 0;
    }
    catch(cv::Exception& e)
    {
        LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
        jclass je = jenv->FindClass("org/opencv/core/CvException");
        if(!je)
            je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, e.what());
    }
    catch (...)
    {
        LOGD("nativeCreateObject caught unknown exception");
        jclass je = jenv->FindClass("java/lang/Exception");
        jenv->ThrowNew(je, "Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
        return 0;
    }

    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeCreateObject exit");
    return 0;
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeDestroyObject enter");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStart exit");
}

JNIEXPORT void JNICALL Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop
(JNIEnv * jenv, jclass, jlong thiz)
{
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop enter");
    LOGD("Java_org_opencv_samples_facedetect_DetectionBasedTracker_nativeStop exit");
}

