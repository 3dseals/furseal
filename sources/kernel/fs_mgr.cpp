/*
 * fs_mgr.cpp
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#include <stdarg.h>


#include "fs_kernel_all.h"

#include "fs_mem_all.h"
#include "fs_task_all.h"
#include "fs_input_all.h"
#include "fs_res_all.h"
#include "fs_draw_all.h"
#include "fs_base_all.h"


fsMgr* fsMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsMgr)


void fsMgr::printf(const char* str, ...)
{
    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    FS_VSPRINTF(buf, 256, str);

    fsLowLevelAPI::printf(buf);
}


void fsMgr::wprintf(const wchar_t* str, ...)
{
    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    wchar_t buf[256];
    FS_VSWPRINTF(buf, 256, str);

    fsLowLevelAPI::wprintf(buf);
}


void fsMgr::sprintf(char* buf, u32 buf_size, const char* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    FS_VSPRINTF(buf, buf_size, str);
}


void fsMgr::swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...)
{
    if (!buf || buf_size == 0 || !str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    FS_VSWPRINTF(buf, buf_size, str);
}


void* fsMgr::openFile(const char* filename, FileMode file_mode)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    void* fh = fsLowLevelAPI::openFile(filename, static_cast<fsLowLevelAPI::FileMode>(file_mode));

    if (!fh)
    {
        fsThrow(ExceptionCannotOpenFile);
    }

    return fh;
}


u32 fsMgr::getFileSize(void* file_handler)
{
    if (!file_handler)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    s32 file_size = fsLowLevelAPI::getFileSize(file_handler);

    if (file_size < 0)
    {
        fsThrow(ExceptionCannotReadFile);
    }

    return file_size;
}


void fsMgr::readFile(void* buf, u32 offset, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!fsLowLevelAPI::readFile(buf, offset, size, file_handler))
    {
        fsThrow(ExceptionCannotReadFile);
    }
}


void fsMgr::writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
{
    if (!buf || size == 0 || !file_handler)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!fsLowLevelAPI::writeFile(offset, buf, size, file_handler))
    {
        fsThrow(ExceptionCannotWriteFile);
    }
}


void fsMgr::closeFile(void* file_handler)
{
    if (!file_handler)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::closeFile(file_handler);
}


u64 fsMgr::getUsecTime()
{
    return fsLowLevelAPI::getUsecTime();
}


void fsMgr::sleepUsec(u64 usec)
{
    fsLowLevelAPI::sleepUsec(usec);
}


void fsMgr::createAfterMem(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag)
{
    if (!title || width == 0 || height == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    destroyBeforeMem();

    m_instance = fsNew(fsMgr)(title, width, height, sys_flag);

    fsTaskMgr::createAfterSys(aim_fps);
    fsInputMgr::createAfterTask();
    fsResMgr::createAfterTask();
    fsDrawMgr::createAfterRes();
}


void fsMgr::destroyBeforeMem()
{
    fsTaskMgr::destroyFirst();
    fsDrawMgr::destroyBeforeRes();
    fsResMgr::destroyBeforeSys();
    fsInputMgr::destroyBeforeSys();

    if (m_instance)
    {
        fsDelete(m_instance, fsMgr);
        m_instance = NULL;
    }
}


u16 fsMgr::getFramebufferWidth()
{
    instance();

    u16 width = fsLowLevelAPI::getFramebufferWidth();

    return (width > 0) ? width : 1;
}


u16 fsMgr::getFramebufferHeight()
{
    instance();

    u16 height = fsLowLevelAPI::getFramebufferHeight();

    return (height > 0) ? height : 1;
}


void fsMgr::updateForEngine(bool render)
{
    instance();

    fsLowLevelAPI::updateFramebufferSize();


    fsTaskMgr::updateForEngine();
    if(render)
    {
    fsTaskMgr::measureRenderTimeForEngine(fsDrawMgr::renderForEngine);
    fsLowLevelAPI::swapFramebuffer();
    }
}


void fsMgr::readLittleEndianForEngine(void* dest, const void* src, u32 size)
{
    fsLowLevelAPI::readLittleEndian(dest, src, size);
}


void fsMgr::writeLittleEndianForEngine(void* dest, const void* src, u32 size)
{
    fsLowLevelAPI::writeLittleEndian(dest, src, size);
}


void fsMgr::setInitialDirectoryForEngine(s32 argc, char** argv)
{
    if (argc < 0 || (argc > 0 && !argv))
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::setInitialDirectory(argc, argv);
}


fsMgr::fsMgr(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!fsLowLevelAPI::createApplication(title, width, height, sys_flag))
    {
        fsThrow(ExceptionCreateFramebufferFailed);
    }
}


fsMgr::~fsMgr()
{
    fsLowLevelAPI::destroyApplication();
}

FS_DEFINE_OPERATOR_EQUAL(fsMgr)


FS_DEFINE_MANAGER_INSTANCE(fsMgr)
