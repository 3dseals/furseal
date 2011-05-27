/*
 * fs_mot.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_MOT_H_
#define FS_MOT_H_


/*!
    @ingroup fsDraw
    A motion for 3D model, which consists of motion nodes.
*/
class FS_API fsMot
{
    friend class fsJoin;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The motion play modes.
    */
    enum PlayMode
    {
        PLAY_NORMAL, //!< Play once.
        PLAY_LOOP, //!< Play repeatedly.
        PLAY_KEEP //!< Play and keep the last frame.
    };

    /*!
        Constructs a motion.
    */
    fsMot();

    /*!
        Destructs this motion.
    */
    ~fsMot();

    /*!
        Initializes this motion.
        @param[in] mdl A target model.
        @param[in] mot_data_id The ID of motion data.
    */
    void init(fsMdl* mdl, fsID mot_data_id);

    /*!
        Returns the index of the current motion.
        @return The index of the current motion.
    */
    u16 getMotionIndex() const;

    /*!
        Returns the play mode.
        @return The play mode.
    */
    PlayMode getPlayMode() const;

    /*!
        Returns the play speed.
        @return The play speed.
    */
    r32 getPlaySpeed() const;

    /*!
        Returns the number of the frames for interporation.
        @return The number of the frames for interporation.
    */
    u16 getInterpFrame() const;

    /*!
        Returns whether playing motion.
        @return Whether playing motion.
    */
    bool isPlaying() const;

    /*!
        Plays the specified motion.
        @param[in] mdl A target model.
        @param[in] mot_index The index of a motion.
        @param[in] play_mode A play mode.
        @param[in] play_speed A play speed.
        @param[in] interp_frame The number of the frames for interporation.
    */
    void play(fsMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame);

    /*!
        Stops playing motion.
    */
    void stop();

    /*!
        Returns the frame number of next updating time.
    */
    r32 getNextUpdateFrame() const;

    /*!
        Updates this motion.
    */
    void update(fsMdl* mdl);

private:
    struct InterpInfo
    {
        fsQuat quat;
        fsVec trans;
    };

    fsMot(const fsMot&);
    void operator=(const fsMot&);

    void uninit();

    fsMotData m_mot_data;
    InterpInfo* m_interp_info;
    fsType<u8, PlayMode> m_play_mode;
    u16 m_mot_index;
    u16 m_interp_frame;
    r32 m_play_speed;
    r32 m_playing_frame;
    r32 m_interp_speed;
    r32 m_interp_ratio;
};


#endif /* FS_MOT_H_ */
