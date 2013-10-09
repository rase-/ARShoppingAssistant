# Android.mk for compiling nonfree module in OpenCV4Android
# Created by Guohui Wang
# Email: robertwgh_at_gmail_com
# Data: 05/22/2013

# Compile libnonfree.so
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
OPENCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off
include /cygdrive/d/CV_dev/OpenCV-2.4.5-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_C_INCLUDES:= /cygdrive/d/CV_dev/OpenCV-2.4.5-android-sdk/sdk/native/jni/include
LOCAL_MODULE    := nonfree
LOCAL_CFLAGS    := -Werror -O3 -ffast-math
LOCAL_LDLIBS    += -llog
LOCAL_SRC_FILES := nonfree_init.cpp \
		precomp.cpp \
		sift.cpp \
		surf.cpp
include $(BUILD_SHARED_LIBRARY)