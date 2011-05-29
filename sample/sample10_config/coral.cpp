/*
 * coral.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_main.h"


const r32 NODE_WIDTH = 8.0f;
const r32 NODE_HEIGHT = 35.0f;
const r32 GROW_SPEED = 0.08f;
const r32 OFFSET_SPEED = 0.004f;


class Coral : public fsTask
{
public:
    Coral(const fsMat& world, u8 type, u8 branch_num, u16 survival_time);
    Coral(Coral* parent, const fsMat& world, u8 type, u8 branch_num);

private:
    void init(Coral* parent, const fsMat& world, u8 type, u8 branch_num, u16 survival_time);
    virtual void onUpdate();

    fsScr* m_scr;
    Coral* m_parent;
    u8 m_type;
    u8 m_branch_num;

    bool m_is_live;
    u16 m_survival_time;
    r32 m_grow_rate;
    r32 m_v_offset;
    r32 m_fall_speed;

    fsPrim m_coral_prim;
};


void newCoral(r32 x, r32 z, u8 type, u8 branch_num, u16 survival_time)
{
    fsNewTask(Coral)(fsMat::UNIT.translate(x, 0.0f, z), type, branch_num, survival_time);
}


Coral::Coral(const fsMat& world, u8 type, u8 branch_num, u16 survival_time) : fsTask(ORDER_ZERO)
{
    init(NULL, world, type, branch_num, survival_time);
}


Coral::Coral(Coral* parent, const fsMat& world, u8 type, u8 branch_num) : fsTask(parent)
{
    init(parent, world, type, branch_num, 0);
}


void Coral::init(Coral* parent, const fsMat& world, u8 type, u8 branch_num, u16 survival_time)
{
    m_scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_parent = parent;
    m_type = type;
    m_branch_num = branch_num;

    m_is_live = true;
    m_survival_time = survival_time;
    m_grow_rate = GROW_SPEED;
    m_v_offset = 0.0f;
    m_fall_speed = 0.0f;

    m_coral_prim.init(fsPrim::MODE_TRIANGLE_STRIP, 4, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_coral_prim.setTextureID(fsID_("coral_64x128.png"));
    m_coral_prim.local() = world;

    r32 u1 = 0.25f * m_type;
    r32 u2 = u1 + 0.25f;

    m_coral_prim.setDataUV(0, u1, 0.0f);
    m_coral_prim.setDataUV(1, u2, 0.0f);
    m_coral_prim.setDataUV(2, u1, 0.0f);
    m_coral_prim.setDataUV(3, u2, 0.0f);
}


void Coral::onUpdate()
{
    if (m_survival_time > 0)
    {
        m_survival_time--;
    }

    if (m_is_live && ((m_parent && !m_parent->m_is_live) || (!m_parent && m_survival_time == 0)))
    {
        m_is_live = false;
        m_fall_speed = fsMath::rand(2.0f, 3.0f, 0.1f);

        m_coral_prim.setBlendMode(fsDraw::BLEND_HALF, true);
    }

    if (m_is_live)
    {
        m_v_offset += OFFSET_SPEED;

        if (m_v_offset > 0.5f)
        {
            m_v_offset -= 0.5f;
        }

        m_coral_prim.dataV(0) = m_coral_prim.dataV(1) = m_v_offset + 0.5f - m_grow_rate / 2.0f;
        m_coral_prim.dataV(2) = m_coral_prim.dataV(3) = m_v_offset + 0.5f;
    }
    else
    {
        if (m_coral_prim.local().trans.y > 0.0f)
        {
            m_coral_prim.local().trans.y -= m_fall_speed;
        }

        if (m_coral_prim.dataCol(0).a >= 2)
        {
            for (s32 i = 0; i < 4; i++)
            {
                m_coral_prim.dataCol(i).a -= 2;
            }

            if (!m_parent && m_coral_prim.dataCol(0).a < 2)
            {
                fsDeleteTask(this);
                return;
            }
        }
    }

    if (m_is_live && m_grow_rate < 1.0f)
    {
        m_grow_rate = fsMath::min(m_grow_rate + GROW_SPEED, 1.0f);

        if (m_branch_num > 0 && m_grow_rate >= 1.0f)
        {
            fsMat mat = fsMat::UNIT;
            mat.trans.set(0.0f, NODE_HEIGHT * m_grow_rate - 2.0f, 0.0f);

            fsMat mat1 = mat.rotateY_s32(fsMath::rand(10, 170)).rotateZ_s32(fsMath::rand(10, 30)).rotateX_s32(fsMath::rand(10, 30)).toGlobalFrom(m_coral_prim.local());
            fsMat mat2 = mat.rotateY_s32(fsMath::rand(10, -170)).rotateZ_s32(fsMath::rand(10, 30)).rotateX_s32(fsMath::rand(10, 30)).toGlobalFrom(m_coral_prim.local());

            if (mat1.y_axis.dot(fsVec::Y_UNIT) > 0.0f)
            {
                fsNewTask(Coral)(this, mat1, m_type, m_branch_num - 1);
            }

            if (mat2.y_axis.dot(fsVec::Y_UNIT) > 0.0f)
            {
                fsNewTask(Coral)(this, mat2, m_type, m_branch_num - 1);
            }
        }
    }

    fsVec pos1 = m_coral_prim.local().trans;
    fsVec pos2 = pos1 + m_coral_prim.local().y_axis;

    fsVec vec_x = m_scr->calcVisibleVector(pos1, pos2).toLocalOf_noTrans(m_coral_prim.local()) * (NODE_WIDTH / 2.0f);
    fsVec vec_y(0.0f, NODE_HEIGHT * m_grow_rate, 0.0f);

    m_coral_prim.dataPos(0) = -vec_x + vec_y;
    m_coral_prim.dataPos(1) = vec_x + vec_y;
    m_coral_prim.dataPos(2) = -vec_x;
    m_coral_prim.dataPos(3) = vec_x;
}
