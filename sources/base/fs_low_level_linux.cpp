/*
    Copyright (c) 2011 netsurfers
*/


#ifdef FS_LINUX


#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>


#include "base/fs_low_level_api.h"


static void destroyFramebuffer()
{

}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    return true;
}


bool fsLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    if (!createFramebuffer(width, height))
    {
        return false;
    }

    return true;
}


void fsLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


void fsLowLevelAPI::startApplication(bool (*update_func)(void))
{
    while (true)
    {
        (*update_func)();
    }
}


void fsLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    if (argc > 0)
    {
        if (chdir(dirname(argv[0])) != 0)
        {
            return; // to avoid gcc warning
        }
    }
}


void fsLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void fsLowLevelAPI::error(const char* msg)
{
    printf(msg);
    printf("\n");

    exit(1);
}


#endif // FS_LINUX
