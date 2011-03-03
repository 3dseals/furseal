/*
    Copyright (c) 2011 netsurfers
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
