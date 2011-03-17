/*
 * fs_rend.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"


#include "fs_base_all.h"


fsRend::~fsRend()
{
    uninit();
}


bool fsRend::isActive() const
{
    if (!m_prim)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_is_active.getType();
}


void fsRend::setActive(bool is_active)
{
    if (!m_prim)
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_is_active = is_active;
}


fsRend::fsRend()
{
    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


void fsRend::init(fsPrim* prim, u32 body_size, u32 data_size)
{
    init2(prim, body_size, data_size, false, NULL);
}


void fsRend::init(fsPrim* prim, u32 body_size, u32 data_size, void* rend_data)
{
    init2(prim, body_size, data_size, true, rend_data);
}


fsPrim* fsRend::getPrim() const
{
    if (!m_prim)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_prim;
}


void fsRend::renderAllocBuffer(fsVec** pos_buf, fsCol** col_buf, r32** uv_buf, fsVec** normal_buf, //
    const fsPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf)
{
    u32 data_size = 0;

    if (has_pos_buf)
    {
        data_size += sizeof(fsVec);
    }

    if (has_col_buf)
    {
        data_size += sizeof(fsCol);
    }

    if (has_uv_buf)
    {
        data_size += sizeof(r32) * 2;
    }

    if (has_normal_buf)
    {
        data_size += sizeof(fsVec);
    }

    if (data_size == 0)
    {
        *pos_buf = NULL;
        *col_buf = NULL;
        *uv_buf = NULL;
        *normal_buf = NULL;

        return;
    }

    u8* buf = static_cast<u8*>(fsMemHelper::allocTempBufferForEngine(data_size * prim->m_cur_data_num));

    if (has_pos_buf)
    {
        *pos_buf = reinterpret_cast<fsVec*>(buf);
        buf += sizeof(fsVec) * prim->m_cur_data_num;
    }
    else
    {
        *pos_buf = NULL;
    }

    if (has_col_buf)
    {
        *col_buf = reinterpret_cast<fsCol*>(buf);
        buf += sizeof(fsCol) * prim->m_cur_data_num;
    }
    else
    {
        *col_buf = NULL;
    }

    if (has_uv_buf)
    {
        *uv_buf = reinterpret_cast<r32*>(buf);
        buf += sizeof(r32) * 2 * prim->m_cur_data_num;
    }
    else
    {
        *uv_buf = NULL;
    }

    if (has_normal_buf)
    {
        *normal_buf = reinterpret_cast<fsVec*>(buf);
        buf += sizeof(fsVec) * prim->m_cur_data_num;
    }
    else
    {
        *normal_buf = NULL;
    }
}


const fsMat& fsRend::renderGetPrimWorld(const fsPrim* prim)
{
    return prim->m_world;
}


fsCol fsRend::renderGetPrimFinalColor(const fsPrim* prim)
{
    return prim->m_final_col;
}


void fsRend::renderCalcColorBuffer(fsCol* col_buf, const fsPrim* prim)
{
    fsPrim::PrimData* prim_data = prim->m_prim_data;

    for (s32 i = 0; i < prim->m_cur_data_num; i++)
    {
        col_buf[i] = prim->m_final_col * prim_data[i].col;
    }
}


void fsRend::renderCalcUVBuffer(r32* uv_buf, const fsPrim* prim)
{
    bool tex = false;
    r32 u_param_a = tex;
    r32 u_param_b = tex;
    r32 v_param_a = tex;
    r32 v_param_b = tex;

    for (u32 i = 0; i < prim->m_cur_data_num; i++)
    {
        fsPrim::PrimData* prim_data = &prim->m_prim_data[i];

        *uv_buf = prim_data->u * u_param_a + u_param_b;
        uv_buf++;

        *uv_buf = prim_data->v * v_param_a + v_param_b;
        uv_buf++;
    }
}


void fsRend::renderSetVertexPointer(u32 stride, const r32* vert)
{
    fsLowLevelAPI::setVertexPointer(stride, vert);
}


void fsRend::renderSetColorPointer(u32 stride, const u8* color)
{
    fsLowLevelAPI::setColorPointer(stride, color);
}


void fsRend::renderSetTexCoordPointer(u32 stride, const r32* uv)
{
    fsLowLevelAPI::setTexCoordPointer(stride, uv);
}


void fsRend::renderSetTexture(const fsPrim* prim)
{

}


void fsRend::renderSetVertexPointer(const fsPrim* prim)
{
    fsLowLevelAPI::setVertexPointer(sizeof(fsPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->pos));
}


void fsRend::renderSetColorPointer(const fsPrim* prim)
{
    fsLowLevelAPI::setColorPointer(sizeof(fsPrim::PrimData), reinterpret_cast<const u8*>(&prim->m_prim_data->col));
}


void fsRend::renderSetTexCoordPointer(const fsPrim* prim)
{
    fsLowLevelAPI::setTexCoordPointer(sizeof(fsPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->u));
}


void fsRend::renderDrawArrays(u8 prim_mode, u16 first, u16 count)
{
    fsLowLevelAPI::drawArrays(static_cast<fsLowLevelAPI::DrawMode>(prim_mode), first, count);
}


void fsRend::renderSetUniform_s32(u32 location, s32 uniform)
{
    fsLowLevelAPI::setUniform_s32(location, uniform);
}


void fsRend::renderSetUniform_r32(u32 location, r32 uniform)
{
    fsLowLevelAPI::setUniform_r32(location, uniform);
}


void fsRend::renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib)
{
    fsLowLevelAPI::setAttribPointer_r32(location, size, stride, attrib);
}


void fsRend::renderCallPrimRenderWithDestroyingBuffer(fsPrim* prim, const fsMat& view)
{

        prim->render_soft(view);

}


FS_DEFINE_COPY_CONSTRUCTOR(fsRend)


FS_DEFINE_OPERATOR_EQUAL(fsRend)


void fsRend::init2(fsPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data)
{
    if (!prim)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    prim->getPrimMode(); // assure the fsPrim is initialized

    if (prim->m_rend)
    {
        prim->m_rend->uninit();
    }

    uninit();

    if (prim->m_is_share_data.getType() != is_share_data)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (data_size > 0 && is_share_data && !rend_data)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_is_active = true;
    m_rend_body_size = body_size;
    m_rend_data_size = data_size;
    m_prim = prim;
    m_rend_body = (m_rend_body_size > 0) ? fsMalloc(m_rend_body_size) : NULL;

    if (is_share_data)
    {
        m_rend_data = rend_data;
    }
    else if (m_rend_data_size > 0)
    {
        m_rend_data = fsMalloc(m_rend_data_size * m_prim->m_max_data_num);

        initData(m_rend_data, m_prim->m_max_data_num);
    }
    else
    {
        m_rend_data = NULL;
    }

    m_prim->m_rend = this;
}


void fsRend::uninit()
{
    if (m_prim)
    {
        m_prim->m_rend = NULL;
    }

    if (m_rend_body)
    {
        fsFree(m_rend_body);
    }

    if (m_rend_data && !m_prim->m_is_share_data.getType())
    {
        fsFree(m_rend_data);
    }

    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


u16 fsRend::getMaxDataNum() const
{
    return m_prim->getMaxDataNum();
}


void fsRend::reallocData(u16 max_data_num)
{
    if (m_rend_data_size > 0)
    {
        void* new_rend_data = fsMalloc(m_rend_data_size * max_data_num);
        u32 copy_num = fsMath::min(max_data_num, m_prim->m_max_data_num);

        initData(new_rend_data, max_data_num);

        fsMemHelper::memcpy(new_rend_data, m_rend_data, m_rend_data_size * copy_num);

        fsFree(m_rend_data);

        m_rend_data = new_rend_data;
    }
}


void fsRend::copyData(u16 dest_index, const fsPrim* src_prim, u16 src_index)
{
    fsRend* src_rend = src_prim->m_rend;

    if (!src_rend || src_rend->getClassID() != getClassID())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_rend_data_size > 0)
    {
        void* src = static_cast<u8*>(src_rend->m_rend_data) + m_rend_data_size * src_index;
        void* dest = static_cast<u8*>(m_rend_data) + m_rend_data_size * dest_index;

        fsMemHelper::memcpy(dest, src, m_rend_data_size);
    }
}
