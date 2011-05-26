/*
 * fs_task.cpp
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs_task_all.h"
#include "base/fs_private_macro.h"


bool fsTask::hasOrder() const
{
    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


fsTask::TaskOrder fsTask::getOrder() const
{
    if (!hasOrder())
    {
        fsThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_order.getType();
}


bool fsTask::hasParent() const
{
    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


fsTask* fsTask::getParentN() const
{
    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


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


fsTask* fsTask::getPrevSiblingN() const
{
    fsTree<fsTask>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


fsTask* fsTask::getNextSiblingN() const
{
    fsTree<fsTask>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


fsTask* fsTask::getLastDescendant() const
{
    return m_tree.getLastDescendant()->getSelf();
}


bool fsTask::hasChild() const
{
    return m_tree.hasChild();
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


const char* fsTask::getName() const
{
    return m_name;
}


u64 fsTask::getExecuteUsecTime() const
{
    return m_execute_time;
}


bool fsTask::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void fsTask::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


void fsTask::onUpdate() {}


void fsTask::onMessage(fsID msg_id, fsMsg<4>&) {}


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
