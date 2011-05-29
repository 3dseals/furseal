/*
 * fog.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class Fog : public fsTask
{
public:
    Fog();

private:
    static const u32 FOG_NUM = 16;

    virtual void onUpdate();
    virtual void onMessage(fsID msg_id, fsMsg<4>& msg);

    fsLts* m_cur_lts;
    fsSprt m_fog_sprt[FOG_NUM];
};


void newFog()
{
    fsNewTask(Fog);
}


Fog::Fog() : fsTask(ORDER_ZERO)
{
    m_cur_lts = fsDrawMgr::getLightSet(fsDrawMgr::DEFAULT_LIGHT_SET_ID);

    for (u32 i = 0; i < FOG_NUM; i++)
    {
        fsSprt* fog_sprt = &m_fog_sprt[i];

        fog_sprt->init(1, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
        fog_sprt->setTextureID(fsID_("fog_128x128.png"));

        fog_sprt->setBlendMode(fsDraw::BLEND_HALF, true);

        fog_sprt->dataPos(0).set(fsMath::rand(-180.0f, 180.0f, 1.0f), fsMath::rand(0.0f, 2.0f, 0.1f), fsMath::rand(-180.0f, 180.0f, 1.0f));
        fog_sprt->setDataSize(0, 128.0f, 128.0f);
        fog_sprt->dataAng(0) = fsMath::rand(-180, 180);
    }
}


void Fog::onUpdate()
{
    for (u32 i = 0; i < FOG_NUM; i++)
    {
        fsSprt* fog_sprt = &m_fog_sprt[i];

        m_cur_lts->findNearLight(fog_sprt->dataPos(0));

        fog_sprt->dataCol(0) = m_cur_lts->getAmbientColor();

        for (s32 j = 0; j < m_cur_lts->getNearLightNum(); j++)
        {
            fog_sprt->dataCol(0) += m_cur_lts->getNearLightColor(j);
        }

        fog_sprt->dataCol(0).a = 32;

        fog_sprt->dataAng(0)++;
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_B))
    {
        if (m_fog_sprt->getBlendMode() == fsDraw::BLEND_HALF)
        {
            for (u32 i = 0; i < FOG_NUM; i++)
            {
                m_fog_sprt[i].setBlendMode(fsDraw::BLEND_ADD, true);
            }
        }
        else
        {
            for (u32 i = 0; i < FOG_NUM; i++)
            {
                m_fog_sprt[i].setBlendMode(fsDraw::BLEND_HALF, true);
            }
        }
    }
}


void Fog::onMessage(fsID msg_id, fsMsg<4>& msg)
{
    if (msg_id == fsID_("CHANGE LIGHT SET"))
    {
        m_cur_lts = fsDrawMgr::getLightSet(msg.getParam<fsID>(0));
    }
}
