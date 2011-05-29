/*
 * copy_cat.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class CopyCat : public fsTask
{
public:
    CopyCat();

private:
    static const u32 COPY_CAT_NUM = 32;

    virtual void onUpdate();

    bool m_is_copy_texture;
    fsScr* m_scr2d;
    fsSprt m_copy_sprt;
};


void newCopyCat()
{
    fsNewTask(CopyCat);
}


CopyCat::CopyCat() : fsTask(ORDER_ZERO)
{
    m_is_copy_texture = true;

    m_scr2d = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_scr2d->attachScreenTexture(fsTex::FORMAT_RGBA);

    m_copy_sprt.init(COPY_CAT_NUM, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_copy_sprt.setTextureID(m_scr2d->getScreenTextureID());
    m_copy_sprt.setBlendMode(fsDraw::BLEND_HALF, true);

    for (u32 i = 0; i < COPY_CAT_NUM; i++)
    {
        m_copy_sprt.dataPos(i).set(fsMath::rand(-320.0f, 320.0f, 10.0f), fsMath::rand(-240.0f, 240.0f, 10.0f), fsMath::rand(-300.0f, 300.0f, 10.0f));
        m_copy_sprt.dataAng(i) = fsMath::rand(-90, 90);
        m_copy_sprt.setDataSize(i, 120.0f, 90.0f);
    }
}


void CopyCat::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);

        m_is_copy_texture = true;
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_C))
    {
        m_is_copy_texture = !m_is_copy_texture;
    }

    if (m_is_copy_texture)
    {
        m_scr2d->updateScreenTexture(false);
    }
}
