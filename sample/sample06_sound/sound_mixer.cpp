/*
 * sound_mixer.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class SoundMixer : public fsTask
{
public:
    SoundMixer();

private:
    virtual void onUpdate();

    bool m_is_button_down;
    u8 m_trafs_no;
    u8 m_button_no;
};


void newSoundMixer()
{
    fsNewTask(SoundMixer);
}


SoundMixer::SoundMixer() : fsTask(ORDER_ZERO)
{
    m_is_button_down = false;
    m_trafs_no = 0;
    m_button_no = 0;
}


static bool chefsClifs(u8* trk_no, u8* btn_no)
{
    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    r32 mouse_x = scr->framebufferXToScreenX(fsInputMgr::getMouseX());
    r32 mouse_y = scr->framebufferYToScreenY(fsInputMgr::getMouseY());

    *trk_no = static_cast<u8>((mouse_x + 300.0f) / 150.0f);
    mouse_x += -150.0f * *trk_no + 300.0f;

    for (s32 i = 0; i < 4; i++)
    {
        if (fsMath::abs(-35.0f * i + 130.0f - mouse_y) < 10.0f)
        {
            if (fsMath::abs(40.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2;

                return true;
            }
            else if (fsMath::abs(105.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2 + 1;

                return true;
            }
        }
    }

    r32 fader_y = 110.0f * fsSoundMgr::getTrackVolume(*trk_no) / 255.0f - 130.0f;

    if (fsMath::abs(75.0f - mouse_x) < 30.0f && fsMath::abs(fader_y - mouse_y) < 7.5f)
    {
        *btn_no = 8;

        return true;
    }

    return false;
}


static void drawLine(const fsVec& pos1, const fsVec& pos2)
{
    fsVec offset1(fsMath::rand(-2.0f, 2.0f, 1.0f), fsMath::rand(-2.0f, 2.0f, 1.0f));
    fsVec offset2(fsMath::rand(-2.0f, 2.0f, 1.0f), fsMath::rand(-2.0f, 2.0f, 1.0f));

    fsDebugMgr::drawLine(pos1 + offset1, pos2 + offset2, fsCol::FULL, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
}


static void drawRect(const fsVec& center, r32 width, r32 height, bool is_active)
{
    width /= 2.0f;
    height /= 2.0f;

    fsVec pos1(center.x - width, center.y + height);
    fsVec pos2(center.x - width, center.y - height);
    fsVec pos3(center.x + width, center.y + height);
    fsVec pos4(center.x + width, center.y - height);

    fsCol col = is_active ? fsCol(255, 128, 64, 255) : fsCol(128, 128, 128, 96);
    fsDebugMgr::drawPolygon(pos1, pos2, pos3, pos4, col, fsDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos4);
    drawLine(pos4, pos3);
    drawLine(pos3, pos1);
}


static void drawTriangle(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3, bool is_active)
{
    fsCol col = is_active ? fsCol(255, 128, 64, 255) : fsCol(128, 128, 128, 96);
    fsDebugMgr::drawPolygon(pos1, pos2, pos3, col, fsDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos3);
    drawLine(pos3, pos1);
}


static void drawMixer(bool is_pushed, u8 trk_no, u8 btn_no)
{
    /*
        draw frame
    */
    drawRect(fsVec::ZERO, 600.0f, 320.0f, false);
    drawLine(fsVec(-150.0f, 160.0f), fsVec(-150.0f, -160.0f));
    drawLine(fsVec(0.0f, 160.0f), fsVec(0.0f, -160.0f));
    drawLine(fsVec(150.0f, 160.0f), fsVec(150.0f, -160.0f));

    btn_no = is_pushed ? trk_no * 9 + btn_no + 1 : 0;

    for (u32 i = 0; i < fsSoundMgr::TRACK_NUM; i++)
    {
        /*
            draw buttons
        */
        r32 x = 150.0f * i - 225.0f;

        for (s32 j = 0; j < 3; j++)
        {
            r32 button_y = -35.0f * j + 130.0f;

            for (s32 k = 0; k < 2; k++)
            {
                drawRect(fsVec(x - 32.0f + 64.0f * k, button_y), 40.0f, 20.0f, (btn_no == i * 9 + j * 2 + k + 1));
            }
        }

        drawTriangle(fsVec(x - 32.0f, 35.0f), fsVec(x - 52.0f, 15.0f), fsVec(x - 12.0f, 15.0f), (btn_no == i * 9 + 7));
        drawTriangle(fsVec(x + 12.0f, 35.0f), fsVec(x + 32.0f, 15.0f), fsVec(x + 52.0f, 35.0f), (btn_no == i * 9 + 8));

        /*
            draw fader
        */
        r32 y = 110.0f * fsSoundMgr::getTrackVolume(i) / 255.0f - 130.0f;

        drawRect(fsVec(x, y), 60.0f, 15.0f, (btn_no == i * 9 + 9));

        if (y > -20.0f - 7.5f)
        {
            drawLine(fsVec(x, y - 7.5f), fsVec(x, -130.0f));
        }
        else if (y < -130.0f + 7.5f)
        {
            drawLine(fsVec(x, -20.0f), fsVec(x, y + 7.5f));
        }
        else
        {
            drawLine(fsVec(x, -20.0f), fsVec(x, y + 7.5f));
            drawLine(fsVec(x, y - 7.5f), fsVec(x, -130.0f));
        }
    }
}


void SoundMixer::onUpdate()
{
    if (fsInputMgr::isOff(fsInputMgr::KEY_LBUTTON))
    {
        m_is_button_down = false;
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_LBUTTON) && chefsClifs(&m_trafs_no, &m_button_no))
    {
        m_is_button_down = true;

        if (m_button_no < 6)
        {
            fsMsg<4> msg;
            msg.setParam(0, m_trafs_no);
            msg.setParam(1, m_button_no);

            fsTaskMgr::sendMessage(fsID_("PLAY"), msg);
        }
        else if (m_button_no < 8)
        {
            fsMsg<4> msg;
            msg.setParam(0, m_trafs_no);
            msg.setParam(1, static_cast<u8>((m_button_no == 6) ? 255 : 0));
            msg.setParam(2, 4.0f);

            fsTaskMgr::sendMessage(fsID_("FADE"), msg);
        }
    }

    if (m_is_button_down && m_button_no == 8)
    {
        r32 trk_vol = fsMath::clamp(255.0f * (370.0f - static_cast<r32>(fsInputMgr::getMouseY())) / 110.0f, 0.0f, 255.0f);

        fsSoundMgr::setTrackVolume(m_trafs_no, static_cast<u8>(trk_vol));
    }

    drawMixer(m_is_button_down, m_trafs_no, m_button_no);
}
