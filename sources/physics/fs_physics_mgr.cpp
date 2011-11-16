/*
 * fs_physics_mgr.cpp
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_physics_all.h"
#include "fs_base_all.h"


fsPhysicsMgr* fsPhysicsMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsPhysicsMgr)


void fsPhysicsMgr::createAfterTask()
{
	destroyBeforeSys();

    m_instance = fsNew(fsPhysicsMgr)();
}


FS_DEFINE_MANAGER_DESTROY(fsPhysicsMgr, BeforeSys)


void fsPhysicsMgr::updateForEngine()
{
}


fsPhysicsMgr::fsPhysicsMgr()
{
    m_is_executing = false;
    m_bodies_modified = true;
    m_error_tolerance = 0.001f;
    m_next_step_size = 0.0f;
}


fsPhysicsMgr::~fsPhysicsMgr()
{
    while (fsList<fsBody>::Item* body = m_bodys.getFirstN())
    {
    	body->getSelf()->~fsBody();
    }

    while (fsList<fsContact>::Item* contact = m_cantacts.getFirstN())
    {
    	contact->getSelf()->~fsContact();
    }

    while (fsList<fsForce>::Item* force = m_forces.getFirstN())
    {
    	force->getSelf()->~fsForce();
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsPhysicsMgr)


FS_DEFINE_MANAGER_INSTANCE(fsPhysicsMgr)
