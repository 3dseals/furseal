/*
 * fs_task_mgr.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TASK_MGR_H_
#define FS_TASK_MGR_H_


/*!
    @ingroup fsTask
    引擎任务管理器.
*/
class FS_API fsTaskMgr
{
	friend class fsTask;

public:
    //! @cond
    fsDefineException(ExceptionDeleteTaskInDestructor);
	 fsDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        判断任务管理器是否创建.
        @return 任务管理器是否创建.
    */
    static bool isCreated();


    /*!
        Creates the task manager singleton.
        @param[in] aim_fps An aim fps.
    */
    static void createAfterSys(u16 aim_fps);


    /*!
        Destroys the task manager singleton.
    */
    static void destroyFirst();


    /*!
        Returns the first task of the specified order. If the first task doesn't exist, returns NULL.
        @return The first task of the specified order.
    */
    static fsTask* getFirstTaskN(fsTask::TaskOrder order);


    /*!
        Deletes all the tasks of the specified order.
        @param[in] from The first task order.
        @param[in] to The last task order.
    */
    static void deleteOrder(fsTask::TaskOrder from, fsTask::TaskOrder to);


    /*!
        Deletes the specified task. This method is only for system.
        @param[in] task A task.
    */
    static void deleteTaskForEngine(fsTask* task);


    /*!
        Updates the frame. This method is only for system.
    */
    static void updateForEngine();


    /*!
        Returns whether frame-skipping is on.
        @return Whether frame-skipping is on.
    */
    static bool isFrameSkipResetForEngine();


    /*!
        Measures the rendering time. This method is only for system.
        @param[in] render_func A rendering function.
    */
    static void measureRenderTimeForEngine(void (*render_func)());


private:
    static const u32 ORDER_NUM = 8 + 1 + 8;
    static const u32 FPS_MEASURE_FRAME_NUM = 10;

    fsTaskMgr(u16 aim_fps);
    ~fsTaskMgr();
    void operator=(const fsTaskMgr&);

    static fsTaskMgr* instance();

    fsTask m_root_task[ORDER_NUM];
    u16 m_aim_fps;
    r32 m_cur_fps;
    u64 m_execute_time;
    u64 m_render_time;
    u64 m_last_update_time;
    u32 m_frame_count;
    u64 m_fps_measure_time;
    u32 m_fps_measure_count;
    fsTask* m_cur_task;
    fsTask* m_next_task;
    const char* m_next_task_name;
    bool m_is_frame_skip_reset;
    bool m_is_in_destructor;

    static fsTaskMgr* m_instance;
};


#endif /* FS_TASK_MGR_H_ */
