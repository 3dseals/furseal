/*
 * fs_mem_helper.h
 *
 *  Created on: 2011-3-3
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifndef FS_MEM_HELPER_H_
#define FS_MEM_HELPER_H_


/*!
    @ingroup fsMem
    内存管理助手.
*/
class FS_API fsMemHelper
{
public:
    //! @cond
	 fsDefineException(ExceptionInvalidArgument);
	 fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionInvalidSizeOfType);
    //! @endcond


    /*!
        判断内存管理器是否创建.
        @return 内存管理器是否创建.
    */
    static bool isCreated();


    /*!
        创建内存管理器,该方法只被引擎自己调用.
    */
    static void createFirst();


    /*!
        销毁内存管理器,该方法只被引擎自己调用.
    */
    static void destroyLast();


    /*!
        返回第一块内存块地址,如果不存在则返回NULL.
        @return 第一块内存块地址,没有内存块时返回NULL.
    */
    static const void* getFirstMemoryBlockN();


    /*!
        Returns the size of the specified memory block array.
        @param[in] ptr The pointer to a memory block array.
        @return The size of the specified memory block array.
    */
    static u32 getMemoryBlockArraySize(const void* ptr);


    /*!
        申请一块内存,该方法只被引擎自己调用.
        @param[in] 内存大小.
        @param[in] 数组数目.
        @param[in] 内存快名称.
        @return 这块内存块.
    */
    static void* mallocForEngine(u32 size, u32 array_size, const char* name);


    /*!
        释放一块内存,该方法只被引擎自己调用.
        @param[in] ptr 这块内存块.
    */
    static void freeForEngine(void* ptr);


private:
    static const u32 INITIAL_TEMP_BUFFER_SIZE = 1024;

    struct MemoryBlockHeader
    {
        MemoryBlockHeader* prev;
        MemoryBlockHeader* next;
        const char* name;
        u32 size;
        u32 array_size;
    };

    fsMemHelper();
    ~fsMemHelper();
    void operator=(const fsMemHelper&);

    static fsMemHelper* instance();

    MemoryBlockHeader m_mbh_start;
    MemoryBlockHeader m_mbh_end;
    u32 m_cur_used_memory_size;
    u32 m_max_used_memory_size;
    void* m_temp_buf;
    u32 m_temp_buf_size;

    static fsMemHelper* m_instance;
};


#endif // !FS_MEM_HELPER_H_
