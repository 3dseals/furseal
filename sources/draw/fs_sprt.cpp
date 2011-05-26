/*
 * fs_sprt.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_base_all.h"


fsSprt::SprtData::SprtData()
{
    pos = fsVec::ZERO;
    col = fsCol::FULL;
    w = h = 0.0f;
    u1 = v1 = 0.0f;
    u2 = v2 = 1.0f;
    ang = 0;
}


fsSprt::fsSprt()
{
    m_type = TYPE_SPRT;

    m_is_share_data = false;
    m_sprt_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
}


fsSprt::~fsSprt()
{
    if (m_sprt_data && !m_is_share_data.getType())
    {
        fsDeleteArray(m_sprt_data, SprtData);
    }
}


void fsSprt::init(u16 max_data_num, fsID scr_id)
{
    init2(false, NULL, max_data_num, scr_id, NULL);
}


void fsSprt::init(u16 max_data_num, fsDraw* parent)
{
    init2(false, NULL, max_data_num, fsID::ZERO, parent);
}


void fsSprt::init(SprtData* sprt_data, u16 max_data_num, fsID scr_id)
{
    init2(true, sprt_data, max_data_num, scr_id, NULL);
}


void fsSprt::init(SprtData* sprt_data, u16 max_data_num, fsDraw* parent)
{
    init2(true, sprt_data, max_data_num, fsID::ZERO, parent);
}


bool fsSprt::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


u16 fsSprt::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void fsSprt::setCurDataNum(u16 cur_data_num)
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


u16 fsSprt::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void fsSprt::reallocData(u16 max_data_num)
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

    if (m_sprt_data)
    {
        SprtData* new_sprt_data;
        fsNewArray(new_sprt_data, SprtData, max_data_num);

        u32 copy_num = fsMath::min(max_data_num, m_max_data_num);

        fsMemHelper::memcpy(new_sprt_data, m_sprt_data, sizeof(SprtData) * copy_num);

        fsDeleteArray(m_sprt_data, SprtData);

        m_max_data_num = max_data_num;
        m_sprt_data = new_sprt_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        fsNewArray(m_sprt_data, SprtData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void fsSprt::copyData(u16 dest_index, const fsSprt* src_sprt, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!src_sprt || dest_index >= m_max_data_num || src_index >= src_sprt->m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_sprt_data[dest_index] = src_sprt->m_sprt_data[src_index];
}


fsVec& fsSprt::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].pos;
}


fsCol& fsSprt::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].col;
}


r32& fsSprt::dataW(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].w;
}


r32& fsSprt::dataH(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].h;
}


r32& fsSprt::dataU1(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u1;
}


r32& fsSprt::dataV1(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v1;
}


r32& fsSprt::dataU2(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u2;
}


r32& fsSprt::dataV2(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v2;
}


s32& fsSprt::dataAng(u16 index)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].ang;
}


void fsSprt::setDataSize(u16 index, r32 width, r32 height)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->w = width;
    sprt_data->h = height;
}


void fsSprt::setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index >= m_max_data_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->u1 = u1;
    sprt_data->v1 = v1;
    sprt_data->u2 = u2;
    sprt_data->v2 = v2;
}


void fsSprt::init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, fsID scr_id, fsDraw* parent)
{
    m_private_flag.setOn(FLAG_INITIALIZED);

    if (is_share_data)
    {
        if (!sprt_data || max_data_num == 0)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (m_sprt_data && !m_is_share_data.getType())
        {
            fsDeleteArray(m_sprt_data, SprtData);
        }

        m_is_share_data = true;
        m_sprt_data = const_cast<SprtData*>(sprt_data);
        m_max_data_num = max_data_num;
    }
    else
    {
        if (m_sprt_data && m_is_share_data.getType())
        {
            m_sprt_data = NULL;
            m_max_data_num = 0;
        }

        m_is_share_data = false;

        reallocData(max_data_num);
    }

    setCurDataNum(max_data_num);
    setTextureID(fsID::ZERO);

    if (parent)
    {
        setParent(parent);
    }
    else
    {
        setScreenID(scr_id);
    }
}


void fsSprt::render(const fsMat& view)
{
    if (fsDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void fsSprt::render_soft(const fsMat& view)
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
    fsTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    u32 pos_size = sizeof(fsVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(fsCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(fsMemHelper::allocTempBufferForEngine(pos_size + col_size + uv_size));

    fsVec* pos_buf = reinterpret_cast<fsVec*>(buf);
    buf += pos_size;

    fsCol* col_buf = reinterpret_cast<fsCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    fsVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    fsVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    fsVec* pos_ptr = pos_buf;
    fsCol* col_ptr = col_buf;
    r32* uv_ptr = uv_buf;

    u16 count = 0;

    for (s32 i = 0; i < m_cur_data_num; i++)
    {
        if (m_final_col.a == 0)
        {
            continue;
        }

        count += 4;

        SprtData* sprt_data = &m_sprt_data[i];
        fsVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = fsMath::sin_s32(-sprt_data->ang);
            r32 cos = fsMath::cos_s32(-sprt_data->ang);

            vec_w = (view_x * cos - view_y * sin) * sprt_data->w / 2.0f;
            vec_h = (view_y * cos + view_x * sin) * sprt_data->h / 2.0f;
        }

        *(pos_ptr + 0) = sprt_data->pos - vec_w + vec_h;
        *(pos_ptr + 1) = sprt_data->pos - vec_w - vec_h;
        *(pos_ptr + 2) = sprt_data->pos + vec_w - vec_h;
        *(pos_ptr + 3) = sprt_data->pos + vec_w + vec_h;

        pos_ptr += 4;

        *(col_ptr + 0) = *(col_ptr + 1) = *(col_ptr + 2) = *(col_ptr + 3) = m_final_col * sprt_data->col;

        col_ptr += 4;

        if (tex)
        {
            r32 u1 = sprt_data->u1;
            r32 v1 = sprt_data->v1;
            r32 u2 = sprt_data->u2;
            r32 v2 = sprt_data->v2;

            *(uv_ptr + 0) = u1;
            *(uv_ptr + 1) = v1;

            *(uv_ptr + 2) = u1;
            *(uv_ptr + 3) = v2;

            *(uv_ptr + 4) = u2;
            *(uv_ptr + 5) = v2;

            *(uv_ptr + 6) = u2;
            *(uv_ptr + 7) = v1;

            uv_ptr += 8;
        }
    }

    /*
        setup texture
    */
    if (tex)
    {
        fsLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        if (tex->m_flag.isOn(fsTex::FLAG_UV_ADJUST))
        {
            r32* uv_ptr = uv_buf;

            for (s32 i = 0; i < count; i++)
            {
                *uv_ptr = *uv_ptr * tex->m_u_param_a + tex->m_u_param_b;
                uv_ptr++;

                *uv_ptr = *uv_ptr * tex->m_v_param_a + tex->m_v_param_b;
                uv_ptr++;
            }
        }

        fsLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, uv_buf);
    }
    else
    {
        fsLowLevelAPI::setTexture(0, 0, 0, false);
        fsLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw sprites
    */
    fsLowLevelAPI::setVertexPointer(sizeof(fsVec), reinterpret_cast<const r32*>(pos_buf));
    fsLowLevelAPI::setColorPointer(sizeof(fsCol), reinterpret_cast<const u8*>(col_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        fsLowLevelAPI::drawArrays(fsLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }
}


void fsSprt::render_shader(const fsMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    static const fsID s_shader_id[] =
    {
        fsDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID, fsDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID, fsDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID, //
        fsDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID, fsDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID, fsDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID
    };

    fsTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    fsShd* shd = fsDrawMgr::getShader(tex ? s_shader_id[tex->m_format.getType()] : fsDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);

    if (shd->isValid())
    {
        fsLowLevelAPI::setShader(shd->getShdObj());
    }
    else
    {
        render_soft(view);
        return;
    }

    /*
        alloc buffer
    */
    u32 pos_size = sizeof(fsVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(fsCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(fsMemHelper::allocTempBufferForEngine(pos_size + col_size + uv_size));

    fsVec* pos_buf = reinterpret_cast<fsVec*>(buf);
    buf += pos_size;

    fsCol* col_buf = reinterpret_cast<fsCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    fsVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    fsVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    fsVec* pos_ptr = pos_buf;
    fsCol* col_ptr = col_buf;
    r32* uv_ptr = uv_buf;

    u16 count = 0;

    for (s32 i = 0; i < m_cur_data_num; i++)
    {
        if (m_final_col.a == 0)
        {
            continue;
        }

        count += 4;

        SprtData* sprt_data = &m_sprt_data[i];
        fsVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = fsMath::sin_s32(-sprt_data->ang);
            r32 cos = fsMath::cos_s32(-sprt_data->ang);

            vec_w = (view_x * cos - view_y * sin) * sprt_data->w / 2.0f;
            vec_h = (view_y * cos + view_x * sin) * sprt_data->h / 2.0f;
        }

        *(pos_ptr + 0) = sprt_data->pos - vec_w + vec_h;
        *(pos_ptr + 1) = sprt_data->pos - vec_w - vec_h;
        *(pos_ptr + 2) = sprt_data->pos + vec_w - vec_h;
        *(pos_ptr + 3) = sprt_data->pos + vec_w + vec_h;

        pos_ptr += 4;

        *(col_ptr + 0) = *(col_ptr + 1) = *(col_ptr + 2) = *(col_ptr + 3) = sprt_data->col;

        col_ptr += 4;

        if (tex)
        {
            r32 u1 = sprt_data->u1;
            r32 v1 = sprt_data->v1;
            r32 u2 = sprt_data->u2;
            r32 v2 = sprt_data->v2;

            *(uv_ptr + 0) = u1;
            *(uv_ptr + 1) = v1;

            *(uv_ptr + 2) = u1;
            *(uv_ptr + 3) = v2;

            *(uv_ptr + 4) = u2;
            *(uv_ptr + 5) = v2;

            *(uv_ptr + 6) = u2;
            *(uv_ptr + 7) = v1;

            uv_ptr += 8;
        }
    }

    /*
        setup color
    */
    fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[0], m_final_col.r);
    fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[1], m_final_col.g);
    fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[2], m_final_col.b);
    fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[3], m_final_col.a);

    fsLowLevelAPI::setColorPointer(0, NULL);
    fsLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(fsCol), reinterpret_cast<const u8*>(col_buf));

    /*
        setup texture
    */
    fsLowLevelAPI::setTexCoordPointer(0, NULL);

    if (tex)
    {
        fsLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[4], tex->m_u_param_a);
        fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[5], tex->m_u_param_b);
        fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[6], tex->m_v_param_a);
        fsLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[7], tex->m_v_param_b);

        fsLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(r32) * 2, uv_buf);
    }
    else
    {
        fsLowLevelAPI::setTexture(0, 0, 0, false);
    }

    /*
        draw sprites
    */
    fsLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);

    fsLowLevelAPI::setVertexPointer(0, NULL);
    fsLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(fsVec), reinterpret_cast<const r32*>(pos_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        fsLowLevelAPI::drawArrays(fsLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }

    /*
        disable attributes
    */
    fsLowLevelAPI::disableAttribPointer(shd->m_vertex_loc);
    fsLowLevelAPI::disableAttribPointer(shd->m_color_loc);
    fsLowLevelAPI::disableAttribPointer(shd->m_texcoord_loc);

    for (s32 i = 0; i < shd->m_att_num; i++)
    {
        fsLowLevelAPI::disableAttribPointer(shd->m_att_loc_tbl[i]);
    }
}
