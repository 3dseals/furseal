/*
 * fs_res.cpp
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


fsRes::fsRes()
{
    m_data = NULL;
    m_data_size = 0;
    m_exinfo = NULL;
    m_is_auto_free = false;

    // m_id and m_ext are initialized in its own constructor
}


fsID fsRes::getID() const
{
    return m_id;
}


fsStr<char, 3> fsRes::getExtension() const
{
    return m_ext;
}


u32 fsRes::getDataSize() const
{
    return m_data_size;
}


bool fsRes::isAutoFree() const
{
    return m_is_auto_free;
}


void fsRes::init(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo, bool is_auto_free)
{
    m_id = id;
    m_ext = ext;
    m_data = data;
    m_data_size = data_size;
    m_exinfo = exinfo;
    m_is_auto_free = is_auto_free;
}


fsStr<char, 3> fsRes::toUppercase(fsStr<char, 3> str)
{
    for (s32 i = 0; i < str.getLength(); i++)
    {
        char c = str[i];

        if (c >= 'a' && c <= 'z')
        {
            str[i] = 'A' + (c - 'a');
        }
    }

    return str;
}
