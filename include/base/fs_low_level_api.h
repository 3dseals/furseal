/*
 * fs_low_level_api.h
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifndef FS_LOW_LEVEL_API_H_
#define FS_LOW_LEVEL_API_H_


#include "fs_def.h"


/*!
    @ingroup fsBase
    夸平台低级接口.
*/
class fsLowLevelAPI
{
public:

    static r32 sqrt(r32 x);
    static r32 sin(r32 rad);
    static r32 cos(r32 rad);
    static r32 asin(r32 x);
    static r32 acos(r32 x);
    static r32 atan2(r32 y, r32 x);
    static void srand(s32 seed);
    static s32 rand();

    static void* malloc(u32 size);
    static void free(void* ptr);
    static void memset(void* buf, u8 value, u32 size);
    static void memcpy(void* dest, const void* src, u32 size);

    static bool createApplication(const char* title, u16 width, u16 height, u16 sys_flag);
    static void destroyApplication();
    static void startApplication(bool (*update_func)(void));
    static u16 getFramebufferWidth();
    static u16 getFramebufferHeight();
    static void updateFramebufferSize();
    static bool isFramebufferSizeChanged();
    static void swapFramebuffer();
    static bool isFullScreen();
    static bool toggleFullScreen(u16 width, u16 height);

    typedef void (*KeyEventHandler)(u8 keycode, bool is_down);
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);
    typedef void (*ExtraEventHandler)(u8 index, r32 value);
    static void setKeyEventHandler(KeyEventHandler handler);
    static void setMouseEventHandler(MouseEventHandler handler);
    static void setExtraEventHandler(ExtraEventHandler handler);
    static void setMousePos(s16 mouse_x, s16 mouse_y);
    static bool isMouseVisible();
    static void setMouseVisible(bool is_visible);

    static u64 getUsecTime();
    static void sleepUsec(u64 usec);
    static void exit(s32 status);
    static void error(const char* msg);
    static void readLittleEndian(void* dest, const void* src, u32 size);
    static void writeLittleEndian(void* dest, const void* src, u32 size);

    static void* newThread(void (*start_func)(void*), void* user_param);
    static void deleteThread(void* thread_handler);
    static void joinThread(void* thread_handler);
    static void* newMutex();
    static void deleteMutex(void* mutex_handler);
    static void lockMutex(void* mutex_handler);
    static void unlockMutex(void* mutex_handler);

    static void setInitialDirectory(s32 argc, char** argv);
    static void getWindowsFontDirectory(char* buf, u32 buf_size);

    enum FileMode
    {
        FILE_MODE_READ, //
        FILE_MODE_WRITE
    };

    static void printf(const char* msg);
    static void vsprintf(char* buf, u32 buf_size, const char* format, void* arg);
    static void* openFile(const char* filename, FileMode file_mode);
    static s32 getFileSize(void* file_handler);
    static bool readFile(void* buf, u32 offset, u32 size, void* file_handler);
    static bool writeFile(u32 offset, const void* buf, u32 size, void* file_handler);
    static void closeFile(void* file_handler);

    enum DepthTest
    {
        DEPTH_TEST_ALWAYS, //
        DEPTH_TEST_LESS, //
        DEPTH_TEST_GREATER, //
        DEPTH_TEST_LEQUAL, //
        DEPTH_TEST_GEQUAL
    };

    enum BlendMode
    {
        BLEND_OFF, //
        BLEND_HALF, //
        BLEND_ADD, //
        BLEND_DEST_ALPHA
    };

    enum DrawMode
    {
        DRAW_POINTS, //
        DRAW_LINES, //
        DRAW_LINE_LOOP, //
        DRAW_LINE_STRIP, //
        DRAW_TRIANGLES, //
        DRAW_TRIANGLE_STRIP, //
        DRAW_TRIANGLE_FAN
    };

    enum TextureFormat
    {
        FORMAT_RGB, //
        FORMAT_RGBA, //
        FORMAT_ALPHA
    };

    static void setupShaderAPI(bool is_shader_check);
    static void resetDrawState();
    static bool isShaderAvailable();
    static void setViewport(s16 left, s16 top, u16 width, u16 height);
    static void clearFramebuffer(bool is_clear_color, bool is_clear_depth, const u8* clear_color);

    static void setDepthTest(DepthTest depth_test);
    static void setBlendMode(BlendMode blend_mode);
    static void setWriteMode(bool is_draw_rgb, bool is_draw_alpha, bool is_draw_depth);
    static void setBackfaceCulling(bool is_backface_culling);

    static u16 getMaxTextureLength();
    static u16 getValidTextureLength(u16 length);
    static u16 getTexturePixelSize(TextureFormat format);
    static u32 registerTexture(u16 width, u16 height, TextureFormat format, const void* image);
    static void replaceSubTexture(u32 tex_obj, u16 x, u16 y, u16 width, u16 height, TextureFormat format, const void* sub_image);
    static void unregisterTexture(u32 tex_obj);
    static void setTexture(u32 tex_obj1, u32 tex_obj2, u32 tex_obj3, bool is_bilinear);
    static void copyImageFromFramebufferToTexture( //
        u32 tex_obj, u16 tex_width, u16 tex_height, TextureFormat tex_format, u16 left_in_framebuffer, u16 top_in_framebuffer);

    static void setProjection(const r32* projection);
    static void setModelView(const r32* model_view);

    static void setVertexPointer(u32 stride, const r32* vert);
    static void setColorPointer(u32 stride, const u8* color);
    static void setTexCoordPointer(u32 stride, const r32* uv);

    static void drawArrays(DrawMode draw_mode, u16 first, u16 count);

    static u32 registerShader(const char* vert_code, const char* frag_code);
    static void unregisterShader(u32 shd_obj);
    static void setShader(u32 shd_obj);
    static u32 getUniformLocation(u32 shd_obj, const char* name);
    static void setUniform_s32(u32 location, s32 uniform);
    static void setUniform_r32(u32 location, r32 uniform);
    static void setUniform_localToScreen(u32 location);
    static u32 getAttribLocation(u32 shd_obj, const char* name);
    static void setAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib);
    static void setAttribPointer_color(u32 location, u32 stride, const u8* attrib);
    static void disableAttribPointer(u32 location);
};

#endif // !FS_LOW_LEVEL_API_H_
