/*
 * fs_low_level_io_gen.cpp
 *
 *  Created on: 2011-3-3
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifdef _MSC_VER
#include <windows.h>
#else
#include <wchar.h>
#endif


#include <stdio.h>
#include <stdarg.h>


#include "base/fs_low_level_api.h"


void fsLowLevelAPI::printf(const char* msg)
{
    ::printf("%s", msg);

#ifdef _MSC_VER
    OutputDebugString(msg);
#endif
}


void fsLowLevelAPI::vsprintf(char* buf, u32 buf_size, const char* format, void* arg)
{
#if _MSC_VER >= 1400
    ::vsprintf_s(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
#else
    ::vsnprintf(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
#endif
}


void* fsLowLevelAPI::openFile(const char* filename, FileMode file_mode)
{
#if _MSC_VER >= 1400
    FILE* fp;
    return fopen_s(&fp, filename, (file_mode == FILE_MODE_READ) ? "rb" : "wb") ? NULL : fp;
#else
    return fopen(filename, (file_mode == FILE_MODE_READ) ? "rb" : "wb");
#endif
}


s32 fsLowLevelAPI::getFileSize(void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (ftell(fp) >= 0 && !fseek(fp, 0, SEEK_END)) ? ftell(fp) : -1;
}


bool fsLowLevelAPI::readFile(void* buf, u32 offset, u32 size, void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (!fseek(fp, offset, SEEK_SET) && (fread(buf, size, 1, fp) == 1));
}


bool fsLowLevelAPI::writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (!fseek(fp, offset, SEEK_SET) && (fwrite(buf, size, 1, fp) == 1));
}


void fsLowLevelAPI::closeFile(void* file_handler)
{
    fclose(reinterpret_cast<FILE*>(file_handler));
}
