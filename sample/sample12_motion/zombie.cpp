/*
 * zombie.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Zombie : public fsTask
{
public:
    Zombie(u16 zombie_no, r32 x, r32 y, r32 z);

private:
    enum ZombieState
    {
        STATE_WALK, //
        STATE_FALL_DOWN, //
        STATE_GET_UP
    };

    virtual void onUpdate();

    u16 m_zombie_no;
    ZombieState m_state;
    u16 m_state_cntr;
    r32 m_aim_z;

    fsMdl m_zombie_mdl;
    fsMot m_zombie_mot;
    fsMdl m_icon_mdl;
};


void newZombie(u16 zombie_no, r32 x, r32 y, r32 z)
{
    fsNewTask(Zombie)(zombie_no, x, y, z);
}


Zombie::Zombie(u16 zombie_no, r32 x, r32 y, r32 z) : fsTask(ORDER_ZERO)
{
    m_zombie_no = zombie_no;
    m_state = STATE_WALK;
    m_state_cntr = 0;
    m_aim_z = z;

    m_zombie_mdl.init(fsID_("zombie.mdl"), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_zombie_mdl.setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_zombie_mdl.getRootDraw()->local() = fsMat::UNIT;
    m_zombie_mdl.getRootDraw()->local().trans.set(x, y, z);

    m_zombie_mot.init(&m_zombie_mdl, fsID_("zombie.mot"));
    m_zombie_mot.play(&m_zombie_mdl, 1, fsMot::PLAY_LOOP, 1.0f, 0);

    m_icon_mdl.init(fsID_("fs_icon.mdl"), fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_icon_mdl.setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_icon_mdl.getRootDraw()->local().trans.set(-270.0f + m_zombie_no * 80.0f, -195.0f);
    m_icon_mdl.getRootDraw()->local() = m_icon_mdl.getRootDraw()->local().rotateY_s32(m_zombie_no * 15);
}


void Zombie::onUpdate()
{
    if (m_state == STATE_WALK && fsInputMgr::isPressed(fsInputMgr::KEY_LBUTTON))
    {
        fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
        r32 mouse_x = scr->framebufferXToScreenX(fsInputMgr::getMouseX());
        r32 mouse_y = scr->framebufferYToScreenY(fsInputMgr::getMouseY());

        fsVec hit_to(mouse_x, mouse_y, -scr->getFocusDist());
        hit_to *= 10.0f;
        hit_to = hit_to.toGlobalFrom(scr->view());

        fsCdt::Ray hit_ray;
        hit_ray.setPos(scr->view().trans, hit_to);

        fsCdt::Box box;
        box.setSize(64.0f, 128.0f, 64.0f);
        box.setWorld(m_zombie_mdl.getRootDraw()->local());

        fsVec hit_pos;

        if (fsCdt::intersect(&hit_pos, hit_ray, box))
        {
            m_zombie_mot.play(&m_zombie_mdl, 2, fsMot::PLAY_KEEP, 1.0f, 5);

            m_state = STATE_FALL_DOWN;
            m_state_cntr = 100;

            extern void newBlood(const fsVec& pos);

            for (s32 i = 0; i < 5; i++)
            {
                newBlood(hit_pos);
            }
        }
    }

    if (m_state_cntr > 0)
    {
        m_state_cntr--;

        if (m_state_cntr == 0)
        {
            switch (m_state)
            {
            case STATE_FALL_DOWN:
                m_state = STATE_GET_UP;
                m_state_cntr = 100;
                m_zombie_mot.play(&m_zombie_mdl, 0, fsMot::PLAY_LOOP, 0.1f, 10);
                break;

            case STATE_GET_UP:
                m_state = STATE_WALK;
                m_zombie_mot.play(&m_zombie_mdl, 1, fsMot::PLAY_LOOP, 1.0f, 30);
                break;

            default:
                break;
            }
        }
    }

    m_icon_mdl.getRootDraw()->local() = m_icon_mdl.getRootDraw()->local().rotateY_s32(1);

    if (m_state == STATE_WALK)
    {
        if (m_zombie_mdl.getRootDraw()->local().trans.z > m_aim_z)
        {
            m_zombie_mdl.getRootDraw()->local().trans.z -= 0.3f;
        }

        m_icon_mdl.getRootDraw()->local().trans.y = fsMath::interp(m_icon_mdl.getRootDraw()->local().trans.y, -195.0f, 0.15f);
    }
    else
    {
        m_zombie_mdl.getRootDraw()->local().trans.z += 0.3f;

        m_icon_mdl.getRootDraw()->local().trans.y = fsMath::interp(m_icon_mdl.getRootDraw()->local().trans.y, -300.0f, 0.15f);
    }

    m_zombie_mot.update(&m_zombie_mdl);
}
