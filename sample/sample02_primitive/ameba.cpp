/*
 * ambeba.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Ameba : public fsTask
{
public:
    Ameba(r32 x, r32 y, fsCol color);

private:
    static const u16 CIRCLE_VERT_NUM = 36;
    static const u16 AMEBA_VERT_NUM = CIRCLE_VERT_NUM + 2;
    static const u16 ONE_VERT_DEG = 360 / CIRCLE_VERT_NUM;

    void setAmebaPos(s32 phase);
    virtual void onUpdate();

    fsPrim m_ameba_prim;
    s32 m_phase;
};


void newAmeba(r32 x, r32 y, fsCol color)
{
    fsNewTask(Ameba)(x, y, color);
}


Ameba::Ameba(r32 x, r32 y, fsCol color) : fsTask(ORDER_ZERO)
{
    m_ameba_prim.init(fsPrim::MODE_TRIANGLE_FAN, AMEBA_VERT_NUM, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_ameba_prim.setBlendMode(fsDraw::BLEND_HALF, true);

    setAmebaPos(m_phase);

    m_ameba_prim.dataCol(0).set(color.r, color.g, color.b, 128);

    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
        m_ameba_prim.dataCol(i).set(color.r, color.g, color.b, 96);
    }

    m_ameba_prim.local().trans.set(x, y);

    m_phase = 0;
}


void Ameba::setAmebaPos(s32 phase)
{
    m_ameba_prim.dataPos(0) = fsVec::ZERO;

    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
        fsVec dir(fsMath::cos_s32((i - 1) * ONE_VERT_DEG), fsMath::sin_s32((i - 1) * ONE_VERT_DEG));
        r32 rad = fsMath::sin_s32((i - 1) * 60) * fsMath::sin_s32(phase) * 8.0f + 64.0f;

        m_ameba_prim.dataPos(i) = dir * rad;
    }
}


void Ameba::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_P))
    {
        if (m_ameba_prim.getPrimMode() == fsPrim::MODE_TRIANGLE_FAN)
        {
            m_ameba_prim.setPrimMode(fsPrim::MODE_LINE_STRIP);
        }
        else
        {
            m_ameba_prim.setPrimMode(fsPrim::MODE_TRIANGLE_FAN);
        }
    }

    m_phase += 4;
    setAmebaPos(m_phase);
}
