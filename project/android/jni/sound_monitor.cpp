/*
 * sound_monitor.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class SoundMonitor : public fsTask
{
public:
    SoundMonitor();

private:
    virtual void onUpdate();
    virtual void onMessage(fsID msg_id, fsMsg<4>& msg);
};


void newSoundMonitor()
{
    fsNewTask(SoundMonitor);
}


SoundMonitor::SoundMonitor() : fsTask(ORDER_ZERO)
{
    fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_MONO, fsSoundMgr::SAMPLE_RATE_11KHZ, 50);
}


void SoundMonitor::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(320, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_1))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_MONO, fsSoundMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_2))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_MONO, fsSoundMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_3))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_MONO, fsSoundMgr::SAMPLE_RATE_44KHZ, 50);
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_4))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_STEREO, fsSoundMgr::SAMPLE_RATE_11KHZ, 50);
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_5))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_STEREO, fsSoundMgr::SAMPLE_RATE_22KHZ, 50);
    }
    else if (fsInputMgr::isPressed(fsInputMgr::KEY_6))
    {
        fsSoundMgr::openSoundDevice(fsSoundMgr::CHANNEL_NUM_STEREO, fsSoundMgr::SAMPLE_RATE_44KHZ, 50);
    }

    if (!fsSoundMgr::isSoundDeviceOpen())
    {
        fsDebugMgr::drawString(-138, 0, fsCol::FULL, 2, "Can't Open Sound Device");
    }

    fsDebugMgr::drawString(-280.0f, -166.0f, fsCol(255, 128, 128, 255), 1, //
        "When the sound device's sample rate is higher than sound data's sample rate, noise may occur.");

    fsDebugMgr::drawString(-280.0f, -200.0f, fsCol::FULL, 1, "SOUND DEVICE CHANNEL NUM: %d", fsSoundMgr::getSoundDeviceChannelNum());
    fsDebugMgr::drawString(-280.0f, -210.0f, fsCol::FULL, 1, "SOUND DEVICE SAMPLE RATE: %dHz", fsSoundMgr::getSoundDeviceSampleRate());

    for (u32 i = 0; i < fsSoundMgr::TRACK_NUM; i++)
    {
        if (fsSoundMgr::isPlaying(i))
        {
            fsDebugMgr::drawString(-242.0f + 150.0f * i, -144.0f, fsCol(255, 255, 255, 96), 1, "%06d", fsSoundMgr::getPlayingPosition(i));
        }
    }
}


void SoundMonitor::onMessage(fsID msg_id, fsMsg<4>& msg)
{
    const u32 SOUND_NUM = 6;

    static const fsID s_snd_id[SOUND_NUM] =
    {
        fsID_("mono_11khz.wav"), fsID_("stereo_11khz.wav"), //
        fsID_("mono_22khz.wav"), fsID_("stereo_22khz.wav"), //
        fsID_("mono_44khz.wav"), fsID_("stereo_44khz.wav")
    };

    static const bool s_is_play_loop[SOUND_NUM] =
    {
        false, true, true, false, false, false
    };

    if (msg_id == fsID_("PLAY"))
    {
        u8 snd_no = msg.getParam<u8>(1);

        if (snd_no < SOUND_NUM)
        {
            fsSoundMgr::play(msg.getParam<u8>(0), s_snd_id[snd_no], 128, s_is_play_loop[snd_no]);
        }
    }
    else if (msg_id == fsID_("FADE"))
    {
        fsSoundMgr::fadeTrackVolume(msg.getParam<u8>(0), msg.getParam<u8>(1), msg.getParam<r32>(2));
    }
}
