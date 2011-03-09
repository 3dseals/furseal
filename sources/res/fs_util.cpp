/*
 * fs_util.cpp
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


const char* fsUtil::getBasename(const char* filename)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    const char* base = filename;

    for (const char* c = base; *c != '\0'; c++)
    {
        if (*c == '\\' || *c == '/' || *c == ':')
        {
            base = c + 1;
        }
    }

    return base;
}


const char* fsUtil::getExtension(const char* filename)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    const char* ext = getBasename(filename);

    for ( ; *ext != '\0'; ext++)
    {
        if (*ext == '.')
        {
            return ext + 1;
        }
    }

    return ext;
}
