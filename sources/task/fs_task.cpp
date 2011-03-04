/*
 * fs_task.cpp
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"
#include "base/fs_private_macro.h"


fsTask* fsTask::getPrevAllN() const
{
    fsTree<fsTask>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


fsTask* fsTask::getNextAllN() const
{
    fsTree<fsTask>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


fsTask* fsTask::getLastDescendant() const
{
    return m_tree.getLastDescendant()->getSelf();
}


fsTask* fsTask::getFirstChildN() const
{
    fsTree<fsTask>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


fsTask* fsTask::getLastChildN() const
{
    fsTree<fsTask>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


bool fsTask::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void fsTask::onUpdate() {}


fsTask::fsTask(TaskOrder order)
{
    fsTaskMgr* ins = fsTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        fsThrow(ExceptionInvalidCall);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    ins->m_root_task[order].m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


fsTask::fsTask(fsTask* parent)
{
    fsTaskMgr* ins = fsTaskMgr::instance();

    if (!ins->m_next_task_name)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (!parent)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = ins->m_next_task_name;
    m_execute_time = 0;

    parent->m_tree.addLast(&m_tree);

    ins->m_next_task_name = NULL;
}


fsTask::~fsTask()
{
    fsTaskMgr* ins = fsTaskMgr::instance();

    if (m_tree.hasParent())
    {
        if (ins->m_next_task_name != reinterpret_cast<const char*>(this))
        {
            fsThrow(ExceptionInvalidCall);
        }

        ins->m_next_task_name = NULL;
    }
}


fsTask::fsTask()
{
    m_tree.init(this);
    m_flag.setOn(FLAG_ACTIVE);

    m_name = "";
    m_execute_time = 0;
}


FS_DEFINE_COPY_CONSTRUCTOR(fsTask)


FS_DEFINE_OPERATOR_EQUAL(fsTask)