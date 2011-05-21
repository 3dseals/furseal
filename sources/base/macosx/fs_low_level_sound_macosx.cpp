/*
 * fs_low_level_sound_macosx.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


#ifdef FS_MACOSX


#include "base/fs_low_level_api.h"

#include "fs_mem_all.h"


static bool s_is_snd_dev_open = false;
// static bool s_is_playing;
static u8 s_channel_num;
static u16 s_sample_rate;
static u16 s_snd_mix_buf_msec;
static u32 s_snd_mix_buf_size;
static u32 s_snd_mix_buf_sample_num;


bool fsLowLevelAPI::isSoundDeviceOpen()
{
    // TODO

    return s_is_snd_dev_open;
}


bool fsLowLevelAPI::openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func)
{
    if (isSoundDeviceOpen())
    {
        closeSoundDevice();
    }

    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;
    s_snd_mix_buf_size = channel_num * sample_rate * 2 * snd_mix_buf_msec / 1000;
    s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;

    // TODO
    // s_is_snd_dev_open = true;

    return false; // TODO
}


void fsLowLevelAPI::closeSoundDevice()
{
    // TODO

    s_is_snd_dev_open = false;
}


u8 fsLowLevelAPI::getSoundDeviceChannelNum()
{
    return s_channel_num;
}


u16 fsLowLevelAPI::getSoundDeviceSampleRate()
{
    return s_sample_rate;
}


u16 fsLowLevelAPI::getSoundMixBufferMsec()
{
    return s_snd_mix_buf_msec;
}


u32 fsLowLevelAPI::getSoundMixBufferSize()
{
    return s_snd_mix_buf_size;
}


u32 fsLowLevelAPI::getSoundMixBufferSampleNum()
{
    return s_snd_mix_buf_sample_num;
}


void fsLowLevelAPI::lockSoundMixMutex()
{
    // TODO
}


void fsLowLevelAPI::unlockSoundMixMutex()
{
    // TODO
}


#endif // FS_MACOSX
