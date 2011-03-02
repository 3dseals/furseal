/*
    Copyright (c) 2011 netsurfers
*/


#ifndef FS_PRIVATE_MACRO_H_
#define FS_PRIVATE_MACRO_H_


#define FS_DEFINE_OPERATOR_EQUAL(name) void name::operator=(const name&) {}


#define FS_DEFINE_MANAGER_INSTANCE(name) \
    name* name::instance() \
    { \
        return m_instance; \
    }


#endif // !FS_PRIVATE_MACRO_H_