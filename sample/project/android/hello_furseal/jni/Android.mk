#
# Android.mk
#
#  Created on: 2011-3-2
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#


LOCAL_PATH := $(call my-dir)

_platform := android-4

include $(CLEAR_VARS)

LOCAL_MODULE := hello_furseal

LOCAL_SRC_FILES := main.cpp hello_furseal.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../include

LOCAL_CXXFLAGS := -DNDEBUG -DCK_ANDROID

LOCAL_LDLIBS := -L$(NDK_PLATFORMS_ROOT)/$(_platform)/arch-arm/usr/lib -L$(LOCAL_PATH)/../../../../../library -lfurseal -lGLESv1_CM -lgcc

include $(BUILD_SHARED_LIBRARY)
