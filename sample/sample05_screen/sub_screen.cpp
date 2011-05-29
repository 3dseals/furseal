/*
 * sub_screen.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class SubScreen : public fsTask
{
public:
    SubScreen();
    virtual ~SubScreen();

private:
    virtual void onUpdate();

    fsScr* m_sub_scr1;
    fsScr* m_sub_scr2;
};


void newSubScreen()
{
    fsNewTask(SubScreen);
}


SubScreen::SubScreen() : fsTask(ORDER_ZERO)
{
    m_sub_scr1 = fsDrawMgr::newScreen(fsID::genID());
    m_sub_scr1->setClearColor(fsCol(0, 128, 0, 0));
    m_sub_scr1->setGuestScreenID(0, fsID_("blurred_screen"));
    m_sub_scr1->setAreaInFramebuffer(466, 16, 160, 120);
    m_sub_scr1->view() = fsMat::lookAt(fsVec(300.0f, 0.0f, 0.0f), fsVec::ZERO, fsVec::Y_UNIT);

    m_sub_scr2 = fsDrawMgr::newScreen(fsID::genID());
    m_sub_scr2->setClearColor(fsCol(128, 0, 0, 128));
    m_sub_scr2->setGuestScreenID(0, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_sub_scr2->setGuestScreenID(1, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_sub_scr2->setAreaInFramebuffer(16, 344, 160, 120);
    m_sub_scr2->view() = fsMat::lookAt(fsVec(0.0f, 1000.0f, 400.0f), fsVec::ZERO, fsVec::Y_UNIT);
}


SubScreen::~SubScreen()
{
    fsDrawMgr::deleteScreen(m_sub_scr1->getID());
    fsDrawMgr::deleteScreen(m_sub_scr2->getID());
}


void SubScreen::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_S))
    {
        m_sub_scr1->setActive(!m_sub_scr1->isActive());
        m_sub_scr2->setActive(!m_sub_scr2->isActive());
    }
}
