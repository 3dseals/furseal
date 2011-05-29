/*
 * triangle.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Triangle : public fsTask
{
public:
    Triangle();

private:
    virtual void onUpdate();

    fsScr* m_scr2d;
    fsPrim m_tri_prim1;
    fsPrim m_tri_prim2;
};


void newTriangle()
{
    fsNewTask(Triangle);
}


Triangle::Triangle() : fsTask(ORDER_ZERO)
{
    m_scr2d = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_2D_SCREEN_ID);

    /*
        set up m_tri_prim1
    */
    m_tri_prim1.init(fsPrim::MODE_TRIANGLES, 3, fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_tri_prim1.dataPos(0).set(0.0f, 100.0f, 0.0f);
    m_tri_prim1.dataPos(1).set(-110.0f, -100.0f, 0.0f);
    m_tri_prim1.dataPos(2).set(110.0f, -100.0f, 0.0f);

    m_tri_prim1.dataCol(0).set(255, 0, 0);
    m_tri_prim1.dataCol(1).set(0, 255, 0);
    m_tri_prim1.dataCol(2).set(0, 0, 255);

    m_tri_prim1.local().trans.set(-150.0f, 0.0f, 0.0f);

    /*
        set up m_tri_prim2
    */
    m_tri_prim2.init(fsPrim::MODE_TRIANGLES, 6, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_tri_prim2.setTextureID(fsID_("coin_400x200.png"));
    m_tri_prim2.setDrawFlag(fsDraw::FLAG_BACKFACE_CULLING, true);

    m_tri_prim2.dataPos(0).set(0.0f, 100.0f, 0.0f);
    m_tri_prim2.dataPos(1).set(-110.0f, -100.0f, 0.0f);
    m_tri_prim2.dataPos(2).set(110.0f, -100.0f, 0.0f);

    m_tri_prim2.dataPos(3).set(0.0f, 100.0f, 0.0f);
    m_tri_prim2.dataPos(4).set(110.0f, -100.0f, 0.0f);
    m_tri_prim2.dataPos(5).set(-110.0f, -100.0f, 0.0f);

    m_tri_prim2.setDataUV(0, 0.25f, 0.0f);
    m_tri_prim2.setDataUV(1, 0.035f, 0.75f);
    m_tri_prim2.setDataUV(2, 0.465f, 0.75f);

    m_tri_prim2.setDataUV(3, 0.75f, 0.0f);
    m_tri_prim2.setDataUV(4, 0.535f, 0.75f);
    m_tri_prim2.setDataUV(5, 0.965f, 0.75f);

    m_tri_prim2.local().trans.set(150.0f, 0.0f, 0.0f);
}


void Triangle::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    fsDebugMgr::drawBox(fsMat::UNIT, fsVec(400.0f, 400.0f, 400.0f), fsCol::ZERO, fsCol(128, 128, 128), fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_scr2d->view() = m_scr2d->view().rotateZ_r32(-0.1f);

    m_tri_prim1.local() = m_tri_prim1.local().rotateY_s32(2);
    m_tri_prim2.local() = m_tri_prim2.local().rotateY_s32(2);
}
