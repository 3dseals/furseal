/*
 * fs_low_level_mem_gen.cpp
 *
 *  Created on: 2011-3-3
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#include <stdlib.h>
#include <string.h>

#include "base/fs_low_level_api.h"

#ifndef FS_NO_THROW_EXCEPTION
#include <new>
#endif


void* fsLowLevelAPI::malloc(u32 size)
{
    void* ptr = ::malloc(size);

    if (!ptr)
    {
#ifdef FS_NO_THROW_EXCEPTION
        error("bad_alloc");
#else
        throw std::bad_alloc();
#endif
    }

    return ptr;
}


void fsLowLevelAPI::free(void* ptr)
{
    ::free(ptr);
}
