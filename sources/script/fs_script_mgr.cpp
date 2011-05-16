/*
 * fs_script_mgr.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_script_all.h"

#include "fs_res_all.h"
#include "fs_base_all.h"


fsScriptMgr* fsScriptMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsScriptMgr)


FS_DEFINE_MANAGER_CREATE(fsScriptMgr, AfterRes, BeforeRes)


FS_DEFINE_MANAGER_DESTROY(fsScriptMgr, BeforeRes)


bool fsScriptMgr::hasConfig(fsID id)
{
    fsScriptMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_conf_map.getN(id) ? true : false;
}


fsScript* fsScriptMgr::getConfig(fsID id)
{
    fsScriptMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsScript** conf = ins->m_conf_map.getN(id);

    if (!conf)
    {
        fsThrow(ExceptionNotFound);
    }

    return *conf;
}


fsScript* fsScriptMgr::newConfig(fsID id, const void* data, u32 data_size)
{
    fsScriptMgr* ins = instance();

    if (id == fsID::ZERO || !data || data_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_conf_map.getN(id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsScript)(id, data, data_size);
}


void fsScriptMgr::deleteConfig(fsID id)
{
    instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDelete(getConfig(id), fsScript);
}


fsScript* fsScriptMgr::getFirstConfigN()
{
    fsScriptMgr* ins = instance();
    const fsID* id = ins->m_conf_map.getFirstKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


fsScript* fsScriptMgr::getLastConfigN()
{
    fsScriptMgr* ins = instance();
    const fsID* id = ins->m_conf_map.getLastKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


fsScriptMgr::fsScriptMgr()
{
    m_conf_map.init(CONFIG_HASH_SIZE);

    fsResMgr::addType("FSL", configInitializer, configFinalizer);
}


fsScriptMgr::~fsScriptMgr()
{
    while (const fsScript* conf = getFirstConfigN())
    {
        deleteConfig(conf->m_id);
    }

    fsResMgr::removeType("FSL");
}


FS_DEFINE_OPERATOR_EQUAL(fsScriptMgr)


FS_DEFINE_MANAGER_INSTANCE(fsScriptMgr)


void fsScriptMgr::configInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    newConfig(id, data, data_size);
}


void fsScriptMgr::configFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    deleteConfig(id);
}
