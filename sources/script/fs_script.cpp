/*
 * fs_script.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_script_all.h"

#include "fs_kernel_all.h"
#include "fs_res_all.h"
#include "fs_base_all.h"


fsScript* fsScript::getPrevN() const
{
    fsScriptMgr* ins = fsScriptMgr::instance();
    const fsID* id = ins->m_conf_map.getPrevKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


fsScript* fsScript::getNextN() const
{
    fsScriptMgr* ins = fsScriptMgr::instance();
    const fsID* id = ins->m_conf_map.getNextKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


fsID fsScript::getID() const
{
    return m_id;
}


bool fsScript::isValid() const
{
    return (m_err_line_no == 0);
}


u16 fsScript::getErrorLineNo() const
{
    if (isValid())
    {
        fsThrow(ExceptionInvalidCall);
    }

    return m_err_line_no;
}


u16 fsScript::getEntryNum(const char* ent_name) const
{
    if (!ent_name)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    u16 ent_num = 0;
    fsID ent_name_id = fsID::genID(ent_name);

    for (fsList<fsScriptEntry>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        fsScriptEntry* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            ent_num++;
        }
    }

    return ent_num;
}


fsScriptEntry* fsScript::getEntryFromFirstN(const char* ent_name) const
{
    if (!ent_name)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID ent_name_id = fsID::genID(ent_name);

    for (fsList<fsScriptEntry>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        fsScriptEntry* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


fsScriptEntry* fsScript::getEntryFromLastN(const char* ent_name) const
{
    if (!ent_name)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID ent_name_id = fsID::genID(ent_name);

    for (fsList<fsScriptEntry>::Item* item = m_ent_list.getLastN(); item; item = item->getPrevN())
    {
        fsScriptEntry* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


fsScriptEntry* fsScript::getEntryBeforeN(const char* ent_name, fsScriptEntry* ent) const
{
    if (!ent_name || !ent)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID ent_name_id = fsID::genID(ent_name);

    for (fsList<fsScriptEntry>::Item* item = ent->m_item.getPrevN(); item; item = item->getPrevN())
    {
        fsScriptEntry* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


fsScriptEntry* fsScript::getEntryAfterN(const char* ent_name, fsScriptEntry* ent) const
{
    if (!ent_name || !ent)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID ent_name_id = fsID::genID(ent_name);

    for (fsList<fsScriptEntry>::Item* item = ent->m_item.getNextN(); item; item = item->getNextN())
    {
        fsScriptEntry* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


fsScriptEntry* fsScript::getFirstEntryN() const
{
    fsList<fsScriptEntry>::Item* first = m_ent_list.getFirstN();

    return first ? first->getSelf() : NULL;
}


fsScriptEntry* fsScript::getLastEntryN() const
{
    fsList<fsScriptEntry>::Item* last = m_ent_list.getLastN();

    return last ? last->getSelf() : NULL;
}


static void* catcakeMalloc(u32 size)
{
    return fsMalloc(size);
}


static void catcakeFree(void* ptr)
{
    fsFree(ptr);
}


fsScript* fsScript::newPriorConfigBeforeInitialization(const char* filename)
{
    if (fsMemHelper::isCreated())
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    void* fh;

    fsTry
    {
        fh = fsMgr::openFile(filename, fsMgr::FILE_MODE_READ);
    }
    fsCatch(fsMgr::ExceptionCannotOpenFile)
    {
        fsThrow(ExceptionCannotOpenFile);
    }

    u32 data_size;
    void* data = NULL;

    fsTry
    {
        data_size = fsMgr::getFileSize(fh);

        if (data_size == 0)
        {
            fsThrow(ExceptionCannotReadFile);
        }

        data = fsLowLevelAPI::malloc(data_size);

        fsMgr::readFile(data, 0, data_size, fh);
    }
    fsCatch(fsMgr::ExceptionCannotReadFile)
    {
        fsMgr::closeFile(fh);

        if (data)
        {
            fsFree(data);
        }

        fsThrow(ExceptionCannotReadFile);
    }

    fsMgr::closeFile(fh);

    fsScript* conf = new(fsLowLevelAPI::malloc(sizeof(fsScript)), NULL) fsScript(fsID::genID(fsUtil::getBasename(filename)), data, data_size, NULL);

    fsLowLevelAPI::free(data);

    return conf;
}


void fsScript::deletePriorConfig(fsScript* conf)
{
    if (!conf)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (conf->m_malloc == catcakeMalloc)
    {
        fsThrow(ExceptionInvalidCall);
    }

    conf->~fsScript();
    fsLowLevelAPI::free(conf);
}


fsScript::fsScript(fsID id, const void* data, u32 data_size)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = catcakeMalloc;
    m_free = catcakeFree;

    parse(data, data_size);

    fsScriptMgr::instance()->m_conf_map.add(m_id, this);
}


fsScript::fsScript(fsID id, const void* data, u32 data_size, void*)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = fsLowLevelAPI::malloc;
    m_free = fsLowLevelAPI::free;

    parse(data, data_size);
}


fsScript::~fsScript()
{
    if (m_malloc == catcakeMalloc)
    {
        fsScriptMgr::instance()->m_conf_map.remove(m_id);
    }

    while (fsList<fsScriptEntry>::Item* item = m_ent_list.getFirstN())
    {
        deleteEntry(item->getSelf());
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsScript)


fsScriptEntry* fsScript::newEntry(const char* ent_name, u16 val_num)
{
    fsType<u8, fsScriptEntry::ValueType>* val_type = static_cast<fsType<u8, fsScriptEntry::ValueType>*>(m_malloc(sizeof(fsType<u8, fsScriptEntry::ValueType>) * val_num));
    fsScriptEntry::Value* val = static_cast<fsScriptEntry::Value*>(m_malloc(sizeof(fsScriptEntry::Value) * val_num));
    fsScriptEntry* ent = new(m_malloc(sizeof(fsScriptEntry)), NULL) fsScriptEntry(ent_name, val_num, val_type, val);

    m_ent_list.addLast(&ent->m_item);

    return ent;
}


void fsScript::deleteEntry(fsScriptEntry* ent)
{
    ent->~fsScriptEntry();

    for (s32 i = 0; i < ent->m_val_num; i++)
    {
        if (ent->m_val_type[i] == fsScriptEntry::TYPE_STRING)
        {
            m_free(const_cast<char*>(ent->m_val[i].val_str));
        }
    }

    m_free(ent->m_val_type);
    m_free(ent->m_val);
    m_free(ent);
}
