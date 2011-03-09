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


#include "fs.h"


/*!
    @ingroup fsBase
    夸平台低级接口.
*/
class fsLowLevelAPI
{
public:
    static void* malloc(u32 size);
    static void free(void* ptr);
    static void memset(void* buf, u8 value, u32 size);
    static void memcpy(void* dest, const void* src, u32 size);

    static bool createApplication(const char* title, u16 width, u16 height, u16 sys_flag);
    static void destroyApplication();
    static void startApplication(bool (*update_func)(void));
    static void updateFramebufferSize();
    static void swapFramebuffer();

    typedef void (*KeyEventHandler)(u8 keycode, bool is_down);
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);
    typedef void (*ExtraEventHandler)(u8 index, r32 value);
    static void setKeyEventHandler(KeyEventHandler handler);
    static void setMouseEventHandler(MouseEventHandler handler);
    static void setExtraEventHandler(ExtraEventHandler handler);
    static void setMouseVisible(bool is_visible);

    static u64 getUsecTime();
    static void sleepUsec(u64 usec);
    static void exit(s32 status);
    static void error(const char* msg);
    static void setInitialDirectory(s32 argc, char** argv);

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
};

#endif // !FS_LOW_LEVEL_API_H_
