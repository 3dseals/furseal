/*
 * fs_core.cpp
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#include <stdarg.h>


#include "fs_mem_all.h"
#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_draw_all.h"

#include "base/fs_low_level_api.h"


static u64 s_one_frame_time;
static u64 s_next_update_time = 0;
static u64 s_last_render_time = 0;


void fsCreateFurseal(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag)
{
    fsMemHelper::createFirst();
    fsMgr::createAfterMem(title, width, height, aim_fps, sys_flag);
}


void fsDestroyFurseal()
{
	 fsMgr::destroyBeforeMem();
    fsMemHelper::destroyLast();
}


static bool loop()
{
    bool render = false;
    u64 cur_time = fsMgr::getUsecTime();

    if ((fsTaskMgr::isFrameSkipResetForEngine() && cur_time > s_next_update_time) || //
        cur_time - s_last_render_time >= fsDrawMgr::MAX_RENDER_INTERVAL_MSEC_TIME * 1000)
    {
        s_next_update_time = cur_time;
    }

    if (cur_time <= s_next_update_time + s_one_frame_time)
    {
        while (cur_time < s_next_update_time)
        {
            fsLowLevelAPI::sleepUsec((s_next_update_time - cur_time) / 4);

            cur_time = fsMgr::getUsecTime();
        }

        render = true;

        fsMgr::updateForEngine(render);

        s_last_render_time = cur_time;
        s_next_update_time += s_one_frame_time;

        return render;
    }
    else
    {
        fsMgr::updateForEngine(render);

        s_next_update_time += s_one_frame_time;

        return render;
    }
}


void fsStartFurseal()
{
    fsTry
    {
        //开始引擎的循环
        fsLowLevelAPI::startApplication(loop);
    }
    fsCatch(fsMgr::ExceptionEndFurseal) {}
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
    fsThrow(fsMgr::ExceptionEndFurseal);
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

