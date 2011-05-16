/*
 * fs_sound_mgr_mix.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_sound_all.h"

#include "fs_gen_all.h"
#include "fs_kernel_all.h"
#include "fs_base_all.h"


const s32 MIN_AMPLITUDE = -32765;
const s32 MAX_AMPLITUDE = 32765;


#define SOUND_MIX_CODE(src_sample_size, dest_sample_size) \
    while (cur_dest_sample < dest_sample_num) \
    { \
        if (cur_src_sample >= src_sample_num) \
        { \
            if (trk_info->trk_state == STATE_PLAY_LOOP) \
            { \
                cur_src_sample = 0; \
                src = static_cast<const u8*>(trk_info->snd_info->data); \
            } \
            else \
            { \
                break; \
            } \
        } \
    \
        INJECTION_CODE1 \
    \
        for (u32 i = 0; i < dest_sample_step && cur_dest_sample < dest_sample_num; i++) \
        { \
            INJECTION_CODE2 \
    \
            cur_dest_sample++; \
            dest += dest_sample_size; \
        } \
    \
        cur_src_sample += src_sample_step; \
        src += src_sample_step * src_sample_size; \
    }


bool fsSoundMgr::soundMixFunction(void* snd_mix_buf)
{
    fsSoundMgr* ins = instance();
    bool is_playing = false;

    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        if (ins->m_trk_info[i].trk_state != STATE_STOP)
        {
            is_playing = true;
            break;
        }
    }

    if (!is_playing)
    {
        return false;
    }

    TrackInfo temp_trk_info[TRACK_NUM];

    fsLowLevelAPI::lockSoundMixMutex();
    {
        for (u32 i = 0; i < TRACK_NUM; i++)
        {
            TrackInfo* trk_info = &ins->m_trk_info[i];

            temp_trk_info[i] = *trk_info;

            if (trk_info->trk_state == STATE_STOP)
            {
                continue;
            }

            trk_info->play_pos += fsLowLevelAPI::getSoundMixBufferSampleNum() * //
                trk_info->snd_info->sample_rate.getValue() / fsLowLevelAPI::getSoundDeviceSampleRate();

            if (trk_info->play_pos >= trk_info->snd_info->sample_num)
            {
                if (trk_info->trk_state == STATE_PLAY_ONESHOT)
                {
                    trk_info->trk_state = STATE_STOP;
                    trk_info->play_pos = trk_info->snd_info->sample_num;
                }
                else // STATE_PLAY_LOOP
                {
                    trk_info->play_pos = trk_info->play_pos % trk_info->snd_info->sample_num;
                }
            }
        }
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    fsMemHelper::memset(snd_mix_buf, 0, fsLowLevelAPI::getSoundMixBufferSize());

    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        TrackInfo* trk_info = &temp_trk_info[i];

        if (trk_info->trk_state == STATE_STOP)
        {
            continue;
        }

        const u8* src = static_cast<const u8*>(trk_info->snd_info->data) + trk_info->snd_info->channel_num.getValue() * 2 * trk_info->play_pos;
        u8* dest = static_cast<u8*>(snd_mix_buf);

        u32 src_sample_rate = trk_info->snd_info->sample_rate.getValue();
        u32 dest_sample_rate = fsLowLevelAPI::getSoundDeviceSampleRate();

        u32 src_sample_step = fsMath::max(src_sample_rate / dest_sample_rate, static_cast<u32>(1));
        u32 dest_sample_step = fsMath::max(dest_sample_rate / src_sample_rate, static_cast<u32>(1));

        u32 src_sample_num = trk_info->snd_info->sample_num;
        u32 dest_sample_num = fsLowLevelAPI::getSoundMixBufferSampleNum();

        u32 cur_src_sample = trk_info->play_pos;
        u32 cur_dest_sample = 0;

        u8 mix_vol = static_cast<u8>((trk_info->trk_vol * trk_info->snd_vol) / 255.0f);

        if (mix_vol == 0)
        {
            continue;
        }

        if (trk_info->snd_info->channel_num == CHANNEL_NUM_MONO)
        {
            if (fsLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (fsReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = fsMath::clamp(fsReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 fsWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(2, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp = (fsReadLittleEndian(src, s16) * mix_vol) >> 8; \
    s32 dest_amp = fsMath::clamp(fsReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    fsWriteLittleEndian(dest + 0, dest_amp, s16); \
    fsWriteLittleEndian(dest + 2, dest_amp, s16);

                SOUND_MIX_CODE(2, 4)
            }
        }
        else
        {
            if (fsLowLevelAPI::getSoundDeviceChannelNum() == 1)
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (fsReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (fsReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 src_amp = (src_amp_l + src_amp_r) / 2; \
    s32 dest_amp = fsMath::clamp(fsReadLittleEndian(dest, s16) + src_amp, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 fsWriteLittleEndian(dest, dest_amp, s16);

                SOUND_MIX_CODE(4, 2)
            }
            else
            {
#undef INJECTION_CODE1
#define INJECTION_CODE1 \
    s32 src_amp_l = (fsReadLittleEndian(src + 0, s16) * mix_vol) >> 8; \
    s32 src_amp_r = (fsReadLittleEndian(src + 2, s16) * mix_vol) >> 8; \
    s32 dest_amp_l = fsMath::clamp(fsReadLittleEndian(dest + 0, s16) + src_amp_l, MIN_AMPLITUDE, MAX_AMPLITUDE); \
    s32 dest_amp_r = fsMath::clamp(fsReadLittleEndian(dest + 2, s16) + src_amp_r, MIN_AMPLITUDE, MAX_AMPLITUDE);

#undef INJECTION_CODE2
#define INJECTION_CODE2 \
    fsWriteLittleEndian(dest + 0, dest_amp_l, s16); \
    fsWriteLittleEndian(dest + 2, dest_amp_r, s16);

                SOUND_MIX_CODE(4, 4)
            }
        }
    }

    return true;
}
