/*
 * fs_private_macro.h
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifndef FS_PRIVATE_MACRO_H_
#define FS_PRIVATE_MACRO_H_


#define FS_DEFINE_COPY_CONSTRUCTOR(name) name::name(const name&) {}


#define FS_DEFINE_OPERATOR_EQUAL(name) void name::operator=(const name&) {}


#define FS_DEFINE_MANAGER_IS_CREATED(name) \
    bool name::isCreated() \
    { \
        return m_instance ? true : false; \
    }


#define FS_DEFINE_MANAGER_CREATE(name, create_suffix, destroy_suffix) \
    void name::create##create_suffix() \
    { \
        destroy##destroy_suffix(); \
    \
        m_instance = fsNew(name); \
    }


#define FS_DEFINE_MANAGER_DESTROY(name, suffix) \
    void name::destroy##suffix() \
    { \
        if (m_instance) \
        { \
            fsDelete(m_instance, name); \
            m_instance = NULL; \
        } \
    }


#define FS_DEFINE_MANAGER_INSTANCE(name) \
    name* name::instance() \
    { \
        if (!isCreated()) \
        { \
            fsThrow(ExceptionNotInitialized); \
        } \
    \
        return m_instance; \
    }


#define FS_VSPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    fsLowLevelAPI::vsprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = '\0'


#define FS_VSWPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    fsLowLevelAPI::vswprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = L'\0'


#endif // !FS_PRIVATE_MACRO_H_
