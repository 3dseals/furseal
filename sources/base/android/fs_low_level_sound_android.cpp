/*
 * fs_low_level_sound_android.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
 */


#ifdef FS_ANDROID


#include "base/fs_low_level_api.h"

#include "fs_mem_all.h"

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define BUFFER_SIZE (8 * 1024)
#define BUFFER_COUNT 4

static bool s_is_snd_dev_open = false;
static bool s_is_playing;
static u8 s_channel_num;
static u16 s_sample_rate;
static u16 s_snd_mix_buf_msec;
static u32 s_snd_mix_buf_size;
static u32 s_snd_mix_buf_sample_num;

fsLowLevelAPI::SoundMixFunction s_snd_mix_func;
void* s_snd_mix_buf[2] = { NULL, NULL };

pthread_mutex_t s_snd_mix_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
pthread_t s_snd_play_thread;


void* soundPlayThread(void*)
{
    while (s_is_playing)
    {
        for (s32 i = 0; i < 2; i++)
        {
            if ((*s_snd_mix_func)(s_snd_mix_buf[i]))
            {

            }
            else
            {
                fsLowLevelAPI::sleepUsec(s_snd_mix_buf_msec * 1000);
            }
        }

        fsLowLevelAPI::sleepUsec(1000); // 1msec
    }
    pthread_exit(NULL);
}


char ogg_buffers_temp[BUFFER_SIZE];

// will decode ogg file as much as can into temp buffer
static size_t decodeogg(u32)
{
    char* buffer = ogg_buffers_temp;
    size_t left = sizeof(ogg_buffers_temp);
    return sizeof(ogg_buffers_temp) - left;
}


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

    pthread_mutex_init(&s_snd_mix_mutex, NULL);

    s_snd_mix_func =  snd_mix_func;

    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;

    s_snd_mix_buf_size = channel_num * sample_rate * 2 * snd_mix_buf_msec / 1000;
    s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;

    if (s_channel_num != 1 && s_channel_num != 2)
    {
        return false;
    }

//    s_is_playing = true;
//
//    if (pthread_create(&s_snd_play_thread, NULL, soundPlayThread, NULL) != 0)
//    {
//        s_is_playing = false;
//        closeSoundDevice();
//        return false;
//    }
//
//    s_is_snd_dev_open = true;

    return false;
}


void fsLowLevelAPI::closeSoundDevice()
{
    if (s_is_playing)
    {
        s_is_playing = false;
        pthread_join(s_snd_play_thread, NULL);
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (s_snd_mix_buf[i])
        {
            ckFree(s_snd_mix_buf[i]);
            s_snd_mix_buf[i] = NULL;
        }
    }

	pthread_mutex_destroy(&s_snd_mix_mutex);

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
	pthread_mutex_lock(&s_snd_mix_mutex);
}


void fsLowLevelAPI::unlockSoundMixMutex()
{
	pthread_mutex_unlock(&s_snd_mix_mutex);
}


#endif // FS_ANDROID
