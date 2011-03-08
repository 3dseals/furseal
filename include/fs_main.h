/*
 * fs_main.h
 *
 *  Created on: 2011-3-1
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifndef FS_MAIN_H_
#define FS_MAIN_H_


#ifdef WIN32
#include <windows.h>
#endif

#include "fs.h"


#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "furseal.lib")
#else
#pragma comment(lib, "furseal.lib")
#endif
#endif


/*!
    @ingroup fsSys
    程序的入口函数.
*/
#ifdef WIN32
#define furseal() \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    { \
        fsMgr::setInitialDirectoryForEngine(__argc, __argv); \
    \
        extern void fsMain_(); \
        fsMain_(); \
    \
        return 0; \
    } \
    void fsMain_()
#else
#define furseal() \
    int main(int argc, char* argv[]) \
    { \
        fsMgr::setInitialDirectoryForEngine(argc, argv); \
    \
        extern void fsMain_(); \
        fsMain_(); \
    \
        return 0; \
    } \
    void fsMain_()
#endif


#endif // !FS_MAIN_H_
