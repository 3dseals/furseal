/*
 * fs_task.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TASK_H_
#define FS_TASK_H_


/*!
    @ingroup fsTask
    非抢占式任务.
*/
class FS_API fsTask
{
    friend class fsTaskMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond


    /*!
        任务命令类型.
    */
    enum TaskOrder
    {
        ORDER_MINUS_8_FOR_SYSTEM, ORDER_MINUS_7, ORDER_MINUS_6, ORDER_MINUS_5, //
        ORDER_MINUS_4, ORDER_MINUS_3, ORDER_MINUS_2, ORDER_MINUS_1, //
        ORDER_ZERO, //
        ORDER_PLUS_1, ORDER_PLUS_2, ORDER_PLUS_3, ORDER_PLUS_4, //
        ORDER_PLUS_5, ORDER_PLUS_6, ORDER_PLUS_7, ORDER_PLUS_8_FOR_SYSTEM
    };


    /*!
        返回前一个任务,如果没有前一个任务则返回NULL.
        @return 前一个任务,如果没有前一个任务则返回NULL.
    */
    fsTask* getPrevAllN() const;


    /*!
        返回后一个任务,如果没有后一个任务则返回NULL.
        @return 后一个任务,如果没有后一个任务则返回NULL.
    */
    fsTask* getNextAllN() const;


    /*!
        返回上次的任务,如果没有则返回这次任务.
        @return 上次的任务,如果没有则返回这次任务.
    */
    fsTask* getLastDescendant() const;


    /*!
        返回第一个任务,如果没有则返回NULL.
        @return 第一个任务,如果没有则返回NULL.
    */
    fsTask* getFirstChildN() const;


    /*!
        返回最后一个任务,如果没有则返回NULL.
        @return 最后一个任务,如果没有则返回NULL.
    */
    fsTask* getLastChildN() const;


    /*!
        判断任务是否被激活.
        @return 是否被激活.
    */
    bool isActive() const;


    /*!
        更新任务.
    */
    virtual void onUpdate();


protected:
    //! @cond
    fsTask(TaskOrder order);
    fsTask(fsTask* parent);
    virtual ~fsTask();
    //! @endcond


private:
    enum TaskFlag
    {
        FLAG_ACTIVE
    };

    fsTask();
    fsTask(const fsTask&);
    void operator=(const fsTask&);

    fsTree<fsTask> m_tree;
    fsType<u8, TaskOrder> m_order;
    fsFlag<u8, TaskFlag> m_flag;
    const char* m_name;
    u64 m_execute_time;
};

#endif /* FS_TASK_H_ */
