LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include ../OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk
OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=on

LOCAL_MODULE    := native_activity
LOCAL_C_INCLUDES:= /home/tonykova/workspace/OpenCV-2.4.6-android-sdk/sdk/native/jni/include
LOCAL_C_INCLUDES+= /usr/local/boost_1_54_0
LOCAL_SRC_FILES := native.cpp nonfree_init.cpp precomp.cpp sift.cpp surf.cpp
LOCAL_LDLIBS    += -lm -llog -landroid
LOCAL_STATIC_LIBRARIES := android_native_app_glue boost_thread-gcc-mt-s-1_46
LOCAL_SHARED_LIBRARIES := sift_prebuilt opencv_java_prebuilt
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
