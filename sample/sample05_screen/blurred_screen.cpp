/*
 * blurred_screen.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class BlurredScreen : public fsTask
{
public:
    BlurredScreen();
    virtual ~BlurredScreen();

private:
    virtual void onUpdate();

    fsScr* m_blurred_scr;

    fsPrim m_tetra_prim;
    fsPrim m_blur_prim;

    fsNode m_cat_node;
    fsPrim m_mask_prim;
    fsPrim m_scr_prim;
};


void newBlurredScreen()
{
    fsNewTask(BlurredScreen);
}


BlurredScreen::BlurredScreen() : fsTask(ORDER_ZERO)
{
    /*
        set up m_blurred_scr
    */
    m_blurred_scr = fsDrawMgr::newScreen(fsID_("blurred_screen"));
    m_blurred_scr->moveFirst();
    m_blurred_scr->setAreaInFramebuffer(0, 0, 512, 300);
    m_blurred_scr->setViewSize(512.0f, 300.0f);
    m_blurred_scr->setClearColor(fsCol(32, 32, 32));
    m_blurred_scr->attachScreenTexture(fsTex::FORMAT_RGBA); // fsTex::FORMAT_RGB is also OK except on iPhone

    /*
        set up m_tetra_prim
    */
    m_tetra_prim.init(fsPrim::MODE_TRIANGLES, 12, m_blurred_scr->getID());

    fsVec v1(0.0f, 61.5f, 0.0f), v2(58.0f, -24.0f, 0.0f), v3(-29.0f, -24.0f, 50.0f), v4(-29.0f, -24.0f, -50.0f);

    m_tetra_prim.dataPos(0) = v1;
    m_tetra_prim.dataPos(1) = v2;
    m_tetra_prim.dataPos(2) = v3;
    m_tetra_prim.dataPos(3) = v1;
    m_tetra_prim.dataPos(4) = v3;
    m_tetra_prim.dataPos(5) = v4;
    m_tetra_prim.dataPos(6) = v1;
    m_tetra_prim.dataPos(7) = v4;
    m_tetra_prim.dataPos(8) = v2;
    m_tetra_prim.dataPos(9) = v2;
    m_tetra_prim.dataPos(10) = v4;
    m_tetra_prim.dataPos(11) = v3;

    m_tetra_prim.dataCol(0) = m_tetra_prim.dataCol(1) = m_tetra_prim.dataCol(2) = fsCol(255, 64, 64);
    m_tetra_prim.dataCol(3) = m_tetra_prim.dataCol(4) = m_tetra_prim.dataCol(5) = fsCol(64, 255, 64);
    m_tetra_prim.dataCol(6) = m_tetra_prim.dataCol(7) = m_tetra_prim.dataCol(8) = fsCol(64, 64, 255);
    m_tetra_prim.dataCol(9) = m_tetra_prim.dataCol(10) = m_tetra_prim.dataCol(11) = fsCol(255, 255, 64);

    m_tetra_prim.local().trans.set(-50.0f, -10.0f, 0.0f);

    /*
        set up m_blur_prim
    */
    m_blur_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, m_blurred_scr->getID());
    m_blur_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_blur_prim.setBlendMode(fsDraw::BLEND_HALF, true);
    m_blur_prim.setSortOffset(10000.0f);
    m_blur_prim.setDepthTest(fsDraw::DEPTH_TEST_ALWAYS);
    m_blur_prim.setDataRect(0, fsVec::ZERO, 512.0f, 300.0f, fsCol(255, 255, 255, 232), 0.033f, 0.06f, 0.95f, 0.95f);

    /*
        set up m_cat_node, m_mask_prim and m_scr_prim
    */
    m_cat_node.init(fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_mask_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_mask_prim.setTextureID(fsID_("mask_512x300.png"));
    m_mask_prim.setBlendMode(fsDraw::BLEND_HALF, false);
    m_mask_prim.setDrawFlag(fsDraw::FLAG_WRITE_RGB, false);
    m_mask_prim.setDrawFlag(fsDraw::FLAG_WRITE_ALPHA, true);
    m_mask_prim.setDataRect(0, fsVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, fsCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);

    m_scr_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, &m_cat_node);
    m_scr_prim.setTextureID(m_blurred_scr->getScreenTextureID());
    m_scr_prim.setDepthTest(fsDraw::DEPTH_TEST_ALWAYS);
    m_scr_prim.setBlendMode(fsDraw::BLEND_DEST_ALPHA, false);
    m_scr_prim.setDrawFlag(fsDraw::FLAG_WRITE_ALPHA, true);
    m_scr_prim.setDataRect(0, fsVec(50.0f, 20.0f, 0.0f), 512.0f, 300.0f, fsCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
}


BlurredScreen::~BlurredScreen()
{
    fsDrawMgr::deleteScreen(fsID_("blurred_screen"));
}


void BlurredScreen::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    m_blurred_scr->updateScreenTexture(false);

    m_tetra_prim.local() = m_tetra_prim.local().rotateX_r32(2.0f).rotateY_r32(1.5f);
    m_cat_node.local() = m_cat_node.local().rotateY_r32(0.5f);

    if (fsInputMgr::isPressed(fsInputMgr::KEY_B))
    {
        m_blur_prim.setVisible(!m_blur_prim.isVisible());
    }
}
