#
# makefile
#
#  Created on: 2011-3-17
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := hello_furseal
LOCAL_ARM_MODE  := arm

LOCAL_SRC_FILES := \
	main.cpp \
	hello_furseal.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include \

LOCAL_CXXFLAGS := -DNDEBUG -DFS_ANDROID
LOCAL_LDLIBS :=  -L$(LOCAL_PATH)/../library -lfurseal -lfreetype -lpng -lz -lGLESv1_CM -lgcc -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)
