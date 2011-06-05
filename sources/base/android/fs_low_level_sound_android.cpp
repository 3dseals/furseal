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


#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#include <AL/al.h>
#include <AL/alc.h>


namespace
{

	#define BUFFER_COUNT 4

	static bool s_is_snd_dev_open = false;
	volatile bool s_is_playing = false;
	static u8 s_channel_num;
	static u16 s_sample_rate;
	static u16 s_bits_per_sample;
	static u16 s_snd_mix_buf_msec;
	static u32 s_snd_mix_buf_size;
	static u32 s_snd_mix_buf_sample_num;

	ALCdevice*          m_pAudioDevice;
	ALCcontext*         m_pAlcContext;
	ALuint              m_AudioSource;
	ALuint              m_AudioBuffers[BUFFER_COUNT];
	ALenum              m_AudioFormat;
	fsLowLevelAPI::SoundMixFunction s_snd_mix_func;
	void* s_snd_mix_buf;//[2] = { NULL, NULL };
	void* buf ;

	pthread_mutex_t s_snd_mix_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
	pthread_t s_snd_play_thread;


	void* soundPlayThread(void*)
	{
		ALuint buffer;
		//ALint val=100;
		int size = 0;

		int x = 0;
		while(s_is_playing && x == 0)
		{
			for (s32 i = 0; i < BUFFER_COUNT & s_is_playing; i++){
				size = 0;
				for (s32 j = 0; j < 2; j++)
				{
					if ((*s_snd_mix_func)(buf))
					{
						memcpy( (void *) (s_snd_mix_buf + size), buf , s_snd_mix_buf_size ); //s_snd_mix_buf
						size += s_snd_mix_buf_size;

					}
				}
				if(size > 0)
				{
					x = 1;
					alBufferData(m_AudioBuffers[i], m_AudioFormat, (void *)s_snd_mix_buf, size, s_sample_rate);
				}
				else
				{
					i = -1;
				}
			}
			usleep(100);
		}
		alSourceQueueBuffers(m_AudioSource, BUFFER_COUNT, m_AudioBuffers);
		alSourcePlay(m_AudioSource);


		//alSourcePlay(m_AudioSource);
		while (s_is_playing)
		{


			int               val;
			alGetSourcei(m_AudioSource, AL_BUFFERS_PROCESSED, &val);
			if(val <= 0)
				continue;
			size = 0;
			for (s32 j = 0; j < 2; j++)
			{
				if ((*s_snd_mix_func)(buf))
				{
					size += s_snd_mix_buf_size;
					memcpy( s_snd_mix_buf + j *s_snd_mix_buf_size, buf , s_snd_mix_buf_size ); //s_snd_mix_buf

				}				
			}
			if(size > 0)
			{
				alSourceUnqueueBuffers(m_AudioSource, 1, &buffer);
				alBufferData(buffer, m_AudioFormat, (void *)s_snd_mix_buf, size, s_sample_rate);
				alSourceQueueBuffers(m_AudioSource, 1, &buffer);
			}
			alGetSourcei(m_AudioSource, AL_SOURCE_STATE, &val);
			if(val != AL_PLAYING)
				alSourcePlay(m_AudioSource);

			//pthread_yield();
		}
		//free(buf);
		pthread_exit(NULL);

	}
}

bool fsLowLevelAPI::isSoundDeviceOpen()
{
    // TODO

    return s_is_snd_dev_open;
}


bool fsLowLevelAPI::openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func)
{
	fprintf(stderr,"channel_num %d sample_rate %d snd_mix_buf_mse %d \n",channel_num, sample_rate, snd_mix_buf_msec);
    if (isSoundDeviceOpen())
    {
        closeSoundDevice();
    }

    pthread_mutex_init(&s_snd_mix_mutex, NULL);

    s_snd_mix_func =  snd_mix_func;

	m_pAudioDevice = alcOpenDevice(NULL);

	if (m_pAudioDevice)
	{
		m_pAlcContext = alcCreateContext(m_pAudioDevice, NULL);
		alcMakeContextCurrent(m_pAlcContext);
	}

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		return false;
	}

	alGenSources(1, &m_AudioSource);
	alGenBuffers(BUFFER_COUNT, m_AudioBuffers);
    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;

	s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;
	s_bits_per_sample = 16;

    if (s_channel_num != 1 && s_channel_num != 2)
    {
        return false;
    }
	
	switch (s_bits_per_sample){
	case 8:
		m_AudioFormat = (channel_num == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		m_AudioFormat = (channel_num == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		return 0;
	}
	//	m_AudioFormat = AL_FORMAT_STEREO16;
	s_snd_mix_buf_size = 2 * s_channel_num * s_snd_mix_buf_sample_num;
	/*for (s32 i = 0; i < 2; i++)
	  {
	  s_snd_mix_buf[i] = ckMalloc(s_snd_mix_buf_size);
	  ckMemMgr::memset(s_snd_mix_buf[i], 0, s_snd_mix_buf_size);
	  }*/
	buf = fsMalloc(s_snd_mix_buf_size);
	s_snd_mix_buf = fsMalloc(s_snd_mix_buf_size*2);
	//alGenBuffers(BUFFER_COUNT, m_AudioBuffers);
	//alGenSources(1, &m_AudioSource);
	//alSourcePlay(m_AudioSource);

	s_is_playing = true;
	if (pthread_create(&s_snd_play_thread, NULL, soundPlayThread, NULL) != 0)
	{
		s_is_playing = false;
		closeSoundDevice();
		return false;
	}

//    if (pthread_create(&s_snd_play_thread, NULL, soundPlayThread, NULL) != 0)
//    {
//        s_is_playing = false;
//        closeSoundDevice();
//        return false;
//    }
//
    s_is_snd_dev_open = true;

	return true;
}


void fsLowLevelAPI::closeSoundDevice()
{
    if (s_is_playing)
    {
        s_is_playing = false;
        pthread_join(s_snd_play_thread, NULL);
    }

	if(buf)
	{
		fsFree(buf);
		buf = NULL;
	}
	if (s_snd_mix_buf)
	{
		fsFree(s_snd_mix_buf);
		s_snd_mix_buf = NULL;
	}

	alSourceStop(m_AudioSource);
	alDeleteSources(1, &m_AudioSource);
	alDeleteBuffers(BUFFER_COUNT, m_AudioBuffers);
	if (m_pAudioDevice)
	{
		alcMakeContextCurrent(NULL);

		alcDestroyContext(m_pAlcContext);
		alcCloseDevice(m_pAudioDevice);
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
