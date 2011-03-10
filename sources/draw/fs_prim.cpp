/*
 * fs_prim.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


fsPrim::PrimData::PrimData()
{
    pos = fsVec::ZERO;
    col = fsCol::FULL;
    u = v = 0.0f;
}


fsPrim::fsPrim()
{
    m_type = TYPE_PRIM;

    m_prim_mode = MODE_TRIANGLE_STRIP;
    m_is_share_data = false;
    m_prim_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
    m_rend = NULL;
}


fsPrim::~fsPrim()
{
    if (m_rend)
    {
        m_rend->uninit();
    }

    if (m_prim_data && !m_is_share_data.getType())
    {
        fsDeleteArray(m_prim_data, PrimData);
    }
}


void fsPrim::init(PrimMode prim_mode, u16 max_data_num, fsID scr_id)
{
    init2(prim_mode, false, NULL, max_data_num, scr_id, NULL);
}


void fsPrim::init(PrimMode prim_mode, u16 max_data_num, fsDraw* parent)
{
    init2(prim_mode, false, NULL, max_data_num, fsID::ZERO, parent);
}


void fsPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, fsID scr_id)
{
    init2(prim_mode, true, prim_data, max_data_num, scr_id, NULL);
}


void fsPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, fsDraw* parent)
{
    init2(prim_mode, true, prim_data, max_data_num, fsID::ZERO, parent);
}


bool fsPrim::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


fsPrim::PrimMode fsPrim::getPrimMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_prim_mode.getType();
}


void fsPrim::setPrimMode(PrimMode prim_mode)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_prim_mode = prim_mode;
}


u16 fsPrim::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void fsPrim::setCurDataNum(u16 cur_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (cur_data_num > m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_cur_data_num = cur_data_num;
}


u16 fsPrim::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void fsPrim::reallocData(u16 max_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (m_is_share_data.getType())
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (max_data_num == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_rend)
    {
        m_rend->reallocData(max_data_num);
    }

    if (m_prim_data)
    {
        PrimData* new_prim_data;
        fsNewArray(new_prim_data, PrimData, max_data_num);

        u32 copy_num = fsMath::min(max_data_num, m_max_data_num);

        fsMemHelper::memcpy(new_prim_data, m_prim_data, sizeof(PrimData) * copy_num);

        fsDeleteArray(m_prim_data, PrimData);

        m_max_data_num = max_data_num;
        m_prim_data = new_prim_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        fsNewArray(m_prim_data, PrimData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void fsPrim::copyData(u16 dest_index, const fsPrim* src_prim, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!src_prim || dest_index >= m_max_data_num || src_index >= src_prim->m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (m_rend)
    {
        m_rend->copyData(dest_index, src_prim, src_index);
    }

    m_prim_data[dest_index] = src_prim->m_prim_data[src_index];
}


fsVec& fsPrim::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].pos;
}


fsCol& fsPrim::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].col;
}


r32& fsPrim::dataU(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].u;
}


r32& fsPrim::dataV(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].v;
}


void fsPrim::setDataUV(u16 index, r32 u, r32 v)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    PrimData* prim_data = &m_prim_data[index];

    prim_data->u = u;
    prim_data->v = v;
}


void fsPrim::setDataRect(u16 index, const fsVec& center, r32 width, r32 height, fsCol col, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index + 3 >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    width /= 2.0f;
    height /= 2.0f;

    PrimData* prim_data = &m_prim_data[index];

    (prim_data + 0)->pos = center + fsVec(-width, height);
    (prim_data + 1)->pos = center + fsVec(-width, -height);
    (prim_data + 2)->pos = center + fsVec(width, -height);
    (prim_data + 3)->pos = center + fsVec(width, height);

    (prim_data + 0)->col = col;
    (prim_data + 1)->col = col;
    (prim_data + 2)->col = col;
    (prim_data + 3)->col = col;

    (prim_data + 0)->u = u1;
    (prim_data + 0)->v = v1;
    (prim_data + 1)->u = u1;
    (prim_data + 1)->v = v2;
    (prim_data + 2)->u = u2;
    (prim_data + 2)->v = v2;
    (prim_data + 3)->u = u2;
    (prim_data + 3)->v = v1;
}


void fsPrim::render(const fsMat& view)
{
    if (m_rend && m_rend->isActive())
    {
        m_rend->render(view);
    }
//    else if (fsDrawMgr::isShaderAvailable())
//    {
//        render_shader(view);
//    }
    else
    {
        render_soft(view);
    }
}


void fsPrim::init2(PrimMode prim_mode, bool is_share_data, PrimData* prim_data, u16 max_data_num, fsID scr_id, fsDraw* parent)
{
    if (m_rend)
    {
        m_rend->uninit();
    }

    m_private_flag.setOn(FLAG_INITIALIZED);

    setPrimMode(prim_mode);

    if (is_share_data)
    {
        if (!prim_data || max_data_num == 0)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (m_prim_data && !m_is_share_data.getType())
        {
            fsDeleteArray(m_prim_data, PrimData);
        }

        m_is_share_data = true;
        m_prim_data = const_cast<PrimData*>(prim_data);
        m_max_data_num = max_data_num;
    }
    else
    {
        if (m_prim_data && m_is_share_data.getType())
        {
            m_prim_data = NULL;
            m_max_data_num = 0;
        }

        m_is_share_data = false;

        reallocData(max_data_num);
    }

    setCurDataNum(max_data_num);
    //setTextureID(fsID::ZERO);

    if (parent)
    {
        setParent(parent);
    }
    else
    {
        setScreenID(scr_id);
    }
}


void fsPrim::render_soft(const fsMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        reset shader
    */
    fsLowLevelAPI::setShader(0);

    /*
        alloc buffer
    */
    bool tex = false;
    u32 col_size = (m_final_col != fsCol::FULL) ? sizeof(fsCol) * m_cur_data_num : 0;
    u32 uv_size = 0;
    u32 buf_size = col_size + uv_size;

    u8* buf = (buf_size > 0) ? reinterpret_cast<u8*>(fsMemHelper::allocTempBufferForEngine(buf_size)) : 0;

    fsCol* col_buf = reinterpret_cast<fsCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup color
    */
    if (col_size > 0)
    {
        for (s32 i = 0; i < m_cur_data_num; i++)
        {
            col_buf[i] = m_final_col * m_prim_data[i].col;
        }

        fsLowLevelAPI::setColorPointer(sizeof(fsCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        fsLowLevelAPI::setColorPointer(sizeof(PrimData), reinterpret_cast<const u8*>(&m_prim_data->col));
    }

    /*
        setup texture
    */
    if (tex)
    {
        //fsLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        if (uv_size > 0)
        {
            fsLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, reinterpret_cast<const r32*>(uv_buf));
        }
        else
        {
            fsLowLevelAPI::setTexCoordPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->u));
        }
    }
    else
    {
        fsLowLevelAPI::setTexture(0, 0, 0, false);
        fsLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw primitives
    */
    fsLowLevelAPI::setVertexPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->pos));
    fsLowLevelAPI::drawArrays(static_cast<fsLowLevelAPI::DrawMode>(m_prim_mode.getType()), 0, m_cur_data_num);
}


void fsPrim::render_shader(const fsMat& view)
{

}
