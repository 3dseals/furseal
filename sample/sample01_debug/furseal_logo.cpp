/*
 * furseal_log.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class FursealLogo : public fsTask
{
public:
    FursealLogo(const char** cmd, u8 cmd_num);
    virtual ~FursealLogo();

private:
    void setPos();
    virtual void onUpdate();

    const char** m_cmd;
    u8 m_cmd_num;
    fsScr* m_scr;
    fsSprt m_logo_sprt;
};


void newFursealLogo(const char** cmd, u8 cmd_num)
{
    fsNewTask(FursealLogo)(cmd, cmd_num);
}


FursealLogo::FursealLogo(const char** cmd, u8 cmd_num) : fsTask(ORDER_PLUS_7)
{
    m_cmd = cmd;
    m_cmd_num = cmd_num;

    m_scr = fsDrawMgr::newScreen(fsID::genID());
    m_scr->setClearMode(false, true);
    m_scr->setPerspective(false);
    m_scr->moveLast();

    m_logo_sprt.init(1, m_scr->getID());
    m_logo_sprt.setTextureID(fsID_("fs_logo_71x14.png"));
    m_logo_sprt.setDrawFlag(fsDraw::FLAG_BILINEAR, false);

    m_logo_sprt.setDataSize(0, 71.0f * 3.0f, 14.0f * 3.0f);

    setPos();
}


FursealLogo::~FursealLogo()
{
    fsDrawMgr::deleteScreen(m_scr->getID());
}


void FursealLogo::setPos()
{
    r32 x = (fsMgr::getFramebufferWidth() - m_logo_sprt.dataW(0)) / 2.0f - 16.0f;
    r32 y = (fsMgr::getFramebufferHeight() - m_logo_sprt.dataH(0)) / -2.0f + 16.0f;

    m_logo_sprt.dataPos(0).set(x, y);
}


void FursealLogo::onUpdate()
{
    if (fsMgr::isFramebufferSizeChanged())
    {
        m_scr->setAreaInFramebuffer(0, 0, fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
        m_scr->setViewSize(fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());

        setPos();
    }

    const r32 left = fsMgr::getFramebufferWidth() / -2.0f + 16.0f;
    const r32 top = fsMgr::getFramebufferHeight() / 2.0f - 16.0f;
    const r32 interval = 12.0f;

    for (s32 i = 0; i < m_cmd_num; i++)
    {
        fsDebugMgr::drawString(left, top - interval * i, fsCol::FULL, 1, m_cmd[i]);
    }
}
