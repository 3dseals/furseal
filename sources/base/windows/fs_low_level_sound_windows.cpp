/*
 * fs_low_level_sound_windows.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
 */


#ifdef WINDOWS


#include <windows.h>

#include "base/fs_low_level_api.h"

#include "fs_mem_all.h"


static bool s_is_snd_dev_open = false;
static bool s_is_playing;
static u8 s_channel_num;
static u16 s_sample_rate;
static u16 s_snd_mix_buf_msec;
static u32 s_snd_mix_buf_size;
static u32 s_snd_mix_buf_sample_num;
static fsLowLevelAPI::SoundMixFunction s_snd_mix_func;
static void* s_snd_mix_buf[2] = /**/ { /**/ NULL, NULL /**/ };
static HWAVEOUT s_hwo = NULL;
static WAVEFORMATEX s_wfx;
static WAVEHDR s_wh[2];
static void* s_snd_mix_mutex = NULL;
static void* s_snd_play_thread = NULL;


static void soundPlayThread(LPVOID)
{
    while (s_is_playing)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if (s_wh[i].dwFlags & WHDR_DONE)
            {
                if ((*s_snd_mix_func)(s_snd_mix_buf[i]))
                {
                    waveOutWrite(s_hwo, &s_wh[i], sizeof(WAVEHDR));
                }
                else
                {
                    fsLowLevelAPI::sleepUsec(s_snd_mix_buf_msec * 1000);
                }
            }
        }

        fsLowLevelAPI::sleepUsec(1000); // 1msec
    }
}


bool fsLowLevelAPI::isSoundDeviceOpen()
{
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
    s_snd_mix_func = snd_mix_func;

    for (s32 i = 0; i < 2; i++)
    {
        s_snd_mix_buf[i] = fsMalloc(s_snd_mix_buf_size);
        fsMemHelper::memset(s_snd_mix_buf[i], 0, s_snd_mix_buf_size);
    }

    s_wfx.wFormatTag = WAVE_FORMAT_PCM;
    s_wfx.nChannels = s_channel_num;
    s_wfx.wBitsPerSample = 16;
    s_wfx.nBlockAlign = s_channel_num * 2;
    s_wfx.nSamplesPerSec = s_sample_rate;
    s_wfx.nAvgBytesPerSec = s_sample_rate * s_wfx.nBlockAlign;

    waveOutOpen(&s_hwo, WAVE_MAPPER, &s_wfx, static_cast<DWORD_PTR>(NULL), static_cast<DWORD_PTR>(NULL), CALLBACK_NULL);

    if (!s_hwo)
    {
        closeSoundDevice();
        return false;
    }

    for (s32 i = 0; i < 2; i++)
    {
        s_wh[i].lpData = static_cast<LPSTR>(s_snd_mix_buf[i]);
        s_wh[i].dwBufferLength = s_snd_mix_buf_size;
        s_wh[i].dwBytesRecorded = 0;
        s_wh[i].dwFlags = 0;
        s_wh[i].dwLoops = 1;

        waveOutPrepareHeader(s_hwo, &s_wh[i], sizeof(WAVEHDR));
        waveOutWrite(s_hwo, &s_wh[i], sizeof(WAVEHDR));
    }

    waveOutReset(s_hwo);

    s_snd_mix_mutex = newMutex();

    if (!s_snd_mix_mutex)
    {
        closeSoundDevice();
        return false;
    }

    s_is_playing = true;
    s_snd_play_thread = newThread(soundPlayThread, NULL);

    if (!s_snd_play_thread)
    {
        closeSoundDevice();
        return false;
    }

    s_is_snd_dev_open = true;

    return true;
}


void fsLowLevelAPI::closeSoundDevice()
{
    if (s_snd_play_thread)
    {
        s_is_playing = false;
        joinThread(s_snd_play_thread);
        deleteThread(s_snd_play_thread);

        s_snd_play_thread = NULL;
    }

    if (s_snd_mix_mutex)
    {
        deleteMutex(s_snd_mix_mutex);
        s_snd_mix_mutex = NULL;
    }

    if (s_hwo)
    {
        waveOutReset(s_hwo);
        waveOutUnprepareHeader(s_hwo, &s_wh[0], sizeof(WAVEHDR));
        waveOutUnprepareHeader(s_hwo, &s_wh[1], sizeof(WAVEHDR));
        waveOutClose(s_hwo);

        s_hwo = NULL;
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (s_snd_mix_buf[i])
        {
            fsFree(s_snd_mix_buf[i]);
            s_snd_mix_buf[i] = NULL;
        }
    }

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
    lockMutex(s_snd_mix_mutex);
}


void fsLowLevelAPI::unlockSoundMixMutex()
{
    unlockMutex(s_snd_mix_mutex);
}


#endif // WIN32
