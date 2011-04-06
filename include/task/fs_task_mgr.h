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
	fsDefineException(ExceptionNewTaskInDestructor);
	fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        判断任务管理器是否创建.
        @return 任务管理器是否创建.
    */
    static bool isCreated();


    /*!
        创建任务管理器,该方法只被引擎自己调用.
        @param[in] aim_fps An aim fps.
    */
    static void createAfterSys(u16 aim_fps);


    /*!
        销毁任务管理器,该方法只被引擎自己调用.
    */
    static void destroyFirst();


    /*!
        返回第一个任务的命令,如果不存在则返回NULL.
        @return 第一个任务的命令.
    */
    static fsTask* getFirstTaskN(fsTask::TaskOrder order);


    /*!
        删除某一段任务命令.
        @param[in] from 第一个任务命令.
        @param[in] to 最后一个任务命令.
    */
    static void deleteOrder(fsTask::TaskOrder from, fsTask::TaskOrder to);


    /*!
        删除任务,该方法只被引擎自己调用.
        @param[in] task A task.
    */
    static void deleteTaskForEngine(fsTask* task);


    /*!
        Suspends the frame-skipping of the current frame.
    */
    static void resetFrameSkip();


    /*!
        更新任务管理器,该方法只被引擎自己调用.
    */
    static void updateForEngine();


    /*!
        设置下一个任务的名字.
        @param[in] name 下一个任务的名字.
    */
    static u32 setNextTaskNameForEngine(const char* name);


    /*!
        判断是否忽略当前帧.
        @return 是否忽略当前帧.
    */
    static bool isFrameSkipResetForEngine();


    /*!
        对绘图进行布局,该方法只被引擎自己调用.
        @param[in] 渲染的函数指针.
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
