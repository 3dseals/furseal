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

LOCAL_MODULE := furseal
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	tremolo/bitwise.c      \
	tremolo/bitwiseARM.s   \
	tremolo/codebook.c     \
	tremolo/dpen.s         \
	tremolo/dsp.c          \
	tremolo/floor0.c       \
	tremolo/floor1.c       \
	tremolo/floor1ARM.s    \
	tremolo/floor1LARM.s   \
	tremolo/floor_lookup.c \
	tremolo/framing.c      \
	tremolo/info.c         \
	tremolo/mapping0.c     \
	tremolo/mdct.c         \
	tremolo/mdctARM.s      \
	tremolo/mdctLARM.s     \
	tremolo/misc.c         \
	tremolo/res012.c       \
	tremolo/speed.s        \
	tremolo/vorbisfile.c   \
	tremolo/speed.s        \
	OpenAL32/alAuxEffectSlot.c \
	OpenAL32/alBuffer.c        \
	OpenAL32/alDatabuffer.c    \
	OpenAL32/alEffect.c        \
	OpenAL32/alError.c         \
	OpenAL32/alExtension.c     \
	OpenAL32/alFilter.c        \
	OpenAL32/alListener.c      \
	OpenAL32/alSource.c        \
	OpenAL32/alState.c         \
	OpenAL32/alThunk.c         \
	Alc/ALc.c                  \
	Alc/alcConfig.c            \
	Alc/alcEcho.c              \
	Alc/alcModulator.c         \
	Alc/alcReverb.c            \
	Alc/alcRing.c              \
	Alc/alcThread.c            \
	Alc/ALu.c                  \
	Alc/android.c              \
	Alc/bs2b.c                 \
	Alc/null.c                 \
	../../../sources/base/android/fs_low_level_sound_android.cpp \
	../../../sources/base/android/fs_low_level_android.cpp \
	../../../sources/base/fs_low_level_draw_opengl.cpp \
	../../../sources/base/fs_low_level_io_gen.cpp \
	../../../sources/base/fs_low_level_math_gen.cpp \
	../../../sources/base/fs_low_level_mem_gen.cpp \
	../../../sources/base/fs_low_level_util_freetype.cpp \
	../../../sources/base/fs_low_level_util_libpng.cpp \
	../../../sources/cdt/fs_cdt_aabb.cpp \
	../../../sources/cdt/fs_cdt_box.cpp \
	../../../sources/cdt/fs_cdt_plane.cpp \
	../../../sources/cdt/fs_cdt_ray.cpp \
	../../../sources/cdt/fs_cdt_sph.cpp \
	../../../sources/cdt/fs_cdt_tri.cpp \
	../../../sources/debug/fs_debug_mgr.cpp \
	../../../sources/debug/fs_debug_mgr_draw.cpp \
	../../../sources/debug/fs_debug_mgr_dump.cpp \
	../../../sources/debug/fs_debug_mgr_font.cpp \
	../../../sources/draw/fs_col.cpp \
	../../../sources/draw/fs_draw.cpp \
	../../../sources/draw/fs_draw_mgr.cpp \
	../../../sources/draw/fs_font.cpp \
	../../../sources/draw/fs_lit.cpp \
	../../../sources/draw/fs_lts.cpp \
	../../../sources/draw/fs_mdl.cpp \
	../../../sources/draw/fs_mdl_data.cpp \
	../../../sources/draw/fs_mot.cpp \
	../../../sources/draw/fs_mot_data.cpp \
	../../../sources/draw/fs_node.cpp \
	../../../sources/draw/fs_prim.cpp \
	../../../sources/draw/fs_sprt.cpp \
	../../../sources/draw/fs_rend.cpp \
	../../../sources/draw/fs_scr.cpp \
	../../../sources/draw/fs_shd.cpp \
	../../../sources/draw/fs_tex.cpp \
	../../../sources/draw/rend/fs_rend_light.cpp \
	../../../sources/draw/rend/fs_rend_shader.cpp \
	../../../sources/gen/fs_id.cpp \
	../../../sources/gen/fs_mat.cpp \
	../../../sources/gen/fs_math.cpp \
	../../../sources/gen/fs_quat.cpp \
	../../../sources/gen/fs_vec.cpp \
	../../../sources/gen/fs_fix.cpp \
	../../../sources/input/fs_input_mgr.cpp \
	../../../sources/kernel/fs_mgr.cpp \
	../../../sources/kernel/fs_core.cpp \
	../../../sources/mem/fs_mem_helper.cpp \
	../../../sources/mem/fs_mem_util.cpp \
	../../../sources/res/fs_res.cpp \
	../../../sources/res/fs_res_mgr.cpp \
	../../../sources/res/fs_util.cpp \
	../../../sources/res/fs_util_import_3ds_model.cpp \
	../../../sources/res/fs_util_load_pixel_art_model.cpp \
	../../../sources/script/fs_script.cpp \
	../../../sources/script/fs_script_entry.cpp \
	../../../sources/script/fs_script_mgr.cpp \
	../../../sources/script/fs_script_parse.cpp \
	../../../sources/sound/fs_sound_mgr.cpp \
	../../../sources/sound/fs_sound_mgr_mix.cpp \
	../../../sources/task/fs_task.cpp \
	../../../sources/task/fs_task_mgr.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/OpenAL32/Include \
	$(LOCAL_PATH)/../../../include \
	$(LOCAL_PATH)/../../../project/include/zlib \
	$(LOCAL_PATH)/../../../project/include/libpng \
	$(LOCAL_PATH)/../../../project/include/freetype \

LOCAL_CFLAGS   := -D_ARM_ASSEM_ -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_CXXFLAGS := -DNDEBUG -DFS_ANDROID
LOCAL_LDLIBS :=  -L$(LOCAL_PATH)/../library -lfreetype -lpng -lz -lGLESv1_CM -llog -Wl,-s

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := hello_furseal
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES := \
	main.cpp \
	sound_mixer.cpp \
	sound_monitor.cpp \

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/tremolo \
	$(LOCAL_PATH)/OpenAL32/Include \
	$(LOCAL_PATH)/../../../include \
	$(LOCAL_PATH)/../../../project/include/zlib \
	$(LOCAL_PATH)/../../../project/include/libpng \
	$(LOCAL_PATH)/../../../project/include/freetype \
LOCAL_CFLAGS   := -D_ARM_ASSEM_ -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_CXXFLAGS := -DNDEBUG -DFS_ANDROID
LOCAL_STATIC_LIBRARIES := furseal
LOCAL_LDLIBS :=  -L$(LOCAL_PATH)/../library -lfreetype -lpng -lz -lGLESv1_CM -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)
