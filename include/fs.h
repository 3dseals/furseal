/*
    Copyright (c) 2011 netsurfers
*/


#ifndef FS_H_
#define FS_H_

/*!
    @defgroup fsDef fsDef -- 全局宏定义
    定义furseal引擎全局宏.
*/
#ifndef FS_API
#ifdef  _MSC_VER
#ifdef  FURSEAL_EXPORTS
#define FS_API __declspec(dllexport)
#else
#define FS_API __declspec(dllimport)
#endif
#else
#define FS_API
#endif
#endif

#ifndef NULL
#define NULL 0
#endif

/*!
    @ingroup fsDef
    引擎的基本数据类型.
*/
namespace fsBasicType
{
    typedef signed char s8; //!< 8位有符号数据.
    typedef signed short s16; //!< 16位有符号数据.
    typedef signed int s32; //!< 32位有符号数据.

    typedef unsigned char u8; //!< 8位无符号数据.
    typedef unsigned short u16; //!< 16位无符号数据.
    typedef unsigned int u32; //!< 32位无符号数据.

    typedef float r32; //!< 32位浮点数据.
    typedef double r64; //!< 64位浮点数据.

#if defined(__GNUC__)
    typedef signed long long s64; //!< 64位有符号数据.
    typedef unsigned long long u64; //!< 64位无符号数据.
#elif defined(_MSC_VER)
    typedef signed __int64 s64; //!< 64位有符号数据.
    typedef unsigned __int64 u64; //!< 64位无符号数据.
#else
#error
#endif
}

using namespace fsBasicType;

/*!
    @ingroup fsDef
    furseal引擎版本.
*/
const u32 FURSEAL_VERSION = 2; // 0.0.2

/*!
    @defgroup fsKen fsKen -- 引擎内核
    提供控制管理引擎的功能.
*/
#include "kernel/fs_mgr.h"
#include "kernel/fs_core.h"

#endif // !FS_H_
