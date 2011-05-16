/*
 * fs_sound_mgr.h
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SOUND_MGR_H_
#define FS_SOUND_MGR_H_


class fsSoundDaemon;


/*!
    @ingroup fsSound
    The sound manager singleton.
*/
class FS_API fsSoundMgr
{
    friend class fsSoundDaemon;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionInvalidType);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionWaveInitializerFailed);
    //! @endcond

    /*!
        The number of the tracks.
    */
    static const u32 TRACK_NUM = 4;

    /*!
        The types of the channel number.
    */
    enum ChannelNum
    {
        CHANNEL_NUM_MONO = 1, //!< Monaural.
        CHANNEL_NUM_STEREO = 2 //!< Stereo.
    };

    /*!
        The sample rate types.
    */
    enum SampleRate
    {
        SAMPLE_RATE_11KHZ = 11025, //!< 11 kHz.
        SAMPLE_RATE_22KHZ = 22050, //!< 22 kHz.
        SAMPLE_RATE_44KHZ = 44100 //!< 44 kHz.
    };

    /*!
        Returns whether the sound manager singleton is created.
        @return Whether the sound manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the sound manager singleton.
    */
    static void createAfterRes();

    /*!
        Destroys the sound manager singleton.
    */
    static void destroyBeforeRes();

    /*!
        Returns whether the sound device is open.
        @return Whether the sound device is open.
    */
    static bool isSoundDeviceOpen();

    /*!
        Opens the sound device.
        @param[in] channel_num The number of the channels.
        @param[in] sample_rate The sample rate.
        @param[in] snd_mix_buf_msec The size of the sound-mixing buffer in milli second.
        @return Whether succeed in opening the sound device.
    */
    static bool openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec);

    /*!
        Closes the sound device.
    */
    static void closeSoundDevice();

    /*!
        Returns the number of the channels.
        @return The number of the channels.
    */
    static ChannelNum getSoundDeviceChannelNum();

    /*!
        Returns the sample rate.
        @return The sample rate.
    */
    static SampleRate getSoundDeviceSampleRate();

    /*!
        Returns the length of the sound-mixing buffer in milli second.
        @return The length of the sound-mixing buffer in milli second.
    */
    static u16 getSoundMixBufferMsec();

    /*!
        Returns the number of the channels of the specified sound.
        @param[in] snd_id The resource ID of a sound.
        @return The number of the channels of the specified sound.
    */
    static ChannelNum getChannelNum(fsID snd_id);

    /*!
        Returns the sample rate of the specified sound.
        @param[in] snd_id The resource ID of a sound.
        @return The sample rate of the specified sound.
    */
    static SampleRate getSampleRate(fsID snd_id);

    /*!
        Returns the number of the samples of the specified sound.
        @param[in] snd_id The resource ID of a sound.
        @return The number of the samples of the specified sound.
    */
    static u32 getSampleNum(fsID snd_id);

    /*!
        Returns the volume of the specified track.
        @return The volume of the specified track.
    */
    static u8 getTrackVolume(u8 trk_no);

    /*!
        Sets the volume of the specified track.
        @param[in] trk_no A track number.
        @param[in] trk_vol The volume of a track.
    */
    static void setTrackVolume(u8 trk_no, u8 trk_vol);

    /*!
        Starts fade in/out of the track volume.
        @param[in] trk_no A track number.
        @param[in] to_trk_vol The volume of a track.
        @param[in] delta_per_frame The speed per frame.
    */
    static void fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame);

    /*!
        Returns whether the specified track is playing.
        @return Whether the specified track is playing.
    */
    static bool isPlaying(u8 trk_no);

    /*!
        Plays the specified sound.
        @param[in] trk_no A track number.
        @param[in] snd_id The resource ID of a sound.
        @param[in] snd_vol The volume of a sound.
        @param[in] is_play_loop Whether enables loop playback.
        @param[in] play_pos The play position.
    */
    static void play(u8 trk_no, fsID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos = 0);

    /*!
        Stops playing sound.
        @param[in] trk_no A track number.
    */
    static void stop(u8 trk_no);

    /*!
        Returns the resource ID of the playing sound of the specified track.
        @param[in] trk_no A track number.
    */
    static fsID getPlayingSoundID(u8 trk_no);

    /*!
        Returns the sound volume of the specified track.
    */
    static u8 getPlayingSoundVolume(u8 trk_no);

    /*!
        Returns whether the specified track enables loop playback.
        @param[in] trk_no The track number.
        @return Whether the specified track enables loop playback.
    */
    static bool isPlayingLoop(u8 trk_no);

    /*!
        Returns the playing position of the specified track.
        @param[in] trk_no The track number.
        @return The playing position of the specified track.
    */
    static u32 getPlayingPosition(u8 trk_no);

private:
    enum TrackState
    {
        STATE_STOP, //
        STATE_PLAY_ONESHOT, //
        STATE_PLAY_LOOP
    };

    struct SoundInfo
    {
        fsType<u8, ChannelNum> channel_num;
        fsType<u16, SampleRate> sample_rate;
        const void* data;
        u32 data_size;
        u32 sample_num;
    };

    struct TrackInfo
    {
        fsType<u8, TrackState> trk_state;
        r32 trk_vol;
        fsID snd_id;
        u8 snd_vol;
        const SoundInfo* snd_info;
        u32 play_pos;
        u8 fade_to_trk_vol;
        u32 fade_frame_count;
        r32 fade_delta_per_frame;
    };

    fsSoundMgr();
    ~fsSoundMgr();
    void operator=(const fsSoundMgr&);

    static fsSoundMgr* instance();

    static bool soundMixFunction(void* snd_mix_buf);

    static void waveInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void waveFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    TrackInfo m_trk_info[TRACK_NUM];
    fsSoundDaemon* m_snd_daemon;

    static fsSoundMgr* m_instance;
};

#endif /* FS_SOUND_MGR_H_ */
