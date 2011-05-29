/*
 * font_draw.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class FontDraw : public fsTask
{
public:
    FontDraw();

private:
    virtual void onUpdate();

    u64 m_start_time;
    fsScr* m_scr3d;
    fsMat m_view1;
    fsMat m_view2;
    bool m_is_view2;

    fsFont m_text_font1;
    fsSprt m_text_sprt1;

    fsFont m_text_font2;
    fsSprt m_text_sprt2;

    fsFont m_time_font;
    fsSprt m_time_sprt;
};


void newFontDraw()
{
    fsNewTask(FontDraw);
}


FontDraw::FontDraw() : fsTask(ORDER_ZERO)
{
    m_start_time = fsMgr::getUsecTime();
    m_scr3d = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_view1 = m_scr3d->view();
    m_view2 = m_view1.rotateY_s32(90).rotateX_s32(-30).translate(800.0f, 10.0f, 1500.0f);
    m_is_view2 = false;

    /*
        set up m_text_font1 and m_text_sprt1
    */
    const char* text1 = "Furseal Font-drawing Sample";

    fsDrawMgr::setFont(fsID_("stonsans.ttf"), 0);
    fsDrawMgr::setFontSize(48);

    m_text_font1.init(fsDrawMgr::calcFontDrawWidth(text1), fsDrawMgr::getFontSize());
    m_text_font1.drawString(0, 0, text1);

    m_text_sprt1.init(2, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt1.setTextureID(m_text_font1.getTextureID());
    m_text_sprt1.setBlendMode(fsDraw::BLEND_HALF, true);

    m_text_sprt1.dataPos(0).set(1.0f, 99.0f, 0.0f);
    m_text_sprt1.setDataSize(0, m_text_font1.getWidth(), m_text_font1.getHeight());

    m_text_sprt1.dataPos(1).set(0.0f, 100.0f, 0.0f);
    m_text_sprt1.dataCol(1).set(255, 64, 128);
    m_text_sprt1.setDataSize(1, m_text_font1.getWidth(), m_text_font1.getHeight());

    /*
        set up m_text_font2 and m_text_sprt2
    */
    wchar_t text2[64];
    fsMgr::swprintf(text2, sizeof(text2) / sizeof(wchar_t), L"You can use %ls and %ls.", L"ASCII", L"UNICODE");

#ifdef WIN32
    fsDrawMgr::setFont(fsID_("msgothic.ttc"), 0);
#endif
    fsDrawMgr::setFontSize(36);

    m_text_font2.init(fsDrawMgr::calcFontDrawWidth(text2), fsDrawMgr::getFontSize());
    m_text_font2.drawString(0, 0, text2);

    m_text_sprt2.init(2, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_text_sprt2.setTextureID(m_text_font2.getTextureID());
    m_text_sprt2.setBlendMode(fsDraw::BLEND_HALF, true);

    m_text_sprt2.dataPos(0).set(1.0f, 29.0f, 0.0f);
    m_text_sprt2.dataCol(0).set(192, 192, 255);
    m_text_sprt2.setDataSize(0, m_text_font2.getWidth(), m_text_font2.getHeight());

    m_text_sprt2.dataPos(1).set(0.0f, 30.0f, 0.0f);
    m_text_sprt2.dataCol(1).set(64, 128, 255);
    m_text_sprt2.setDataSize(1, m_text_font2.getWidth(), m_text_font2.getHeight());

    /*
        set up m_time_font and m_time_sprt
    */
    m_time_font.init(256, 100);

    m_time_sprt.init(2, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_time_sprt.setTextureID(m_time_font.getTextureID());
    m_time_sprt.setBlendMode(fsDraw::BLEND_HALF, true);

    m_time_sprt.setDataSize(0, 256, 100);

    m_time_sprt.dataPos(1).set(0.0f, -45.0f);
    m_time_sprt.dataCol(1).set(128, 128, 128, 128);
    m_time_sprt.setDataSize(1, 256, 80);
    m_time_sprt.setDataUV(1, 0.0f, 1.0f, 1.0f, 0.0f);

    m_time_sprt.local().trans.set(20.0f, -80.0f);
}


void FontDraw::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    r32 time = static_cast<r32>(fsMgr::getUsecTime() - m_start_time) / 1000000.0f;
    time -= (static_cast<u32>(time) / 100) * 100;

    fsDrawMgr::setFont(fsID_("stonsans.ttf"), 0);
    fsDrawMgr::setFontSize(100);

    m_time_font.clearImage();
    m_time_font.drawString(0, 0, "%2.2f", time);

    if (fsInputMgr::isPressed(fsInputMgr::KEY_V))
    {
        m_is_view2 = !m_is_view2;
    }

    if (m_is_view2)
    {
        m_scr3d->view() = m_scr3d->view().slerp(m_view2, 0.05f);
    }
    else
    {
        m_scr3d->view() = m_scr3d->view().slerp(m_view1, 0.05f);
    }
}
