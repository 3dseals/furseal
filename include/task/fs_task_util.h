/*
 * fs_task_util.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TASK_UTIL_H_
#define FS_TASK_UTIL_H_


/*!
    @ingroup fsTask
    结束一个任务.
    @param[in] task 任务的实例.
*/
#define fsDeleteTask(task) fsTaskMgr::deleteTaskForEngine(task)


#endif /* FS_TASK_UTIL_H_ */
