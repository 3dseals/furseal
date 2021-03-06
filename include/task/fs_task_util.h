/*
 * fs_task_util.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TASK_UTIL_H_
#define FS_TASK_UTIL_H_


/*!
    @ingroup fsTask
    创建一个任务的实例.
    @param[in] type 任务的类型.
*/
#define fsNewTask(type) new(fsMemHelper::mallocForEngine(sizeof(type) + fsTaskMgr::setNextTaskNameForEngine(__FILE__), 0, __FILE__), NULL) type


/*!
    @ingroup fsTask
    结束一个任务.
    @param[in] task 任务的实例.
*/
#define fsDeleteTask(task) fsTaskMgr::deleteTaskForEngine(task)


#endif /* FS_TASK_UTIL_H_ */
