/*
 * fs_res_mgr.cpp
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"
#include "base/fs_private_macro.h"


fsResMgr* fsResMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsResMgr)


FS_DEFINE_MANAGER_CREATE(fsResMgr, AfterTask, BeforeSys)


FS_DEFINE_MANAGER_DESTROY(fsResMgr, BeforeSys)


u16 fsResMgr::getTypeNum()
{
    return instance()->m_type_info_map.getDataNum();
}


u16 fsResMgr::getResourceNum()
{
    return instance()->m_res_map.getDataNum();
}


void fsResMgr::addType(fsStr<char, 3> ext, Initializer init, Finalizer final)
{
    fsResMgr* ins = instance();

    if (ext == "" || !init || !final)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    ext = fsRes::toUppercase(ext);

    if (ins->m_type_info_map.getN(ext))
    {
        fsThrow(ExceptionSameExtensionExists);
    }

    TypeInfo ti;
    ti.ext = ext;
    ti.init = init;
    ti.final = final;

    ins->m_type_info_map.add(ext, ti);
}


void fsResMgr::removeType(fsStr<char, 3> ext)
{
    fsResMgr* ins = instance();

    if (ext == "")
    {
        fsThrow(ExceptionInvalidArgument);
    }

    ext = fsRes::toUppercase(ext);

    fsTry
    {
        ins->m_type_info_map.remove(ext);
    }
    fsCatch(fsMap<fsStr<char, 3>, TypeInfo>::ExceptionNotFound)
    {
        fsThrow(ExceptionNotFound);
    }
}


bool fsResMgr::hasResource(fsID id)
{
    fsResMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_res_map.getN(id) ? true : false;
}


fsRes fsResMgr::getResource(fsID id)
{
    fsResMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes* res = ins->m_res_map.getN(id);

    if (!res)
    {
        fsThrow(ExceptionNotFound);
    }

    return *res;
}


void fsResMgr::addResource(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free)
{
    fsResMgr* ins = instance();

    if (id == fsID::ZERO || !data || data_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    ext = fsRes::toUppercase(ext);

    fsTry
    {
        fsRes res;
        res.init(id, ext, data, data_size, NULL, is_auto_free);

        ins->m_res_map.add(id, res);
    }
    fsCatch(fsMap<fsID, fsRes>::ExceptionSameKeyExists)
    {
        fsThrow(ExceptionSameIDExists);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(ext);

    if (ti)
    {
        fsRes* res = ins->m_res_map.get(id);

        (*ti->init)(res->m_id, res->m_ext, res->m_data, res->m_data_size, &res->m_exinfo);
    }
}


void fsResMgr::removeResource(fsID id)
{
    fsResMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes* res;

    fsTry
    {
        res = ins->m_res_map.get(id);
    }
    fsCatch(fsMap<fsID, fsRes>::ExceptionNotFound)
    {
        fsThrow(ExceptionNotFound);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(res->m_ext);

    if (ti)
    {
        (*ti->final)(res->m_id, res->m_ext, res->m_data, res->m_data_size, res->m_exinfo);
    }

    if (res->m_is_auto_free)
    {
        fsFree(const_cast<void*>(res->m_data));
    }

    ins->m_res_map.remove(id);
}


void fsResMgr::loadResource(const char* filename, bool is_type_detect)
{
    instance();

    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return loadResourceAs(fsID::genID(fsUtil::getBasename(filename)), filename, is_type_detect);
}


void fsResMgr::loadResourceAs(fsID id, const char* filename, bool is_type_detect)
{
    instance();

    if (id == fsID::ZERO || !filename)
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

        data = fsMalloc(data_size);

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

    fsTry
    {
        addResource(id, is_type_detect ? fsUtil::getExtension(filename) : "", data, data_size, true);
    }
    fsCatch(ExceptionSameIDExists)
    {
        fsFree(data);

        fsThrow(ExceptionSameIDExists);
    }
}


const fsStr<char, 3>* fsResMgr::getFirstTypeN()
{
    return instance()->m_type_info_map.getFirstKeyN();
}


const fsStr<char, 3>* fsResMgr::getNextTypeN(fsStr<char, 3> ext)
{
    fsResMgr* ins = instance();

    if (ext == "")
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_type_info_map.getNextKeyN(ext);
}


const fsRes* fsResMgr::getFirstResourceN()
{
    fsResMgr* ins = instance();
    const fsID* id = ins->m_res_map.getFirstKeyN();

    return id ? ins->m_res_map.get(*id) : NULL;
}


const fsRes* fsResMgr::getNextResourceN(fsID id)
{
    fsResMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    const fsID* id2 = ins->m_res_map.getNextKeyN(id);

    return id2 ? ins->m_res_map.get(*id2) : NULL;
}


fsResMgr::fsResMgr()
{
    m_type_info_map.init(TYPE_HASH_SIZE);
    m_res_map.init(RESOURCE_HASH_SIZE);
}


fsResMgr::~fsResMgr()
{
    while (const fsRes* res = getFirstResourceN())
    {
        removeResource(res->m_id);
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsResMgr)


FS_DEFINE_MANAGER_INSTANCE(fsResMgr)
