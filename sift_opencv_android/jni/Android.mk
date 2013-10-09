# Android.mk for compiling sift_test using OpenCV4Android
# Created by Guohui Wang
# Email: robertwgh_at_gmail_com
# Data: 05/22/2013

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := sift_prebuilt
LOCAL_SRC_FILES := libnonfree.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := opencv_java_prebuilt
LOCAL_SRC_FILES := libopencv_java.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= /cygdrive/d/CV_dev/OpenCV-2.4.5-android-sdk/sdk/native/jni/include
LOCAL_MODULE    := test_sift
LOCAL_CFLAGS    := -Werror -O3 -ffast-math
LOCAL_LDLIBS    += -llog -ldl 
LOCAL_SHARED_LIBRARIES := sift_prebuilt opencv_java_prebuilt
LOCAL_SRC_FILES := test_sift.cpp
include $(BUILD_EXECUTABLE)
