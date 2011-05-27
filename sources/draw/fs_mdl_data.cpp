/*
 * fs_mdl_data.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "base/fs_private_macro.h"


fsMdlData::fsMdlData()
{
    m_data = NULL;
}


fsMdlData::~fsMdlData()
{
    if (m_data && m_mode == MODE_WRITE)
    {
        fsFree(m_data);
    }
}


void fsMdlData::initAsReader(const void* data, u32 data_size)
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
    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);
    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<fsPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<fsVec*>(m_data + m_mdl_data_header->normal_data_offset);

    if (m_mdl_data_header->format_id != fsID_("CATCAKE_MODEL_DATA") || m_mdl_data_header->format_version > MODEL_DATA_VERSION)
    {
        fsThrow(ExceptionInvalidData);
    }

    u32 valid_data_size = sizeof(ModelDataHeader) + sizeof(NodeInfo) * getNodeNum() + (sizeof(fsPrim::PrimData) + (hasNormal() ? sizeof(fsVec) : 0)) * getVertNum();

    if (m_data_size != valid_data_size)
    {
        fsThrow(ExceptionInvalidData);
    }
}


void fsMdlData::initAsWriter(u16 node_num, u16 vert_num, fsID tex_id, bool has_normal)
{
    if (node_num == 0 || vert_num == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_data && m_mode == MODE_WRITE)
    {
        fsFree(m_data);
    }

    m_mode = MODE_WRITE;

    u32 node_info_size = sizeof(NodeInfo) * node_num;
    u32 prim_data_size = sizeof(fsPrim::PrimData) * vert_num;
    u32 normal_data_size = has_normal ? sizeof(fsVec) * vert_num : 0;

    m_data_size = sizeof(ModelDataHeader) + node_info_size + prim_data_size + normal_data_size;
    m_data = static_cast<u8*>(fsMalloc(m_data_size));

    fsMemHelper::memset(m_data, 0, m_data_size);

    m_mdl_data_header = reinterpret_cast<ModelDataHeader*>(m_data);

    m_mdl_data_header->format_id = fsID_("CATCAKE_MODEL_DATA");
    m_mdl_data_header->format_version = MODEL_DATA_VERSION;
    m_mdl_data_header->node_num = node_num;
    m_mdl_data_header->vert_num = vert_num;
    m_mdl_data_header->flag.set(FLAG_HAS_NORMAL, has_normal);
    m_mdl_data_header->tex_id = tex_id;
    m_mdl_data_header->prim_data_offset = sizeof(ModelDataHeader) + node_info_size;
    m_mdl_data_header->normal_data_offset = m_mdl_data_header->prim_data_offset + prim_data_size;

    m_node_info = reinterpret_cast<NodeInfo*>(m_mdl_data_header + 1);
    m_prim_data = reinterpret_cast<fsPrim::PrimData*>(m_data + m_mdl_data_header->prim_data_offset);
    m_normal_data = reinterpret_cast<fsVec*>(m_data + m_mdl_data_header->normal_data_offset);

    for (s32 i = 0; i < node_num; i++)
    {
        m_node_info[i].local = fsMat::UNIT;
    }

    for (s32 i = 0; i < vert_num; i++)
    {
        m_prim_data[i].col = fsCol::FULL;

        if (has_normal)
        {
            m_normal_data[i] = fsVec::Z_UNIT;
        }
    }
}


fsMdlData::ModelDataMode fsMdlData::getMode() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mode.getType();
}


u16 fsMdlData::getNodeNum() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->node_num;
}


u16 fsMdlData::getVertNum() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->vert_num;
}


bool fsMdlData::hasNormal() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->flag.isOn(FLAG_HAS_NORMAL);
}


fsID fsMdlData::getTextureID() const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_mdl_data_header->tex_id;
}


u16 fsMdlData::getNodeParentIndex(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].parent_index;
}


void fsMdlData::setNodeParentIndex(u16 node_index, u16 parent_index)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || parent_index >= m_mdl_data_header->node_num || node_index == parent_index)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = parent_index;
}


void fsMdlData::setNodeParentIndex_noParent(u16 node_index)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].parent_index = node_index;
}


const fsMat& fsMdlData::getNodeLocal(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].local;
}


void fsMdlData::setNodeLocal(u16 node_index, const fsMat& local)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].local = local;
}


fsPrim::PrimMode fsMdlData::getNodePrimMode(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].prim_mode.getType();
}


void fsMdlData::setNodePrimMode(u16 node_index, fsPrim::PrimMode prim_mode)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].prim_mode = prim_mode;
}


fsPrim::BlendMode fsMdlData::getNodeBlendMode(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].blend_mode.getType();
}


void fsMdlData::setNodeBlendMode(u16 node_index, fsPrim::BlendMode blend_mode)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_node_info[node_index].blend_mode = blend_mode;
}


u16 fsMdlData::getNodeVertIndex(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_index;
}


u16 fsMdlData::getNodeVertNum(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].vert_num;
}


void fsMdlData::setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num || vert_index >= m_mdl_data_header->vert_num || vert_index + vert_num > m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    NodeInfo* node_info = &m_node_info[node_index];

    node_info->vert_index = vert_index;
    node_info->vert_num = vert_num;
}


const fsPrim::PrimData* fsMdlData::getNodePrimData(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return &m_prim_data[m_node_info[node_index].vert_index];
}


const fsVec* fsMdlData::getNodeNormalData(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return &m_normal_data[m_node_info[node_index].vert_index];
}


const fsVec& fsMdlData::getNodeClipBoundMinForReader(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_min;
}


const fsVec& fsMdlData::getNodeClipBoundMaxForReader(u16 node_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_WRITE)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (node_index >= m_mdl_data_header->node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_node_info[node_index].bound_max;
}


const fsVec& fsMdlData::getVertPos(u16 vert_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].pos;
}


void fsMdlData::setVertPos(u16 vert_index, const fsVec& pos)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].pos = pos;
}


fsCol fsMdlData::getVertCol(u16 vert_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].col;
}


void fsMdlData::setVertCol(u16 vert_index, fsCol col)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_prim_data[vert_index].col = col;
}


r32 fsMdlData::getVertU(u16 vert_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].u;
}


r32 fsMdlData::getVertV(u16 vert_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[vert_index].v;
}


void fsMdlData::setVertUV(u16 vert_index, r32 u, r32 v)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsPrim::PrimData* prim_data = &m_prim_data[vert_index];

    prim_data->u = u;
    prim_data->v = v;
}


const fsVec& fsMdlData::getVertN(u16 vert_index) const
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_normal_data[vert_index];
}


void fsMdlData::setVertN(u16 vert_index, const fsVec& n)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (vert_index >= m_mdl_data_header->vert_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_normal_data[vert_index] = n;
}


void fsMdlData::calcNormalAsTriangles(bool is_smoothing)
{
    if (!m_data)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_mode == MODE_READ || m_mdl_data_header->flag.isOff(FLAG_HAS_NORMAL))
    {
        fsThrow(ExceptionInvalidCall);
    }

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];
        u16 vert_num = node_info->vert_num;

        if (vert_num > 0)
        {
            u16 vert_index = node_info->vert_index;

            fsUtil::calcNormalAsTriangles(&m_normal_data[vert_index], &m_prim_data[vert_index], vert_num, is_smoothing);
        }
    }
}


void fsMdlData::registerAsResource(fsID res_id)
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

    for (s32 i = 0; i < m_mdl_data_header->node_num; i++)
    {
        NodeInfo* node_info = &m_node_info[i];

        fsPrim::PrimData* prim_data = &m_prim_data[node_info->vert_index];

        node_info->bound_min = node_info->bound_max = prim_data->pos;

        prim_data++;

        for (s32 j = 1; j < node_info->vert_num; j++)
        {
            if (prim_data->pos.x < node_info->bound_min.x)
            {
                node_info->bound_min.x = prim_data->pos.x;
            }

            if (prim_data->pos.y < node_info->bound_min.y)
            {
                node_info->bound_min.y = prim_data->pos.y;
            }

            if (prim_data->pos.z < node_info->bound_min.z)
            {
                node_info->bound_min.z = prim_data->pos.z;
            }

            if (prim_data->pos.x > node_info->bound_max.x)
            {
                node_info->bound_max.x = prim_data->pos.x;
            }

            if (prim_data->pos.y > node_info->bound_max.y)
            {
                node_info->bound_max.y = prim_data->pos.y;
            }

            if (prim_data->pos.z > node_info->bound_max.z)
            {
                node_info->bound_max.z = prim_data->pos.z;
            }

            prim_data++;
        }
    }

    fsResMgr::addResource(res_id, "", m_data, m_data_size, true);

    m_mode = MODE_READ;
}


FS_DEFINE_COPY_CONSTRUCTOR(fsMdlData)


FS_DEFINE_OPERATOR_EQUAL(fsMdlData)
