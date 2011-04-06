#
# makefile
#
#  Created on: 2011-3-17
#      Author: Lional King
#
#   Copyright (c) 2011 netsurfers
#


LOCAL_PATH := $(call my-dir)

#   Please change for your platform.
_platform := android-4

include $(CLEAR_VARS)

LOCAL_MODULE := furseal

LOCAL_SRC_FILES := \
	../../../sources/base/android/fs_low_level_android.cpp \
	../../../sources/base/linux/fs_low_level_linux.cpp \
	../../../sources/base/windows/fs_low_level_windows.cpp \
	../../../sources/base/fs_low_level_draw_opengl.cpp \
	../../../sources/base/fs_low_level_io_gen.cpp \
	../../../sources/base/fs_low_level_math_gen.cpp \
	../../../sources/base/fs_low_level_mem_gen.cpp \
	../../../sources/base/fs_low_level_util_freetype.cpp \
	../../../sources/base/fs_low_level_util_libpng.cpp \
	../../../sources/draw/fs_col.cpp \
	../../../sources/draw/fs_draw.cpp \
	../../../sources/draw/fs_draw_mgr.cpp \
	../../../sources/draw/fs_font.cpp \
	../../../sources/draw/fs_lit.cpp \
	../../../sources/draw/fs_lts.cpp \
	../../../sources/draw/fs_prim.cpp \
	../../../sources/draw/fs_rend.cpp \
	../../../sources/draw/fs_scr.cpp \
	../../../sources/draw/fs_shd.cpp \
	../../../sources/draw/fs_tex.cpp \
	../../../sources/gen/fs_id.cpp \
	../../../sources/gen/fs_mat.cpp \
	../../../sources/gen/fs_math.cpp \
	../../../sources/gen/fs_quat.cpp \
	../../../sources/gen/fs_vec.cpp \
	../../../sources/input/fs_input_mgr.cpp \
	../../../sources/kernel/fs_mgr.cpp \
	../../../sources/kernel/fs_core.cpp \
	../../../sources/mem/fs_mem_helper.cpp \
	../../../sources/mem/fs_mem_util.cpp \
	../../../sources/res/fs_res.cpp \
	../../../sources/res/fs_res_mgr.cpp \
	../../../sources/res/fs_util.cpp \
	../../../sources/task/fs_task.cpp \
	../../../sources/task/fs_task_mgr.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../include \
	$(LOCAL_PATH)/../../../project/include/zlib \
	$(LOCAL_PATH)/../../../project/include/libpng \
	$(LOCAL_PATH)/../../../project/include/freetype \
	$(NDK_PLATFORMS_ROOT)/$(_platform)/arch-arm/usr/include \

LOCAL_CXXFLAGS := -DNDEBUG -DFS_ANDROID

LOCAL_LDLIBS := -L$(NDK_PLATFORMS_ROOT)/$(_platform)/arch-arm/usr/lib -L$(LOCAL_PATH)/lib -lfreetype -lpng -lz -lGLESv1_CM

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := fursealtemp

LOCAL_STATIC_LIBRARIES := furseal

include $(BUILD_SHARED_LIBRARY)
