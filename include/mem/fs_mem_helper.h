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
    fsDefineException(ExceptionNotInitialized);
    //! @endcond


    /*!
        填充内存块.
        @param[out] buf 内存块首地址.
        @param[in] value 填充的值.
        @param[in] size 填充的大小.
    */
    static void memset(void* buf, u8 value, u32 size);

    /*!
        复制源内存块到目的内存块.
        @param[out] dest 目的内存块首指针.
        @param[in] src 源内存块首指针.
        @param[in] size 复制的大小.
    */
    static void memcpy(void* dest, const void* src, u32 size);


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
        Returns the size of the used memory.
        @return The size of the used memory.
    */
    static u32 getCurUsedMemorySize();


    /*!
        Returns the maximum size of the used memory.
        @return The maximum size of the used memory.
    */
    static u32 getMaxUsedMemorySize();


    /*!
        返回第一块内存块地址,如果不存在则返回NULL.
        @return 第一块内存块地址,没有内存块时返回NULL.
    */
    static const void* getFirstMemoryBlockN();


    /*!
        Returns the next memory block of the specified memory. If the first memory block doesn't exist, return NULL.
        @param[in] ptr The pointer to the memory.
        @return The next memory block of the specified memory.
    */
    static const void* getNextMemoryBlockN(const void* ptr);


    /*!
        Returns the name of the specified memory.
        @param[in] ptr The pointer to the memory.
        @return The name of the specified memory.
    */
    static const char* getMemoryBlockName(const void* ptr);


    /*!
        Returns the size of the specified memory block.
        @param[in] ptr The pointer to a memory block.
        @return The size of the specified memory block.
    */
    static u32 getMemoryBlockSize(const void* ptr);


    /*!
        返回内存块数组的尺寸.
        @param[in] ptr 指向内存块数组的指针.
        @return 内存块数组的尺寸.
    */
    static u32 getMemoryBlockArraySize(const void* ptr);


    /*!
        Returns the size of the memory block header.
        @return The size of the memory block header.
    */
    static u32 getMemoryBlockHeaderSize();


    /*!
        申请一块内存,该方法只被引擎自己调用.
        @param[in] 内存大小.
        @param[in] 数组数目.
        @param[in] 内存快名称.
        @return 这块内存块.
    */
    static void* mallocForEngine(u32 size, u32 array_size, const char* name);


    /*!
        Resizes the specified memory. This method is only for system.
        @param[in,out] ptr The pointer to a memory.
        @param[in] size The size of a memory.
        @param[in] array_size The size of an array.
        @param[in] name The name of a memory.
    */
    static void* reallocForEngine(void* ptr, u32 size, u32 array_size, const char* name);


    /*!
        释放一块内存,该方法只被引擎自己调用.
        @param[in] ptr 这块内存块.
    */
    static void freeForEngine(void* ptr);


    /*!
        申请一块临时缓存,该方法只被引擎自己调用.
        @param[in] size 缓存大小.
        @return 这块缓存.
    */
    static void* allocTempBufferForEngine(u32 size);

    /*!
        返回缓存大小,该方法只被引擎自己调用.
        @return 缓存大小.
    */
    static u32 getTempBufferSizeForEngine();


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
