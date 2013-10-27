LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Include OpenCV
include ../OpenCV-2.4.6-android-sdk/sdk/native/jni/OpenCV.mk
OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=on

# Include ZXing: give path to cpp port directory, and remember to build it
# first. I used SCons to build it, but cmake should work fine too
LOCAL_STATIC_LIBRARIES += ../zxing/build/libzxing.a
LOCAL_C_INCLUDES += /home/tonykova/workspace/zxing/build/core/src

# Include tesseract
#include ../tess-two/tess-two/jni/Android.mk
# Fix path
#LOCAL_PATH := $(call my-dir)

LOCAL_MODULE    := native_activity
# Include OpenCV native jni include dir
LOCAL_C_INCLUDES+= /home/tonykova/workspace/OpenCV-2.4.6-android-sdk/sdk/native/jni/include
# Project specific stuff
LOCAL_SRC_FILES := native.cpp nonfree_init.cpp precomp.cpp sift.cpp surf.cpp
LOCAL_LDLIBS    += -lm -llog -landroid
LOCAL_STATIC_LIBRARIES += android_native_app_glue
LOCAL_SHARED_LIBRARIES := sift_prebuilt opencv_java_prebuilt
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
