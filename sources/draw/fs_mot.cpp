/*
 * fs_mot.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "base/fs_private_macro.h"


fsMot::fsMot()
{
    m_interp_info = NULL;
}


fsMot::~fsMot()
{
    uninit();
}


void fsMot::init(fsMdl* mdl, fsID mot_data_id)
{
    uninit();

    if (mot_data_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(mot_data_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_mot_data.initAsReader(res.getData<void>(), res.getDataSize());

    fsNewArray(m_interp_info, InterpInfo, m_mot_data.getNodeNum());

    m_play_mode = PLAY_NORMAL;
    m_mot_index = 0;
    m_interp_frame = 0;
    m_play_speed = 1.0f;
    m_playing_frame = -1.0f;
    m_interp_speed = 0.0f;
    m_interp_ratio = 0.0f;
}


u16 fsMot::getMotionIndex() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mot_index;
}


fsMot::PlayMode fsMot::getPlayMode() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_play_mode.getType();
}


r32 fsMot::getPlaySpeed() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_play_speed;
}


u16 fsMot::getInterpFrame() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_interp_frame;
}


bool fsMot::isPlaying() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame >= 0.0f);
}


void fsMot::play(fsMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame)
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!mdl || mot_index >= m_mot_data.getMotionNum())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_play_mode = play_mode;
    m_play_speed = play_speed;
    m_mot_index = mot_index;
    m_interp_frame = interp_frame;
    m_playing_frame = 0.0f;

    if (interp_frame > 0)
    {
        m_interp_speed = 1.0f / interp_frame;
        m_interp_ratio = 1.0f;

        const fsMdlData* mdl_data = mdl->getModelData();
        u16 node_num = fsMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

        for (s32 i = 0; i < node_num; i++)
        {
            InterpInfo* interp_info = &m_interp_info[i];
            fsMat local = mdl->getNodeDraw(i)->local().toLocalOf(mdl_data->getNodeLocal(i));

            interp_info->quat = fsQuat::fromMat(local);
            interp_info->trans = local.trans;
        }
    }
    else
    {
        m_interp_speed = 0.0f;
        m_interp_ratio = 0.0f;
    }
}


void fsMot::stop()
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_playing_frame = -1.0f;
}


r32 fsMot::getNextUpdateFrame() const
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return (m_playing_frame < 0.0f) ? 0.0f : m_playing_frame;
}


void fsMot::update(fsMdl* mdl)
{
    if (!m_interp_info)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!mdl)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying())
    {
        return;
    }

    /*
        find current key_frame_index
    */
    u32 key_frame_num = m_mot_data.getMotionKeyFrameNum(m_mot_index);
    u32 first_key_frame_index = m_mot_data.getMotionKeyFrameIndex(m_mot_index);
    u32 last_key_frame_index = first_key_frame_index + key_frame_num - 1;

    u32 key_frame_index;

    for (key_frame_index = last_key_frame_index; key_frame_index > first_key_frame_index; key_frame_index--)
    {
        if (m_playing_frame >= m_mot_data.getKeyFramePlayFrame(key_frame_index))
        {
            break;
        }
    }

    /*
        calculate blend ratio
    */
    u32 cur_key_frame_play_frame = m_mot_data.getKeyFramePlayFrame(key_frame_index);

    r32 blend_ratio = (key_frame_index >= last_key_frame_index) ? 0.0f : //
        (m_playing_frame - cur_key_frame_play_frame) / (m_mot_data.getKeyFramePlayFrame(key_frame_index + 1) - cur_key_frame_play_frame);

    bool is_blend = (blend_ratio > 0.0f);

    /*
        update interpolate ratio
    */
    if (m_interp_ratio > 0.0f)
    {
        m_interp_ratio -= m_interp_speed * m_play_speed;

        if (m_interp_ratio < 0.0f)
        {
            m_interp_ratio = 0.0f;
        }
    }

    bool is_interp = (m_interp_ratio > 0.0f);

    /*
        update model's local matrix
    */
    const fsMdlData* mdl_data = mdl->getModelData();
    u16 node_num = fsMath::min(m_mot_data.getNodeNum(), mdl_data->getNodeNum());

    for (s32 i = 0; i < node_num; i++)
    {
        fsQuat quat = m_mot_data.getNodeRotate(i, key_frame_index);
        fsVec trans = m_mot_data.getNodeTrans(i, key_frame_index);

        if (is_blend)
        {
            fsQuat quat2 = m_mot_data.getNodeRotate(i, key_frame_index + 1); // to avoid runtime error on iPhone
            fsVec trans2 = m_mot_data.getNodeTrans(i, key_frame_index + 1); // to avoid runtime error on iPhone

            quat = quat.slerp(quat2, blend_ratio);
            trans = trans.interp(trans2, blend_ratio);
        }

        if (is_interp)
        {
            InterpInfo* interp_info = &m_interp_info[i];

            quat = quat.slerp(interp_info->quat, m_interp_ratio);
            trans = trans.interp(interp_info->trans, m_interp_ratio);
        }

        mdl->getNodeDraw(i)->local() = quat.toMat(trans).toGlobalFrom(mdl_data->getNodeLocal(i));
    }

    /*
        update current frame
    */
    r32 frame_num = static_cast<r32>(m_mot_data.getMotionFrameNum(m_mot_index));
    bool is_frame_remaining = (m_playing_frame < frame_num);

    m_playing_frame += m_play_speed;

    if (m_playing_frame > frame_num)
    {
        if (m_play_mode == PLAY_LOOP)
        {
            m_playing_frame -= frame_num;
        }
        else if (m_play_mode == PLAY_KEEP || is_frame_remaining)
        {
            m_playing_frame = frame_num;
        }
        else
        {
            m_playing_frame = -1.0f;
        }
    }
}


FS_DEFINE_COPY_CONSTRUCTOR(fsMot)


FS_DEFINE_OPERATOR_EQUAL(fsMot)


void fsMot::uninit()
{
    if (m_interp_info)
    {
        fsDeleteArray(m_interp_info, InterpInfo);

        m_interp_info = NULL;
    }
}
