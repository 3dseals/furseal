/*
 * fs_mem_helper.cpp
 *
 *  Created on: 2011-3-3
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#include "fs.h"

#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


fsMemHelper* fsMemHelper::m_instance = NULL;


void fsMemHelper::memset(void* buf, u8 value, u32 size)
{
    if (!buf || size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::memset(buf, value, size);
}


void fsMemHelper::memcpy(void* dest, const void* src, u32 size)
{
    if (!dest || !src || size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::memcpy(dest, src, size);
}


FS_DEFINE_MANAGER_IS_CREATED(fsMemHelper)


void fsMemHelper::destroyLast()
{
    if (m_instance)
    {
        m_instance->~fsMemHelper();
        fsLowLevelAPI::free(m_instance);
    }
}


void fsMemHelper::createFirst()
{
    if (!(sizeof(s8) == 1 && sizeof(s16) == 2 && sizeof(s32) == 4 && sizeof(s64) == 8 && //
        sizeof(u8) == 1 && sizeof(u16) == 2 && sizeof(u32) == 4 && sizeof(u64) == 8 && //
        sizeof(r32) == 4 && sizeof(r64) == 8))
    {
        fsThrow(ExceptionInvalidSizeOfType);
    }

    destroyLast();

    m_instance = new(fsLowLevelAPI::malloc(sizeof(fsMemHelper)), NULL) fsMemHelper;

    m_instance->m_temp_buf_size = INITIAL_TEMP_BUFFER_SIZE;
    m_instance->m_temp_buf = fsMalloc(m_instance->m_temp_buf_size);
}


const void* fsMemHelper::getFirstMemoryBlockN()
{
	fsMemHelper* ins = instance();

    return (ins->m_mbh_start.next != &ins->m_mbh_end) ? ins->m_mbh_start.next + 1 : NULL;
}


u32 fsMemHelper::getMemoryBlockArraySize(const void* ptr)
{
    if (!ptr)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return (reinterpret_cast<const MemoryBlockHeader*>(ptr) - 1)->array_size;
}


void* fsMemHelper::mallocForEngine(u32 size, u32 array_size, const char* name)
{
	fsMemHelper* ins = instance();

    if (size == 0 || !name)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    size += sizeof(MemoryBlockHeader);

    MemoryBlockHeader* mbh = reinterpret_cast<MemoryBlockHeader*>(fsLowLevelAPI::malloc(size));

    mbh->prev = ins->m_mbh_end.prev;
    mbh->next = &ins->m_mbh_end;
    mbh->name = name;
    mbh->size = size;
    mbh->array_size = array_size;

    mbh->prev->next = mbh;
    mbh->next->prev = mbh;

    ins->m_cur_used_memory_size += size;

    if (ins->m_cur_used_memory_size > ins->m_max_used_memory_size)
    {
        ins->m_max_used_memory_size = ins->m_cur_used_memory_size;
    }

    return mbh + 1;
}


void fsMemHelper::freeForEngine(void* ptr)
{
    if (!isCreated())
    {
        return;
    }

    fsMemHelper* ins = instance();

    if (!ptr)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    MemoryBlockHeader* mbh = reinterpret_cast<MemoryBlockHeader*>(ptr) - 1;

    ins->m_cur_used_memory_size -= mbh->size;

    mbh->prev->next = mbh->next;
    mbh->next->prev = mbh->prev;

    fsLowLevelAPI::free(mbh);
}


fsMemHelper::fsMemHelper()
{
    m_mbh_start.prev = NULL;
    m_mbh_start.next = &m_mbh_end;
    m_mbh_start.name = __FILE__;
    m_mbh_start.size = 0;
    m_mbh_start.array_size = 0;

    m_mbh_end.prev = &m_mbh_start;
    m_mbh_end.next = NULL;
    m_mbh_end.name = __FILE__;
    m_mbh_end.size = 0;
    m_mbh_end.array_size = 0;

    m_temp_buf = NULL;
    m_temp_buf_size = 0;

    m_cur_used_memory_size = m_max_used_memory_size = sizeof(fsMemHelper);
}


fsMemHelper::~fsMemHelper()
{
    while (void* ptr = const_cast<void*>(getFirstMemoryBlockN()))
    {
        fsFree(ptr);
    }

    m_instance = NULL;
}


FS_DEFINE_OPERATOR_EQUAL(fsMemHelper)


FS_DEFINE_MANAGER_INSTANCE(fsMemHelper)
