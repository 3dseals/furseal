/*
 * fs_sound_mgr.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_sound_all.h"

#include "fs_gen_all.h"
#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_res_all.h"
#include "fs_base_all.h"


fsSoundMgr* fsSoundMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsSoundMgr)


FS_DEFINE_MANAGER_CREATE(fsSoundMgr, AfterRes, BeforeRes)


FS_DEFINE_MANAGER_DESTROY(fsSoundMgr, BeforeRes)


bool fsSoundMgr::isSoundDeviceOpen()
{
    instance();

    return fsLowLevelAPI::isSoundDeviceOpen();
}


bool fsSoundMgr::openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec)
{
    instance();

    if (snd_mix_buf_msec == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (fsLowLevelAPI::openSoundDevice(channel_num, sample_rate, snd_mix_buf_msec, soundMixFunction))
    {
        return true;
    }
    else
    {
        fsLowLevelAPI::printf("*** can't open sound device ***");

        return false;
    }
}


void fsSoundMgr::closeSoundDevice()
{
    instance();

    if (fsLowLevelAPI::isSoundDeviceOpen())
    {
        fsLowLevelAPI::closeSoundDevice();
    }
}


fsSoundMgr::ChannelNum fsSoundMgr::getSoundDeviceChannelNum()
{
    instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return CHANNEL_NUM_MONO;
    }

    return static_cast<ChannelNum>(fsLowLevelAPI::getSoundDeviceChannelNum());
}


fsSoundMgr::SampleRate fsSoundMgr::getSoundDeviceSampleRate()
{
    instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return SAMPLE_RATE_11KHZ;
    }

    return static_cast<SampleRate>(fsLowLevelAPI::getSoundDeviceSampleRate());
}


u16 fsSoundMgr::getSoundMixBufferMsec()
{
    instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    return fsLowLevelAPI::getSoundMixBufferMsec();
}


fsSoundMgr::ChannelNum fsSoundMgr::getChannelNum(fsID snd_id)
{
    instance();

    if (snd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(snd_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        fsThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->channel_num.getType();
}


fsSoundMgr::SampleRate fsSoundMgr::getSampleRate(fsID snd_id)
{
    instance();

    if (snd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(snd_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        fsThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_rate.getType();
}


u32 fsSoundMgr::getSampleNum(fsID snd_id)
{
    instance();

    if (snd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(snd_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV")
    {
        fsThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_num;
}


u8 fsSoundMgr::getTrackVolume(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    u8 trk_vol;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        trk_vol = static_cast<u8>(ins->m_trk_info[trk_no].trk_vol);
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return trk_vol;
}


void fsSoundMgr::setTrackVolume(u8 trk_no, u8 trk_vol)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_vol = static_cast<u8>(trk_vol);
        ins->m_trk_info[trk_no].fade_frame_count = 0;
    }
    fsLowLevelAPI::unlockSoundMixMutex();
}


void fsSoundMgr::fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || delta_per_frame < fsMath::EPSILON)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::lockSoundMixMutex();
    {
        TrackInfo* trk_info = &ins->m_trk_info[trk_no];
        u32 fade_frame_count = static_cast<u32>(fsMath::abs<r32>(to_trk_vol - trk_info->trk_vol) / delta_per_frame);

        if (fade_frame_count > 0)
        {
            trk_info->fade_to_trk_vol = to_trk_vol;
            trk_info->fade_frame_count = fade_frame_count;
            trk_info->fade_delta_per_frame = (to_trk_vol > trk_info->trk_vol) ? delta_per_frame : -delta_per_frame;
        }
        else
        {
            trk_info->trk_vol = to_trk_vol;
        }
    }
    fsLowLevelAPI::unlockSoundMixMutex();
}


bool fsSoundMgr::isPlaying(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    bool is_playing;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        is_playing = (ins->m_trk_info[trk_no].trk_state != STATE_STOP);
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return is_playing;
}


void fsSoundMgr::play(u8 trk_no, fsID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || snd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(snd_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionNotFound);
    }

    if (res.getExtension() != "WAV")
    {
        fsThrow(ExceptionInvalidType);
    }

    SoundInfo* snd_info = res.getExInfo<SoundInfo>();

    if (play_pos >= snd_info->sample_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    TrackInfo* trk_info = &ins->m_trk_info[trk_no];

    fsLowLevelAPI::lockSoundMixMutex();
    {
        trk_info->trk_state = is_play_loop ? STATE_PLAY_LOOP : STATE_PLAY_ONESHOT;
        trk_info->snd_id = snd_id;
        trk_info->snd_vol = snd_vol;
        trk_info->snd_info = snd_info;
        trk_info->play_pos = play_pos;
    }
    fsLowLevelAPI::unlockSoundMixMutex();
}


void fsSoundMgr::stop(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_state = STATE_STOP;
    }
    fsLowLevelAPI::unlockSoundMixMutex();
}


fsID fsSoundMgr::getPlayingSoundID(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return fsID::ZERO;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        fsThrow(ExceptionInvalidCall);
    }

    fsID snd_id;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        snd_id = ins->m_trk_info[trk_no].snd_id;
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return snd_id;
}


u8 fsSoundMgr::getPlayingSoundVolume(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        fsThrow(ExceptionInvalidCall);
    }

    u8 snd_vol;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        snd_vol = ins->m_trk_info[trk_no].snd_vol;
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return snd_vol;
}


bool fsSoundMgr::isPlayingLoop(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        fsThrow(ExceptionInvalidCall);
    }

    bool is_playing_loop;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        is_playing_loop = (ins->m_trk_info[trk_no].trk_state == STATE_PLAY_LOOP);
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return is_playing_loop;
}


u32 fsSoundMgr::getPlayingPosition(u8 trk_no)
{
    fsSoundMgr* ins = instance();

    if (!fsLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        fsThrow(ExceptionInvalidCall);
    }

    u32 play_pos;

    fsLowLevelAPI::lockSoundMixMutex();
    {
        play_pos = ins->m_trk_info[trk_no].play_pos;
    }
    fsLowLevelAPI::unlockSoundMixMutex();

    return play_pos;
}


class fsSoundDaemon : public fsTask
{
public:
    fsSoundDaemon() : fsTask(ORDER_PLUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        fsSoundMgr* ins = fsSoundMgr::instance();

        for (u32 i = 0; i < fsSoundMgr::TRACK_NUM; i++)
        {
            fsSoundMgr::TrackInfo* trk_info = &ins->m_trk_info[i];

            if (trk_info->fade_frame_count > 0)
            {
                trk_info->trk_vol = fsMath::clamp(trk_info->trk_vol + trk_info->fade_delta_per_frame, 0.0f, 255.0f);
                trk_info->fade_frame_count--;

                if (trk_info->fade_frame_count == 0)
                {
                    trk_info->trk_vol = trk_info->fade_to_trk_vol;
                }
            }
        }
    }
};


fsSoundMgr::fsSoundMgr()
{
    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        TrackInfo* trk_info = &m_trk_info[i];

        trk_info->trk_state = STATE_STOP;
        trk_info->trk_vol = 192.0f;
        trk_info->fade_frame_count = 0;
    }

    m_snd_daemon = fsNewTask(fsSoundDaemon);

    fsResMgr::addType("WAV", waveInitializer, waveFinalizer);
}


fsSoundMgr::~fsSoundMgr()
{
    if (fsTaskMgr::isCreated())
    {
        fsDeleteTask(m_snd_daemon);
    }

    fsLowLevelAPI::closeSoundDevice();

    for (const fsRes* res = fsResMgr::getFirstResourceN(); res; res = fsResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "WAV")
        {
            waveFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    fsResMgr::removeType("WAV");
}


FS_DEFINE_OPERATOR_EQUAL(fsSoundMgr)


FS_DEFINE_MANAGER_INSTANCE(fsSoundMgr)


void fsSoundMgr::waveInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    const u8* buf = static_cast<const u8*>(data);

    if (buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F' || //
        buf[8] != 'W' || buf[9] != 'A' || buf[10] != 'V' || buf[11] != 'E')
    {
        fsThrow(ExceptionWaveInitializerFailed);
    }

    buf += 12;

    bool is_fmt_chunk = false;
    bool is_data_chunk = false;
    const u8* end = buf + data_size - 1;
    SoundInfo* snd_info = fsNew(SoundInfo);
    u16 bits_per_sample = 0;

    while (!is_fmt_chunk || !is_data_chunk)
    {
        u32 chunk_size = fsReadLittleEndian(buf + 4, u32);

        if (buf + 8 + chunk_size > end)
        {
            fsDelete(snd_info, SoundInfo);
            fsThrow(ExceptionWaveInitializerFailed);
        }

        if (buf[0] == 'f' && buf[1] == 'm' && buf[2] == 't' && buf[3] == ' ')
        {
            if (fsReadLittleEndian(buf + 8, u16) != 1)
            {
                fsDelete(snd_info, SoundInfo);
                fsThrow(ExceptionWaveInitializerFailed);
            }

            is_fmt_chunk = true;

            snd_info->channel_num = static_cast<ChannelNum>(fsReadLittleEndian(buf + 10, u16));
            snd_info->sample_rate = static_cast<SampleRate>(fsReadLittleEndian(buf + 12, u32));
            bits_per_sample = fsReadLittleEndian(buf + 22, u16);
        }
        else if (buf[0] == 'd' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'a')
        {
            is_data_chunk = true;

            snd_info->data = buf + 8;
            snd_info->data_size = chunk_size;
        }

        buf += 8 + chunk_size;
    }

    if ((snd_info->channel_num != CHANNEL_NUM_MONO && snd_info->channel_num != CHANNEL_NUM_STEREO) || //
        (snd_info->sample_rate != SAMPLE_RATE_11KHZ && snd_info->sample_rate != SAMPLE_RATE_22KHZ && snd_info->sample_rate != SAMPLE_RATE_44KHZ) || //
        bits_per_sample != 16 || snd_info->data_size == 0)
    {
        fsThrow(ExceptionWaveInitializerFailed);
    }

    snd_info->sample_num = snd_info->data_size / (snd_info->channel_num.getValue() * 2);

    *exinfo = snd_info;
}


void fsSoundMgr::waveFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    fsDelete(exinfo, SoundInfo);
}
