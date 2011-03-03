/*
    Copyright (c) 2011 netsurfers
*/


#include <stdarg.h>


#include "fs.h"
#include "base/fs_low_level_api.h"


void fsCreateFurseal(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag)
{

}


void fsDestroyFurseal()
{

}


static bool loop()
{
    return true;
}


void fsStartFurseal()
{
    fsTry
    {
        //开始引擎的循环
        fsLowLevelAPI::startApplication(loop);
    }
    fsCatch(fsMgr::ExceptionEndCatcake) {}
    fsCatch(fsException e)
    {
#ifndef FS_NO_THROW_EXCEPTION
        fsError("Unhandled Exception: %s in %s(%d)\n", e.getException(), e.getFile(), e.getLine());
#endif
    }
}


void fsStopFurseal()
{
#ifdef FS_NO_THROW_EXCEPTION
    fsLowLevelAPI::exit(0);
#else
    fsThrow(fsMgr::ExceptionEndCatcake);
#endif
}

void fsError(const char* msg, ...)
{
    char buf[256];
    va_list ap;

    va_start(ap, msg);
    fsLowLevelAPI::vsprintf(buf, 256, msg, &ap);
    va_end(ap);

    fsLowLevelAPI::error(buf);
}


#ifdef FS_NO_THROW_EXCEPTION
void fsSubstituteThrow(const char* exception, const char* file, u32 line)
{
    fsError("Exception: %s in %s(%d)\n", exception, file, line);
}
#endif

