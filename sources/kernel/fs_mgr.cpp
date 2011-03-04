/*
 * fs_mgr.cpp
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/

#include "fs.h"
#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


fsMgr* fsMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsMgr)


u64 fsMgr::getUsecTime()
{
    return fsLowLevelAPI::getUsecTime();
}


void fsMgr::sleepUsec(u64 usec)
{
    fsLowLevelAPI::sleepUsec(usec);
}


void fsMgr::createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!title || width == 0 || height == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    destroyBeforeMem();

    m_instance = fsNew(fsMgr)(title, width, height, sys_flag);
}


FS_DEFINE_MANAGER_DESTROY(fsMgr, BeforeMem)


void fsMgr::updateForEngine()
{
    instance();

    fsLowLevelAPI::updateFramebufferSize();
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
