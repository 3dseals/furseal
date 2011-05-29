/*
 * blood.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Blood : public fsTask
{
public:
    Blood(const fsVec& pos);

private:
    virtual void onUpdate();

    fsVec m_speed;
    fsMdl m_blood_mdl;
};


void newBlood(const fsVec& pos)
{
    fsNewTask(Blood)(pos);
}


Blood::Blood(const fsVec& pos) : fsTask(ORDER_ZERO)
{
    m_speed.set(fsMath::rand(-2.0f, 2.0f, 0.1f), fsMath::rand(0.0f, 2.0f, 0.1f), fsMath::rand(-1.0f, -2.0f, 0.1f));

    m_blood_mdl.init(fsID_("blood.mdl"), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_blood_mdl.setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_blood_mdl.getRootDraw()->local().trans = pos;
}


void Blood::onUpdate()
{
    m_blood_mdl.getRootDraw()->local() = m_blood_mdl.getRootDraw()->local().rotateX_s32(3);
    m_blood_mdl.getRootDraw()->local().trans += m_speed;

    m_speed.y -= 0.1f;

    if (m_blood_mdl.getRootDraw()->local().trans.y < 0.0f)
    {
        fsDeleteTask(this);
    }
}
