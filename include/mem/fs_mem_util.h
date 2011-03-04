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
    重载new运算符.
    @param[in] type 类的类型.
    @return 类的实例.
*/
#define fsNew(type) new(fsMemHelper::mallocForEngine(sizeof(type), 0, __FILE__), NULL) type

/*!
    @ingroup fsMem
    重载delete运算符.
    @param[in] ptr 要释放的实例.
    @param[in] type 类的类型.
*/
#define fsDelete(ptr, type) \
    do \
    { \
        if (!fsMemHelper::isCreated()) \
        { \
            break; \
        } \
    \
        void* ptr_ = ptr; \
    \
        if (fsMemHelper::getMemoryBlockArraySize(ptr_) > 0) \
        { \
            fsThrow(fsMemHelper::ExceptionInvalidCall); \
        } \
    \
        reinterpret_cast<type*>(ptr_)->~type(); \
        fsMemHelper::freeForEngine(ptr_); \
    } \
    while (false)


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


/*!
    @ingroup fsMem
    The dummy operator delete for system.
*/
FS_API void operator delete(void*, void*, fsException*);



#endif /* FS_MEM_UTIL_H_ */
