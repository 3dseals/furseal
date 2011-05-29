/*
 * scenario_plater.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_main.h"


class ScenarioPlayer : public fsTask
{
public:
    ScenarioPlayer();

private:
    virtual void onUpdate();

    fsScr* m_scr;
    r32 m_rot_y;

    fsScript* m_conf;
    fsScriptEntry* m_ent;
    u16 m_wait_cntr;

    fsPrim m_bg_prim;
};


void newScenarioPlayer()
{
    fsNewTask(ScenarioPlayer);
}


ScenarioPlayer::ScenarioPlayer() : fsTask(ORDER_MINUS_1)
{
    m_scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_rot_y = 0.0f;

    m_conf = fsScriptMgr::getConfig(fsID_("scenario.fsl"));
    m_ent = m_conf->getEntryFromFirstN("command");
    m_wait_cntr = 0;

    if (!m_conf->isValid())
    {
        fsDebugMgr::trace("*** config file error in %d ***\n", m_conf->getErrorLineNo());
    }

    m_bg_prim.init(fsPrim::MODE_TRIANGLE_FAN, 14, fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_bg_prim.dataPos(0) = fsVec::ZERO;
    m_bg_prim.dataCol(0).set(0, 96, 64);

    for (s32 i = 1; i < 14; i++)
    {
        s32 deg = i * 30;

        m_bg_prim.dataPos(i).set(fsMath::cos_s32(deg) * 300.0f, 0.0f, fsMath::sin_s32(deg) * 300.0f);
        m_bg_prim.dataCol(i).set(0, 0, 0);
    }
}


void ScenarioPlayer::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    m_rot_y += 0.3f;
    m_scr->view() = fsMat::UNIT.rotateY_r32(m_rot_y).rotateX_s32(-15).translate(0.0f, 70.0f, 700.0f);

    if (m_wait_cntr > 0)
    {
        m_wait_cntr--;
    }
    else
    {
        while (m_ent)
        {
            u8 cmd = m_ent->getValue_s32(0);
            u16 val_num = m_ent->getValueNum();

            if (cmd == 1 && val_num == 6) // new_coral(r32 x, r32 z, s32 type, s32 branch_num, s32 survival_time)
            {
                extern void newCoral(r32 x, r32 z, u8 type, u8 branch_num, u16 survival_time);
                newCoral(m_ent->getValue_r32(1), m_ent->getValue_r32(2), m_ent->getValue_s32(3), m_ent->getValue_s32(4), m_ent->getValue_s32(5));

                m_ent = m_conf->getEntryAfterN("command", m_ent);
            }
            else if (cmd == 2 && val_num == 2) // wait(s32 frame_num)
            {
                m_wait_cntr = m_ent->getValue_s32(1);

                m_ent = m_conf->getEntryAfterN("command", m_ent);
                break;
            }
            else if (cmd == 3 && val_num == 1) // repeat
            {
                m_ent = m_conf->getEntryFromFirstN("command");
            }
        }
    }
}
