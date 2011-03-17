/*
 * fs_id.cpp
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_gen_all.h"


const fsID fsID::ZERO;


u32 fsID::m_cur_id = 1;


fsID::fsID()
{
    m_id = 0;
}


bool fsID::operator==(fsID id) const
{
    return (m_id == id.m_id);
}


bool fsID::operator!=(fsID id) const
{
    return (m_id != id.m_id);
}


u32 fsID::operator%(u32 n) const
{
    return m_id % n;
}


u32 fsID::getValue() const
{
    return m_id;
}


fsID fsID::genID()
{
    if (m_cur_id > MAX_ID)
    {
        fsThrow(ExceptionOutOfID);
    }

    fsID id;

    id.m_id = m_cur_id++;

    return id;
}


fsID fsID::genID(const char* str)
{
    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID id;

    if (*str == '\0')
    {
        id.m_id = 0;
    }
    else
    {
        id.m_id = 0;

        for ( ; *str != '\0'; str++)
        {
            id.m_id = id.m_id * 37 + *str;
        }

        id.m_id |= 1 << (BIT_NUM - 1);
    }

    return id;
}


fsID fsID::genIDForEngine(u32 value)
{
    fsID id;

    id.m_id = value;

    return id;
}


void fsID::setCurIDForEngine(u32 value)
{
    if (value <= 0 || value > MAX_ID)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_cur_id = value;
}


s32 fsID::ThrowTooLongStringExceptionForEngine()
{
    fsThrow(ExceptionTooLongString);

    return 0;
}
