/*
 * coin.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Coin : public fsTask
{
public:
    Coin();

private:
    virtual void onUpdate();

    fsSprt m_coin_sprt;
};


void newCoin()
{
    fsNewTask(Coin);
}


Coin::Coin() : fsTask(ORDER_ZERO)
{
    m_coin_sprt.init(6, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_coin_sprt.setTextureID(fsID_("coin_400x200.png"));
    m_coin_sprt.setBlendMode(fsDraw::BLEND_HALF, true);

    m_coin_sprt.dataPos(0).set(-200.0f, 150.0f);
    m_coin_sprt.dataPos(1).set(-200.0f, 0.0f);
    m_coin_sprt.dataPos(2).set(-200.0f, -150.0f);
    m_coin_sprt.dataPos(3).set(200.0f, 150.0f);
    m_coin_sprt.dataPos(4).set(200.0f, 0.0f);
    m_coin_sprt.dataPos(5).set(200.0f, -150.0f);

    for (s32 i = 0; i < 6; i++)
    {
        m_coin_sprt.setDataSize(i, 96.0f, 96.0f);

        if (i % 2 == 0)
        {
            m_coin_sprt.setDataUV(i, 0.0f, 0.0f, 0.5f, 1.0f);
        }
        else
        {
            m_coin_sprt.setDataUV(i, 0.5f, 0.0f, 1.0f, 1.0f);
        }
    }
}


void Coin::onUpdate()
{
    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    r32 mouse_x = scr->framebufferXToScreenX(fsInputMgr::getMouseX());
    r32 mouse_y = scr->framebufferYToScreenY(fsInputMgr::getMouseY());

    for (s32 i = 0; i < 6; i++)
    {
        m_coin_sprt.dataAng(i) += (i % 2 == 0) ? -1 : 1;

        r32 diff_x = m_coin_sprt.dataPos(i).x - mouse_x;
        r32 diff_y = m_coin_sprt.dataPos(i).y - mouse_y;

        r32 aim_size = (diff_x * diff_x + diff_y * diff_y <= 48.0f * 48.0f) ? 192.0f : 96.0f;

        m_coin_sprt.dataW(i) = m_coin_sprt.dataH(i) = fsMath::interp(m_coin_sprt.dataW(i), aim_size, 0.1f);
    }
}
