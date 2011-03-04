/*
 * fs_mgr.cpp
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


fsMgr* fsMgr::m_instance = NULL;


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
