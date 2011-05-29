/*
 * waterdrop.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"


class Waterdrop : public fsTask
{
public:
    Waterdrop(r32 x, r32 y);

private:
    void resetParam(r32 x, r32 y);
    virtual void onUpdate();

    fsPrim m_water_prim;
    fsRend_Shader m_water_rend;

    r32 m_speed;
    r32 m_aim_rate;
};


void newWaterdrop(r32 x, r32 y)
{
    fsNewTask(Waterdrop)(x, y);
}


Waterdrop::Waterdrop(r32 x, r32 y) : fsTask(ORDER_ZERO)
{
    m_aim_rate = 0.6f;

    m_water_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_water_prim.setTextureID(fsID_("bg_512x512.png"));
    m_water_prim.setBlendMode(fsDraw::BLEND_HALF, true);

    m_water_rend.init(&m_water_prim, fsID_("refract.shd"));
    m_water_rend.set2ndTextureID(fsID_("waterdrop_64x64.png"));

    m_water_rend.uniform(0) = m_aim_rate;

    m_water_rend.attrib(0, 0) = 0.0f;
    m_water_rend.attrib(0, 1) = 0.0f;

    m_water_rend.attrib(1, 0) = 0.0f;
    m_water_rend.attrib(1, 1) = 1.0f;

    m_water_rend.attrib(2, 0) = 1.0f;
    m_water_rend.attrib(2, 1) = 1.0f;

    m_water_rend.attrib(3, 0) = 1.0f;
    m_water_rend.attrib(3, 1) = 0.0f;

    resetParam(x, y);
}


void Waterdrop::resetParam(r32 x, r32 y)
{
    r32 scale = fsMath::rand(0.5f, 1.0f, 0.1f);

    m_water_prim.setDataRect(0, fsVec::ZERO, 48.0f * scale, 64.0f * scale, fsCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
    m_water_prim.local().trans.set(x, y, scale);

    m_speed = fsMath::rand(2.0f, 4.0f, 0.1f) * scale;
}


void Waterdrop::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_R))
    {
        m_aim_rate = -m_aim_rate;
    }

    if (m_water_rend.uniform(0) < m_aim_rate)
    {
        m_water_rend.uniform(0) += 0.01f;

        if (m_water_rend.uniform(0) > m_aim_rate)
        {
            m_water_rend.uniform(0) = m_aim_rate;
        }
    }
    else if (m_water_rend.uniform(0) > m_aim_rate)
    {
        m_water_rend.uniform(0) -= 0.01f;

        if (m_water_rend.uniform(0) < m_aim_rate)
        {
            m_water_rend.uniform(0) = m_aim_rate;
        }
    }

    m_water_prim.local().trans.y -= m_speed;

    if (m_water_prim.local().trans.y < -320)
    {
        resetParam(fsMath::rand(-320.0f, 320.0f, 1.0f), fsMath::rand(300.0f, 340.0f, 1.0f));
    }
}
