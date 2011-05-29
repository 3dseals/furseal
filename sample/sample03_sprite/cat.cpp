/*
 * cat.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Cat : public fsTask
{
public:
    Cat();

private:
    static const u32 CAT_NUM = 12;

    virtual void onUpdate();

    fsNode m_cat_root;
    fsSprt m_cat_sprt[CAT_NUM];
};


void newCat()
{
    fsNewTask(Cat);
}


Cat::Cat() : fsTask(ORDER_ZERO)
{
    m_cat_root.init(fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    for (u32 i = 0; i < CAT_NUM; i++)
    {
        fsSprt* sprt = &m_cat_sprt[i];

        sprt->init(1, &m_cat_root);
        sprt->setTextureID(fsID_("stephanie_275x196.png"));
        sprt->setBlendMode(fsDraw::BLEND_HALF, true);

        sprt->setDataSize(0, 275.0f * 0.5f, 196.0f * 0.5f);
        sprt->dataAng(0) = fsMath::rand(-30, 30);

        sprt->local().trans.set( //
            fsMath::rand(-200.0f, 200.0f, 20.0f), //
            fsMath::rand(-200.0f, 200.0f, 20.0f), //
            fsMath::rand(-200.0f, 200.0f, 20.0f));
    }
}


void Cat::onUpdate()
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

    m_cat_root.local() = m_cat_root.local().rotateY_r32(0.2f);

    if (fsInputMgr::isPressed(fsInputMgr::KEY_A))
    {
        if (m_cat_root.getColor().a == 255)
        {
            m_cat_root.setColor(fsCol(255, 255, 255, 96));
        }
        else
        {
            m_cat_root.setColor(fsCol::FULL);
        }
    }
}
