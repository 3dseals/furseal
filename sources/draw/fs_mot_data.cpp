/*
 * fs_node_data.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "base/fs_private_macro.h"


fsMotData::fsMotData()
{
    m_data = NULL;
}


fsMotData::~fsMotData()
{
    if (m_data && m_mode == MODE_WRITE)
    {
        fsFree(m_data);
    }
}


void fsMotData::initAsReader(const void* data, u32 data_size)
{
    if (!data || data_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        fsFree(m_data);
    }

    m_mode = MODE_READ;
    m_data = reinterpret_cast<u8*>(const_cast<void*>(data));
    m_data_size = data_size;
    m_mot_data_header = reinterpret_cast<MotionDataHeader*>(m_data);
    m_mot_info = reinterpret_cast<MotionInfo*>(m_mot_data_header + 1);
    m_key_frame_info = reinterpret_cast<KeyFrameInfo*>(m_data + m_mot_data_header->key_frame_info_offset);
    m_node_info = reinterpret_cast<NodeInfo*>(m_data + m_mot_data_header->node_info_offset);

    if (m_mot_data_header->format_id != fsID_("CATCAKE_MOTION_DATA") || m_mot_data_header->format_version > MOTION_DATA_VERSION)
    {
        fsThrow(ExceptionInvalidData);
    }

    u32 valid_data_size = sizeof(MotionDataHeader) + sizeof(MotionInfo) * getMotionNum() + //
        sizeof(KeyFrameInfo) * getKeyFrameNum() + sizeof(NodeInfo) * getNodeNum() * getKeyFrameNum();

    if (m_data_size != valid_data_size)
    {
        fsThrow(ExceptionInvalidData);
    }
}


void fsMotData::initAsWriter(u16 node_num, u16 mot_num, u32 key_frame_num)
{
    if (node_num == 0 || mot_num == 0 || key_frame_num == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        fsFree(m_data);
    }

    m_mode = MODE_WRITE;

    u32 node_info_num = node_num * key_frame_num;

    u32 mot_info_size = sizeof(MotionInfo) * mot_num;
    u32 key_frame_info_size = sizeof(KeyFrameInfo) * key_frame_num;
    u32 node_info_size = sizeof(NodeInfo) * node_info_num;

    m_data_size = sizeof(MotionDataHeader) + mot_info_size + key_frame_info_size + node_info_size;
    m_data = static_cast<u8*>(fsMalloc(m_data_size));

    fsMemHelper::memset(m_data, 0, m_data_size);

    m_mot_data_header = reinterpret_cast<MotionDataHeader*>(m_data);

    m_mot_data_header->format_id = fsID_("CATCAKE_MOTION_DATA");
    m_mot_data_header->format_version = MOTION_DATA_VERSION;
    m_mot_data_header->node_num = node_num;
    m_mot_data_header->mot_num = mot_num;
    m_mot_data_header->key_frame_num = key_frame_num;
    m_mot_data_header->key_frame_info_offset = sizeof(MotionDataHeader) + mot_info_size;
    m_mot_data_header->node_info_offset = m_mot_data_header->key_frame_info_offset + key_frame_info_size;

    m_mot_info = reinterpret_cast<MotionInfo*>(m_mot_data_header + 1);
    m_key_frame_info = reinterpret_cast<KeyFrameInfo*>(m_data + m_mot_data_header->key_frame_info_offset);
    m_node_info = reinterpret_cast<NodeInfo*>(m_data + m_mot_data_header->node_info_offset);

    for (s32 i = 0; i < mot_num; i++)
    {
        m_mot_info[i].frame_num = 1;
        m_mot_info[i].key_frame_num = 1;
    }

    for (u32 i = 0; i < node_info_num; i++)
    {
        m_node_info[i].m_quat.w = 1.0f;
    }
}


fsMotData::MotionDataMode fsMotData::getMode() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mode.getType();
}


u16 fsMotData::getNodeNum() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->node_num;
}


u16 fsMotData::getMotionNum() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->mot_num;
}


u32 fsMotData::getKeyFrameNum() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mot_data_header->key_frame_num;
}


u32 fsMotData::getMotionFrameNum(u16 mot_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].frame_num;
}


u32 fsMotData::getMotionKeyFrameIndex(u16 mot_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].key_frame_index;
}


u32 fsMotData::getMotionKeyFrameNum(u16 mot_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (mot_index >= m_mot_data_header->mot_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_mot_info[mot_index].key_frame_num;
}


void fsMotData::setMotionInfo(u16 mot_index, u32 frame_num, u32 key_frame_index, u32 key_frame_num)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (mot_index >= m_mot_data_header->mot_num || frame_num == 0 || key_frame_num == 0 || //
        key_frame_index + key_frame_num > m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    MotionInfo* mot_info = &m_mot_info[mot_index];

    mot_info->frame_num = frame_num;
    mot_info->key_frame_index = key_frame_index;
    mot_info->key_frame_num = key_frame_num;
}


u32 fsMotData::getKeyFramePlayFrame(u32 key_frame_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (key_frame_index >= m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_key_frame_info[key_frame_index].play_frame;
}


void fsMotData::setKeyFramePlayFrame(u32 key_frame_index, u32 play_frame)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (key_frame_index >= m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_key_frame_info[key_frame_index].play_frame = play_frame;
}


const fsQuat& fsMotData::getNodeRotate(u16 node_index, u32 key_frame_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index].m_quat;
}


const fsVec& fsMotData::getNodeTrans(u16 node_index, u32 key_frame_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index].m_trans;
}


void fsMotData::setNodeLocal(u16 node_index, u32 key_frame_index, const fsMat& local)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mot_data_header->node_num || key_frame_index >= m_mot_data_header->key_frame_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    NodeInfo* joint_info = &m_node_info[m_mot_data_header->key_frame_num * node_index + key_frame_index];

    joint_info->m_quat = fsQuat::fromMat(local);
    joint_info->m_trans = local.trans;
}


void fsMotData::registerAsResource(fsID res_id)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (res_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsResMgr::addResource(res_id, "", m_data, m_data_size, true);

    m_mode = MODE_READ;
}


FS_DEFINE_COPY_CONSTRUCTOR(fsMotData)


FS_DEFINE_OPERATOR_EQUAL(fsMotData)
