/*
 * fs_script_enrty.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_script_all.h"

#include "fs_base_all.h"


fsScriptEntry* fsScriptEntry::getPrevN() const
{
    fsList<fsScriptEntry>::Item* prev = m_item.getPrevN();

    return prev ? prev->getSelf() : NULL;
}


fsScriptEntry* fsScriptEntry::getNextN() const
{
    fsList<fsScriptEntry>::Item* next = m_item.getNextN();

    return next ? next->getSelf() : NULL;
}


const fsStr<char, fsScriptEntry::MAX_NAME_LENGTH>& fsScriptEntry::getName() const
{
    return m_name;
}


u16 fsScriptEntry::getValueNum() const
{
    return m_val_num;
}


fsScriptEntry::ValueType fsScriptEntry::getValueType(u16 index) const
{
    if (index >= m_val_num)
    {
        return TYPE_S32;
    }

    return m_val_type[index].getType();
}


s32 fsScriptEntry::getValue_s32(u16 index) const
{
    if (index >= m_val_num)
    {
        return 0;
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_S32:
        return m_val[index].val_s32;

    case TYPE_R32:
        return static_cast<s32>(m_val[index].val_r32);

    default:
        return 0;
    }
}


r32 fsScriptEntry::getValue_r32(u16 index) const
{
    if (index >= m_val_num)
    {
        return 0.0f;
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_S32:
        return static_cast<r32>(m_val[index].val_s32);

    case TYPE_R32:
        return m_val[index].val_r32;

    default:
        return 0.0f;
    }
}


const char* fsScriptEntry::getValue_string(u16 index) const
{
    if (index >= m_val_num)
    {
        return "";
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_STRING:
        return m_val[index].val_str;

    default:
        return "";
    }
}


fsScriptEntry::fsScriptEntry(const char* name, u16 val_num, fsType<u8, ValueType>* val_type, Value* val)
{
    m_name = name;
    m_name_id = fsID::genID(m_name.getString());
    m_val_num = val_num;
    m_val_type = val_type;
    m_val = val;

    m_item.init(this);
}


FS_DEFINE_OPERATOR_EQUAL(fsScriptEntry)
