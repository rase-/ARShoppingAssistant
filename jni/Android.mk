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

# Include tesseract, try1
#include ../tess-two/tess-two/jni/Android.mk
# Fix path
#LOCAL_PATH := $(call my-dir)
# Trying to include tesseract as a static lib, try2
#TESSERACT_PATH := ../tess-two/tess-two/com_googlecode_tesseract_android/src
#LEPTONICA_PATH := ../tess-two/tess-two/com_googlecode_leptonica_android/src
#LOCAL_STATIC_LIBRARIES += ../tess-two/tess-two/libs/armeabi/liblept.so
#LOCAL_STATIC_LIBRARIES += ../tess-two/tess-two/libs/armeabi/libtess.so
#LOCAL_C_INCLUDES += \
#  $(TESSERACT_PATH)/api \
#  $(TESSERACT_PATH)/ccmain \
#  $(TESSERACT_PATH)/ccstruct \
#  $(TESSERACT_PATH)/ccutil \
#  $(TESSERACT_PATH)/classify \
#  $(TESSERACT_PATH)/cube \
#  $(TESSERACT_PATH)/cutil \
#  $(TESSERACT_PATH)/dict \
#  $(TESSERACT_PATH)/image \
#  $(TESSERACT_PATH)/neural_networks/runtime \
#  $(TESSERACT_PATH)/textord \
#  $(TESSERACT_PATH)/viewer \
#  $(TESSERACT_PATH)/wordrec \
#  $(LEPTONICA_PATH)/src
#  Trying to include by doing what tess-two Android.mk is doing, try3
TESSERACT_PATH := ../tess-two/tess-two/jni/com_googlecode_tesseract_android/src
LEPTONICA_PATH := ../tess-two/tess-two/jni/com_googlecode_leptonica_android/src
include ../tess-two/tess-two/jni/com_googlecode_leptonica_android/Android.mk
include ../tess-two/tess-two/jni/com_googlecode_tesseract_android/Android.mk

# Just build the Android.mk files in the subdirs
# include $(call all-subdir-makefiles)
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
