/*
    Copyright (c) 2011 netsurfers
*/


#ifndef FS_LOW_LEVEL_API_H_
#define FS_LOW_LEVEL_API_H_


#include "fs.h"


class fsLowLevelAPI
{
public:
    static bool createApplication(const char* title, u16 width, u16 height, u16 sys_flag);
    static void destroyApplication();
    static void setInitialDirectory(s32 argc, char** argv);
};

#endif // !FS_LOW_LEVEL_API_H_