/*
 * tetrahedron.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Tetrahedron : public fsTask
{
public:
    Tetrahedron();

private:
    virtual void onUpdate();
    virtual void onMessage(fsID msg_id, fsMsg<4>& msg);

    fsPrim m_tetra_prim;
    fsRend_Light m_tetra_rend;
    fsVec m_speed;
};


void newTetrahedron()
{
    fsNewTask(Tetrahedron);
}


Tetrahedron::Tetrahedron() : fsTask(ORDER_ZERO)
{
    m_tetra_prim.init(fsPrim::MODE_TRIANGLES, 12, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_tetra_rend.init(&m_tetra_prim, fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    fsVec pos1(0.0f, 1.225f, 0.0f);
    fsVec pos2(0.0f, -0.408f, 1.154f);
    fsVec pos3(1.0f, -0.408f, -0.577f);
    fsVec pos4(-1.0f, -0.408f, -0.577f);
    r32 size = 50.0f;

    pos1 *= size;
    pos2 *= size;
    pos3 *= size;
    pos4 *= size;

    m_tetra_prim.dataPos(0) = pos1;
    m_tetra_prim.dataPos(1) = pos2;
    m_tetra_prim.dataPos(2) = pos3;
    m_tetra_prim.dataPos(3) = pos1;
    m_tetra_prim.dataPos(4) = pos3;
    m_tetra_prim.dataPos(5) = pos4;
    m_tetra_prim.dataPos(6) = pos1;
    m_tetra_prim.dataPos(7) = pos4;
    m_tetra_prim.dataPos(8) = pos2;
    m_tetra_prim.dataPos(9) = pos4;
    m_tetra_prim.dataPos(10) = pos3;
    m_tetra_prim.dataPos(11) = pos2;

    m_tetra_prim.dataCol(9) = m_tetra_prim.dataCol(10) = m_tetra_prim.dataCol(11) = fsCol::ZERO;

    m_tetra_rend.calcNormalAsTriangles(false);

    m_speed = fsVec::ZERO;
}


void Tetrahedron::onUpdate()
{
    fsVec pos = m_tetra_prim.local().trans;

    m_speed.y -= 0.1f;

    pos += m_speed;

    if (pos.y < 0.0f)
    {
        pos.y = 0.0f;

        m_speed.y = 4.0f;

        m_speed.x += fsMath::rand(-1.0f, 1.0f, 0.1f);
        m_speed.z += fsMath::rand(-1.0f, 1.0f, 0.1f);

        m_speed.x = fsMath::clamp(m_speed.x, -3.0f, 3.0f);
        m_speed.z = fsMath::clamp(m_speed.z, -3.0f, 3.0f);
    }

    if (pos.x > 180.0f)
    {
        pos.x = 180.0f;
        m_speed.x = -fsMath::abs(m_speed.x);
    }

    if (pos.x < -180.0f)
    {
        pos.x = -180.0f;
        m_speed.x = fsMath::abs(m_speed.x);
    }

    if (m_tetra_prim.local().trans.z > 180.0f)
    {
        pos.z = 180.0f;
        m_speed.z = -fsMath::abs(m_speed.z);
    }

    if (m_tetra_prim.local().trans.z < -180.0f)
    {
        pos.z = -180.0f;
        m_speed.z = fsMath::abs(m_speed.z);
    }

    m_tetra_prim.local() = m_tetra_prim.local().rotateY_s32(1).rotateX_s32(2);
    m_tetra_prim.local().trans = pos;
}


void Tetrahedron::onMessage(fsID msg_id, fsMsg<4>& msg)
{
    if (msg_id == fsID_("CHANGE LIGHT SET"))
    {
        m_tetra_rend.setLightSetID(msg.getParam<fsID>(0));
    }
}
