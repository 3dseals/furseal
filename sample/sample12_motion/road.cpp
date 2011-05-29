/*
 * road.cpp
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

    fsPrim m_road_prim;
    fsPrim m_line_prim;
};


void newStage()
{
    fsNewTask(Stage);
}


Stage::Stage() : fsTask(ORDER_ZERO)
{
    m_rot_y = -120.0f;
    m_rot_x = -10.0f;

    fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID)->setClearColor(fsCol(96, 32, 0));

    fsLts* lts = fsDrawMgr::getLightSet(fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    lts->setAmbientColor(fsCol(160, 128, 128));
    lts->setParaLightActive(0, true);
    lts->setParaLightDir(0, fsVec(1.0f, -1.0f, -1.0f));
    lts->setParaLightColor(0, fsCol(255, 128, 96));

    m_road_prim.init(fsPrim::MODE_TRIANGLE_STRIP, 6, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_road_prim.setBlendMode(fsDraw::BLEND_HALF, true);
    m_road_prim.setSortOffset(-1000.0f);

    m_road_prim.dataPos(0).set(200.0f, 0.0f, -20000.0f);
    m_road_prim.dataPos(1).set(-200.0f, 0.0f, -20000.0f);
    m_road_prim.dataPos(2).set(200.0f, 0.0f, 0.0f);
    m_road_prim.dataPos(3).set(-200.0f, 0.0f, 0.0f);
    m_road_prim.dataPos(4).set(200.0f, 0.0f, 20000.0f);
    m_road_prim.dataPos(5).set(-200.0f, 0.0f, 20000.0f);

    for (s32 i = 0; i < 6; i++)
    {
        m_road_prim.dataCol(i).set(64, 64, 80, (i >= 2 && i <= 3) ? 255 : 0);
    }

    m_line_prim.init(fsPrim::MODE_TRIANGLES, 6 * 100, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_line_prim.setBlendMode(fsDraw::BLEND_HALF, true);

    for (s32 i = 0; i < 100; i++)
    {
        s32 offset = i - 50;

        m_line_prim.dataPos(i * 6 + 0).set(15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 1).set(-15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 2).set(15.0f, 0.0f, 100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 4).set(-15.0f, 0.0f, -100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 3).set(-15.0f, 0.0f, 100.0f + offset * 400.0f);
        m_line_prim.dataPos(i * 6 + 5).set(15.0f, 0.0f, 100.0f + offset * 400.0f);

        for (s32 j = 0; j < 6; j++)
        {
            m_line_prim.dataCol(i * 6 + j).set(255, 255, 255, 100 - fsMath::abs(offset) * 2);
        }
    }
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

    m_line_prim.local().trans.z += 0.3f;

    if (m_line_prim.local().trans.z >= 400.0f)
    {
        m_line_prim.local().trans.z -= 400.0f;
    }

    m_rot_y -= 0.1f;

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

    m_rot_x = fsMath::clamp(m_rot_x, -80.0f, -5.0f);

    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    scr->view() = fsMat::UNIT.rotateY_r32(m_rot_y).rotateX_r32(m_rot_x).translate(0.0f, 30.0f, 650.0f - m_rot_x * 2.0f);
}
