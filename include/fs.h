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
namespace ckBasicType
{
    typedef signed char s8; //!< 8-bit signed type.
    typedef signed short s16; //!< 16-bit signed type.
    typedef signed int s32; //!< 32-bit signed type.

    typedef unsigned char u8; //!< 8-bit unsigned type.
    typedef unsigned short u16; //!< 16-bit unsigned type.
    typedef unsigned int u32; //!< 32-bit unsigned type.

    typedef float r32; //!< 32-bit floating point type.
    typedef double r64; //!< 64-bit floating point type.

#if defined(__GNUC__)
    typedef signed long long s64; //!< 64-bit signed type.
    typedef unsigned long long u64; //!< 64-bit unsigned type.
#elif defined(_MSC_VER)
    typedef signed __int64 s64; //!< 64-bit signed type.
    typedef unsigned __int64 u64; //!< 64-bit unsigned type.
#else
#error
#endif
}

using namespace ckBasicType;

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

#endif // !FS_H_
