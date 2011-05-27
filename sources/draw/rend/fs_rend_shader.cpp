/*
 * fs_rend_shader.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"


void fsRend_Shader::init(fsPrim* prim, fsID shd_id)
{
    if (shd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsShd* shd = fsDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    fsRend::init(prim, rend_body_size, rend_data_size);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


void fsRend_Shader::init(fsPrim* prim, r32* rend_data, fsID shd_id)
{
    if (shd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsShd* shd = fsDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    fsRend::init(prim, rend_body_size, rend_data_size, rend_data);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


fsID fsRend_Shader::getShaderID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->shd->getID();
}


fsID fsRend_Shader::get2ndTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    fsTex* tex2 = rend_body->tex2;

    return tex2 ? tex2->getID() : fsID::ZERO;
}


void fsRend_Shader::set2ndTextureID(fsID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex2 = (tex_id != fsID::ZERO) ? fsDrawMgr::getTexture(tex_id) : NULL;
}


fsID fsRend_Shader::get3rdTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    fsTex* tex3 = rend_body->tex3;

    return tex3 ? tex3->getID() : fsID::ZERO;
}


r32& fsRend_Shader::uniform(u8 uni_index)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index >= rend_body->shd->getUniformNum())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return rend_body->uni[uni_index];
}


void fsRend_Shader::setUniformVec(u8 uni_index, const fsVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = vec.x;
    *(uni + 1) = vec.y;
    *(uni + 2) = vec.z;
}


void fsRend_Shader::setUniformCol3(u8 uni_index, fsCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
}


void fsRend_Shader::setUniformCol4(u8 uni_index, fsCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 3 >= rend_body->shd->getUniformNum())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
    *(uni + 3) = col.a;
}


r32& fsRend_Shader::attrib(u16 data_index, u8 att_index)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index >= att_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return rend_body->att[att_num * data_index + att_index];
}


void fsRend_Shader::setAttribVec(u16 data_index, u8 att_index, const fsVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = vec.x;
    *(att + 1) = vec.y;
    *(att + 2) = vec.z;
}


void fsRend_Shader::setAttribCol3(u16 data_index, u8 att_index, fsCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
}


void fsRend_Shader::setAttribCol4(u16 data_index, u8 att_index, fsCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 3 >= att_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
    *(att + 3) = col.a;
}


void fsRend_Shader::set3rdTextureID(fsID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex3 = (tex_id != fsID::ZERO) ? fsDrawMgr::getTexture(tex_id) : NULL;
}


fsID fsRend_Shader::getClassID()
{
    return fsID_("fsRend_Shader");
}


void fsRend_Shader::initData(void* data, u16 data_num) {}


void fsRend_Shader::render(const fsMat& view)
{
    fsPrim* prim = getPrim();

    if (!fsDrawMgr::isShaderAvailable())
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    RendBody* rend_body = getRendBody<RendBody>();
    fsShd* shd = rend_body->shd;

    if (shd->isValid())
    {
        renderSetShader(shd);
    }
    else
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    /*
        alloc buffer
    */
    fsTex* tex = renderGetPrimTextureN(prim);
    fsCol final_col = renderGetPrimFinalColor(prim);

    fsVec* pos_buf;
    fsCol* col_buf;
    r32* uv_buf;
    fsVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, (final_col != fsCol::FULL), (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    renderSetColorPointer(0, NULL);

    if (col_buf)
    {
        renderCalcColorBuffer(col_buf, prim);
        renderSetAttribPointer_color(shd, sizeof(fsCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        renderSetAttribPointer_color(shd, prim);
    }

    /*
        setup texture
    */
    renderSetTexture(tex, rend_body->tex2, rend_body->tex3, prim->isDrawFlag(fsDraw::FLAG_BILINEAR));
    renderSetTexCoordPointer(0, NULL);

    if (tex)
    {
        if (uv_buf)
        {
            renderCalcUVBuffer(uv_buf, prim);
            renderSetAttribPointer_texCoord(shd, sizeof(r32) * 2, uv_buf);
        }
        else
        {
            renderSetAttribPointer_texCoord(shd, prim);
        }
    }

    /*
        setup param
    */
    for (s32 i = 0; i < shd->getUniformNum(); i++)
    {
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, i), rend_body->uni[i]);
    }

    for (s32 i = 0; i < shd->getAttribNum(); i++)
    {
        u32 att_loc = renderGetShaderAttribLocation(shd, i);

        renderSetAttribPointer_r32(att_loc, 1, rend_body->rend_data_size, &rend_body->att[i]);
    }

    if (shd->getTextureNum() > 0)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 0), 0);
    }

    if (shd->getTextureNum() > 1)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 1), 1);
    }

    if (shd->getTextureNum() > 2)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 2), 2);
    }

    /*
        draw primitives
    */
    renderSetUniform_localToScreen(shd);

    renderSetVertexPointer(0, NULL);
    renderSetAttribPointer_vertex(shd, prim);

    renderDrawArrays(prim->getPrimMode(), 0, cur_data_num);

    /*
        disable attributes
    */
    renderDisableAttribPointers(shd);
}
