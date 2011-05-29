/*
 * debug_draw.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"


class DebugDraw : public fsTask
{
public:
    DebugDraw();

private:
    virtual void onUpdate();

    fsMat m_box_world;
    fsVec m_box_size;

    fsMat m_sphere_world;
    r32 m_sphere_radius;
};


void newDebugDraw()
{
    fsNewTask(DebugDraw);
}


DebugDraw::DebugDraw() : fsTask(ORDER_ZERO)
{
    m_box_world = fsMat::UNIT;
    m_box_world.trans.set(-130.0f, 100.0f, 0.0f);
    m_box_size.set(150.0f, 100.0f, 50.0f);

    m_sphere_world = fsMat::UNIT;
    m_sphere_world.trans.set(130.0f, 100.0f, 0.0);
    m_sphere_radius = 80.0f;
}


void DebugDraw::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_V))
    {
        fsDebugMgr::dumpVector(m_box_size, "m_box_size");
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_M))
    {
        fsDebugMgr::dumpMatrix(m_sphere_world, "m_sphere_world");
    }

    m_box_world = m_box_world.rotateY_s32(2).rotateX_r32(1);
    m_sphere_world = m_sphere_world.rotateX_s32(2).rotateY_s32(1);

    fsDebugMgr::drawBox(m_box_world, m_box_size, fsCol(0, 128, 255, 128), fsCol(255, 255, 255, 128), fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawSphere(m_sphere_world.trans, m_sphere_radius, fsCol(255, 0, 128, 128), fsCol(255, 255, 255, 128), fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    fsDebugMgr::drawAxis(m_box_world, 100.0f, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawAxis(m_sphere_world, 100.0f, fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    fsDebugMgr::drawLine(fsVec(-250.0f, -50.0f), fsVec(-150.0f, -150.0f), fsCol(255, 255, 0), fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    fsDebugMgr::drawPolygon(fsVec(0.0f, -50.0f), fsVec(-50.0f, -150.0f), fsVec(50.0f, -150.0f), fsCol(255, 0, 255), fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    fsDebugMgr::drawPolygon(fsVec(150.0f, -50.0f), fsVec(150.0f, -150.0f), fsVec(250.0f, -50.0f), fsVec(250.0f, -150.0f), fsCol(0, 192, 0), fsDrawMgr::DEFAULT_2D_SCREEN_ID);

    u32 frame_count = fsTaskMgr::getFrameCount();

    if (frame_count % 10 == 0)
    {
        fsDebugMgr::trace("%d\n", frame_count);
    }
}
