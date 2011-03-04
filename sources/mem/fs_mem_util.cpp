/*
 * fs_mem_util.cpp
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


void* operator new(size_t, void* ptr, fsException*)
{
    return ptr;
}

void operator delete(void*, void*, fsException*) {}
