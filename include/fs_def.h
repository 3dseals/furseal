/*
 * fs_def.h
 *
 *  Created on: 2011-3-17
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_DEF_H_
#define FS_DEF_H_


/*!
    @defgroup fsDef fsDef -- 全局宏定义
    定义furseal引擎全局宏.
*/
#if defined(FS_IPHONE) || defined(FS_ANDROID)
#define FS_NO_THROW_EXCEPTION
#endif


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


#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif


#undef min
#undef max


#ifndef NULL
#define NULL 0
#endif


#ifdef __GNUC__
typedef __SIZE_TYPE__ size_t;
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
const u32 FURSEAL_VERSION = 13; // 0.1.3


/*!
    @ingroup fsDef
    furseal引擎异常处理.
*/
class fsException
{
public:
    /*!
        异常的结构.
        @param[in] exception 异常名称.
        @param[in] file 抛出异常的文件名.
        @param[in] line 抛出异常的行号位置.
    */
    fsException(const char* exception, const char* file, u32 line)
    {
        m_exception = exception;
        m_file = file;
        m_line = line;
    }

    /*!
        获取异常名称.
        @return 异常名称.
    */
    const char* getException() const
    {
        return m_exception;
    }

    /*!
        获取异常文件名.
        @return 异常文件名.
    */
    const char* getFile() const
    {
        return m_file;
    }

    /*!
        获取异常行号位置.
        @return 异常行号位置.
    */
    u32 getLine() const
    {
        return m_line;
    }

private:
    const char* m_exception;
    const char* m_file;
    u32 m_line;
};


#ifdef FS_NO_THROW_EXCEPTION

FS_API void fsSubstituteThrow(const char* exception, const char* file, u32 line);

#define fsDefineException(e)
#define fsTry if (true)
#define fsThrow(e) fsSubstituteThrow(#e, __FILE__, __LINE__)
#define fsCatch(...) if (false)

#else

/*!
    @ingroup fsDef
    定义一个异常.
    @param[in] 异常类型名称.
*/
#define fsDefineException(e) \
    class e : public fsException \
    { \
    public: \
        e(const char* exception, const char* file, u32 line) : fsException(exception, file, line) {} \
    }


/*!
    @ingroup fsDef
    捕获异常.
*/
#define fsTry try


/*!
    @ingroup fsDef
    抛出异常.
    @param[in] e 异常类型名称.
*/
#define fsThrow(e) throw e(#e, __FILE__, __LINE__)


/*!
    @ingroup fsDef
    异常处理.
    @param[in] ... 异常参数.
*/
#define fsCatch(...) catch (__VA_ARGS__)

#endif // FS_NO_THROW_EXCEPTION


#endif /* FS_DEF_H_ */
