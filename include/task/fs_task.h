/*
 * fs_task.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
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
        Returns whether this task has the order.
        @return Whether this task has the order.
    */
    bool hasOrder() const;


    /*!
        Retuens the order of this task.
        @return The order of this task.
    */
    TaskOrder getOrder() const;


    /*!
        Returns whether this task has the order.
        @return Whether this task has the order.
    */
    bool hasParent() const;


    /*!
        Returns the parent of this task. If the parent doesn't exist, returns NULL.
        @return The parent of this task.
    */
    fsTask* getParentN() const;


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
        Returns the previous sibling of this task.
        If this task has no previous sibling, returns NULL.
        @return The previous sibling of this task.
    */
    fsTask* getPrevSiblingN() const;


    /*!
        Returns the next sibling of this task.
        If this task has no previous sibling, returns NULL.
        @return The previous sibling of this task.
    */
    fsTask* getNextSiblingN() const;


    /*!
        返回上次的任务,如果没有则返回这次任务.
        @return 上次的任务,如果没有则返回这次任务.
    */
    fsTask* getLastDescendant() const;


    /*!
        Returns whether this task has a child.
        @return Whether this task has a child.
    */
    bool hasChild() const;


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
        Returns the name of this task.
        @return The name of this task.
    */
    const char* getName() const;


    /*!
        Returns the process time of this task in usec.
        @return The process time of this task.
    */
    u64 getExecuteUsecTime() const;


    /*!
        判断任务是否被激活.
        @return 是否被激活.
    */
    bool isActive() const;


    /*!
        Determines whether this task is active.
        @param[in] is_active whetehr this task is active.
    */
    void setActive(bool is_active);


    /*!
        更新任务.
    */
    virtual void onUpdate();


    /*!
        Receives a message.
        @param[in] msg_id The ID of a message.
        @param[in] msg A message.
    */
    virtual void onMessage(fsID msg_id, fsMsg<4>& msg);


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
