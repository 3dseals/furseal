/*
 * fs_mem_util.h
 *
 *  Created on: 2011-3-3
 *      Author: administrator
 *
*  Copyright (c) 2011 netsurfers
 */

#ifndef FS_MEM_UTIL_H_
#define FS_MEM_UTIL_H_


/*!
    @ingroup fsMem
    申请一块内存块.
    @param[in] size 内存大小.
    @return 这块内存块.
*/
#define fsMalloc(size) fsMemHelper::mallocForEngine(size, 0, __FILE__)


/*!
    @ingroup fsMem
    释放一块内存块.
    @param[in] ptr 被释放的内存块.
*/
#define fsFree(ptr) fsMemHelper::freeForEngine(ptr)


/*!
    @ingroup fsMem
    新建一块内存块.
    @param[in] ptr 这块内存块的首地址.
    @return 这块内存块.
*/
FS_API void* operator new(size_t, void* ptr, fsException*);


#endif /* FS_MEM_UTIL_H_ */
