/*
 * golem.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs_main.h"


class Golem : public fsTask
{
public:
    Golem();

private:
    virtual void onUpdate();

    fsMdl m_golem_mdl;
    fsSprt m_copyright_sprt;

    r32 m_cur_mouse_x;
    r32 m_cur_mouse_y;
    r32 m_last_mouse_x;
    r32 m_last_mouse_y;

    r32 m_rot_x;
    r32 m_rot_y;
};


void newGolem()
{
    fsNewTask(Golem);
}


Golem::Golem() : fsTask(ORDER_ZERO)
{
    m_golem_mdl.init(fsID_("golem.3ds"), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_golem_mdl.setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_golem_mdl.getRootDraw()->local() = fsMat::UNIT.rotateX_s32(-90);

    m_copyright_sprt.init(1, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_copyright_sprt.setTextureID(fsID_("copyright_304x34.png"));
    m_copyright_sprt.setBlendMode(fsDraw::BLEND_HALF, true);

    m_copyright_sprt.dataPos(0).set(0.0f, -150.0f);
    m_copyright_sprt.dataCol(0).set(128, 255, 128, 160);
    m_copyright_sprt.setDataSize(0, 304, 34);

    m_rot_y = 0.0f;
    m_rot_x = -20.0f;

    m_cur_mouse_x = m_last_mouse_x = 0.0f;
    m_cur_mouse_y = m_last_mouse_y = 0.0f;

    fsLts* lts = fsDrawMgr::getLightSet(fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    lts->setAmbientColor(fsCol(128, 128, 128));
    lts->setParaLightActive(0, true);
    lts->setParaLightDir(0, fsVec(1.0f, -1.0f, -1.0f));
    lts->setParaLightColor(0, fsCol::FULL);
}


void Golem::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_T))
    {
        m_golem_mdl.setTextureID((m_golem_mdl.getTextureID() == fsID::ZERO) ? fsID_("golem.png") : fsID::ZERO);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_L))
    {
        m_golem_mdl.setLightSetID((m_golem_mdl.getLightSetID() == fsID::ZERO) ? fsDrawMgr::DEFAULT_LIGHT_SET_ID : fsID::ZERO);
    }

    m_last_mouse_x = m_cur_mouse_x;
    m_last_mouse_y = m_cur_mouse_y;

    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    if (fsInputMgr::isPressed(fsInputMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = m_last_mouse_x = scr->framebufferXToScreenX(fsInputMgr::getMouseX());
        m_cur_mouse_y = m_last_mouse_y = scr->framebufferYToScreenY(fsInputMgr::getMouseY());
    }

    if (fsInputMgr::isOn(fsInputMgr::KEY_LBUTTON))
    {
        m_cur_mouse_x = scr->framebufferXToScreenX(fsInputMgr::getMouseX());
        m_cur_mouse_y = scr->framebufferYToScreenY(fsInputMgr::getMouseY());
    }

    m_rot_y += 0.1f - (m_cur_mouse_x - m_last_mouse_x) * 0.5f;
    m_rot_x += (m_cur_mouse_y - m_last_mouse_y) * 0.3f;
    m_rot_x = fsMath::clamp(m_rot_x, -80.0f, 80.0f);

    scr->view() = fsMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 150.0f, 1000.0f + m_rot_x * m_rot_x * 0.1f);

    for (s32 i = -10; i <= 10; i++)
    {
        r32 offset = i * 50.0f;

        fsDebugMgr::drawLine(fsVec(offset, 0.0f, 500.0f), fsVec(offset, 0.0f, -500.0f), fsCol(0, 255, 0, 128), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
        fsDebugMgr::drawLine(fsVec(500.0f, 0.0f, offset), fsVec(-500.0f, 0.0f, offset), fsCol(0, 255, 0, 128), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    }
}
