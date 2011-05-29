/*
 * jeep.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_main.h"


class Jeep : public fsTask
{
public:
    Jeep();

private:
    virtual void onUpdate();
    void resetPivot();

    fsMdl m_jeep_mdl;
    fsMat m_world;
    fsVec m_pivot;
};


void newJeep()
{
    fsNewTask(Jeep);
}


Jeep::Jeep() : fsTask(ORDER_ZERO)
{
    m_jeep_mdl.init(fsID_("jeep.3ds"), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_jeep_mdl.setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    m_world = fsMat::UNIT.translate(fsMath::rand(-100.0f, 100.0f, 10.0f), 0.0f, fsMath::rand(-100.0f, 100.0f, 10.0f));

    resetPivot();
}


void Jeep::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_T))
    {
        m_jeep_mdl.setTextureID((m_jeep_mdl.getTextureID() == fsID::ZERO) ? fsID_("jeep.png") : fsID::ZERO);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_L))
    {
        m_jeep_mdl.setLightSetID((m_jeep_mdl.getLightSetID() == fsID::ZERO) ? fsDrawMgr::DEFAULT_LIGHT_SET_ID : fsID::ZERO);
    }

    r32 sq_dist = m_world.trans.sqDist(m_pivot);

    if (sq_dist < 200.0f * 200.0f || sq_dist > 1000.0f * 1000.0f)
    {
        resetPivot();
    }

    fsVec local_pivot = m_pivot.toLocalOf(m_world);

    if (local_pivot.x > 10.0f)
    {
        m_world = m_world.rotateY_r32(-1.5f);
    }
    else if (local_pivot.x < -10.0f)
    {
        m_world = m_world.rotateY_r32(1.5f);
    }

    m_world.trans -= m_world.z_axis * 4.0f;

    if (m_world.trans.sqLength() < 250.0f * 250.0f)
    {
        m_world.trans = m_world.trans.normalize() * 250.0f;
    }

    m_jeep_mdl.getRootDraw()->local() = m_world.rotateY_s32(180).rotateX_s32(-90);

    fsMat mat = fsMat::UNIT;
    mat.trans = m_pivot;
    fsDebugMgr::drawAxis(mat, 50.0f, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


void Jeep::resetPivot()
{
    m_pivot = fsVec::X_UNIT.rotateY_s32(fsMath::rand(-180, 180)) * 400.0f;
}
