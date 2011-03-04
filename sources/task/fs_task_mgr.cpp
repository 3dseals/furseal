/*
 * fs_task_mgr.cpp
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"
#include "base/fs_private_macro.h"


fsTaskMgr* fsTaskMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsTaskMgr)


void fsTaskMgr::createAfterSys(u16 aim_fps)
{
    if (aim_fps == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    destroyFirst();

    m_instance = fsNew(fsTaskMgr)(aim_fps);
}


FS_DEFINE_MANAGER_DESTROY(fsTaskMgr, First)


fsTask* fsTaskMgr::getFirstTaskN(fsTask::TaskOrder order)
{
    return instance()->m_root_task[order].getFirstChildN();
}


void fsTaskMgr::deleteOrder(fsTask::TaskOrder from, fsTask::TaskOrder to)
{
    fsTaskMgr* ins = instance();

    if (from > to)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    for (s32 i = from; i <= to; i++)
    {
        fsTask* root = &ins->m_root_task[i];

        for (fsTask* task = root->getLastChildN(); task; task = root->getLastChildN())
        {
            fsDeleteTask(task);
        }
    }
}


void fsTaskMgr::deleteTaskForEngine(fsTask* task)
{
    fsTaskMgr* ins = fsTaskMgr::instance();

    if (!task)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_is_in_destructor)
    {
        ins->m_is_in_destructor = false;
        fsThrow(ExceptionDeleteTaskInDestructor);
    }

    ins->m_is_in_destructor = true;

    fsTask* end_task = task->getPrevAllN();
    fsTask* prev_task;

    for (task = task->getLastDescendant(); task != end_task; task = prev_task)
    {
        prev_task = task->getPrevAllN();

        if (task == ins->m_cur_task)
        {
            ins->m_cur_task = NULL;
        }

        if (task == ins->m_next_task)
        {
            ins->m_next_task = task->getNextAllN();
        }

        ins->m_next_task_name = reinterpret_cast<const char*>(task);
        fsDelete(task, fsTask);
    }

    ins->m_is_in_destructor = false;
}


void fsTaskMgr::updateForEngine()
{
    fsTaskMgr* ins = instance();
    u64 update_start_time = fsMgr::getUsecTime();
    u64 task_start_time = update_start_time;

    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        if (ins->m_root_task[i].isActive())
        {
            for (fsTask* task = ins->getFirstTaskN(static_cast<fsTask::TaskOrder>(i)); task; task = ins->m_next_task)
            {
                if (task->isActive())
                {
                    ins->m_cur_task = task;
                    ins->m_next_task = task->getNextAllN();

                    task->onUpdate();

                    if (ins->m_cur_task)
                    {
                        u64 end_time = fsMgr::getUsecTime();

                        task->m_execute_time = end_time - task_start_time;

                        task_start_time = end_time;
                    }
                }
                else
                {
                    ins->m_next_task = task->getLastDescendant()->getNextAllN();
                }
            }
        }
    }

    u64 update_end_time = fsMgr::getUsecTime();

    ins->m_execute_time = update_end_time - update_start_time;
    ins->m_render_time = 0;
    ins->m_last_update_time = update_start_time;
    ins->m_next_task = NULL;

    ins->m_frame_count++;
}


bool fsTaskMgr::isFrameSkipResetForEngine()
{
    fsTaskMgr* ins = instance();
    bool is_frame_skip_reset = ins->m_is_frame_skip_reset;

    ins->m_is_frame_skip_reset = false;

    return is_frame_skip_reset;
}


void fsTaskMgr::measureRenderTimeForEngine(void (*render_func)())
{
    fsTaskMgr* ins = instance();

    if (!render_func)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    u64 render_start_time = fsMgr::getUsecTime();

    render_func();

    ins->m_render_time = fsMgr::getUsecTime() - render_start_time;

    if (ins->m_fps_measure_count % FPS_MEASURE_FRAME_NUM == 0)
    {
        ins->m_cur_fps = static_cast<r32>(1000000.0 / (render_start_time - ins->m_fps_measure_time) * FPS_MEASURE_FRAME_NUM);
        ins->m_fps_measure_time = render_start_time;
    }

    ins->m_fps_measure_count++;
}


fsTaskMgr::fsTaskMgr(u16 aim_fps)
{
    for (u32 i = 0; i < ORDER_NUM; i++)
    {
        m_root_task[i].m_order = static_cast<fsTask::TaskOrder>(i);
    }

    m_aim_fps = aim_fps;
    m_cur_fps = aim_fps;
    m_execute_time = 0;
    m_render_time = 0;
    m_last_update_time = fsMgr::getUsecTime() - 1000000 / m_aim_fps;
    m_frame_count = 0;
    m_fps_measure_time = fsMgr::getUsecTime() - 1000000 / m_aim_fps * FPS_MEASURE_FRAME_NUM;
    m_fps_measure_count = 0;
    m_cur_task = NULL;
    m_next_task = NULL;
    m_next_task_name = NULL;
    m_is_frame_skip_reset = true;
    m_is_in_destructor = false;
}


fsTaskMgr::~fsTaskMgr()
{
    deleteOrder(static_cast<fsTask::TaskOrder>(0), static_cast<fsTask::TaskOrder>(ORDER_NUM - 1));
}


FS_DEFINE_OPERATOR_EQUAL(fsTaskMgr)


FS_DEFINE_MANAGER_INSTANCE(fsTaskMgr)
