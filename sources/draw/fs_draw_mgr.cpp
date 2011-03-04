/*
 * fs_draw_mgr.cpp
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"
#include "base/fs_private_macro.h"


fsDrawMgr* fsDrawMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsDrawMgr)


void fsDrawMgr::createAfterRes()
{
    destroyBeforeRes();

    m_instance = fsNew(fsDrawMgr);

}


FS_DEFINE_MANAGER_DESTROY(fsDrawMgr, BeforeRes)


void fsDrawMgr::renderForEngine()
{
    fsDrawMgr* ins = instance();

    if (!ins->m_is_render)
    {
        return;
    }
}


fsDrawMgr::fsDrawMgr()
{

}


fsDrawMgr::~fsDrawMgr()
{

}


FS_DEFINE_OPERATOR_EQUAL(fsDrawMgr)


FS_DEFINE_MANAGER_INSTANCE(fsDrawMgr)
