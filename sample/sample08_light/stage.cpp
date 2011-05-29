/*
 * stage.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Stage : public fsTask
{
public:
    Stage();

private:
    virtual void onUpdate();

    r32 m_rot_x;
    r32 m_rot_y;
    fsLts* m_lts1;
    fsLts* m_lts2;
    fsLts* m_cur_lts;
};


void newStage()
{
    fsNewTask(Stage);
}


Stage::Stage() : fsTask(ORDER_ZERO)
{
    m_rot_y = 0.0f;
    m_rot_x = -30.0f;

    /*
        setup lightset1
    */
    m_lts1 = fsDrawMgr::getLightSet(fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    m_lts1->setAmbientColor(fsCol(64, 32, 64));

    m_lts1->setParaLightActive(0, true);
    m_lts1->setParaLightDir(0, fsVec(1.0f, -1.0f, -1.0f));
    m_lts1->setParaLightColor(0, fsCol(64, 64, 64));

    fsLit* lit1 = m_lts1->newPointLight(fsID::genID());
    lit1->setPos(fsVec(200.0f, 50.0f, 200.0f));
    lit1->setRadius(50.0f, 400.0f);
    lit1->setColor(fsCol(0, 255, 255));

    fsLit* lit2 = m_lts1->newPointLight(fsID::genID());
    lit2->setPos(fsVec(-200.0f, -50.0f, -200.0f));
    lit2->setRadius(50.0f, 400.0f);
    lit2->setColor(fsCol(255, 0, 0));

    /*
        setup lightset2
    */
    m_lts2 = fsDrawMgr::newLightSet(fsID::genID());

    m_lts2->setAmbientColor(fsCol::ZERO);

    m_lts2->setParaLightActive(0, true);
    m_lts2->setParaLightDir(0, fsVec(-1.0f, -1.0f, 0.0f));
    m_lts2->setParaLightColor(0, fsCol(255, 0, 0));

    m_lts2->setParaLightActive(1, true);
    m_lts2->setParaLightDir(1, fsVec(0.0f, -1.0f, -1.0f));
    m_lts2->setParaLightColor(1, fsCol(0, 255, 0));

    m_lts2->setParaLightActive(2, true);
    m_lts2->setParaLightDir(2, fsVec(-1.0f, 0.0f, -1.0f));
    m_lts2->setParaLightColor(2, fsCol(0, 0, 255));

    m_cur_lts = m_lts1;
}


void Stage::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    fsVec pos1(200.0f, 0.0f, 200.0f);
    fsVec pos2(-200.0f, 0.0f, 200.0f);
    fsVec pos3(-200.0f, 0.0f, -200.0f);
    fsVec pos4(200.0f, 0.0f, -200.0f);
    fsCol col(128, 128, 128);

    fsDebugMgr::drawLine(pos1, pos2, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(pos2, pos3, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(pos3, pos4, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(pos4, pos1, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_rot_y += 0.1f;

    if (fsInputMgr::isOn(fsInputMgr::KEY_RIGHT))
    {
        m_rot_y += 1.0f;
    }

    if (fsInputMgr::isOn(fsInputMgr::KEY_LEFT))
    {
        m_rot_y -= 1.0f;
    }

    if (fsInputMgr::isOn(fsInputMgr::KEY_UP))
    {
        m_rot_x -= 1.0f;
    }

    if (fsInputMgr::isOn(fsInputMgr::KEY_DOWN))
    {
        m_rot_x += 1.0f;
    }

    m_rot_x = fsMath::clamp(m_rot_x, -80.0f, -20.0f);

    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    scr->view() = fsMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 0.0f, 650.0f + m_rot_x);

    if (fsInputMgr::isPressed(fsInputMgr::KEY_L))
    {
        fsMsg<4> msg;

        m_cur_lts = (m_cur_lts == m_lts1) ? m_lts2 : m_lts1;

        msg.setParam(0, m_cur_lts->getID());

        fsTaskMgr::sendMessage(fsID_("CHANGE LIGHT SET"), msg);
    }
}
