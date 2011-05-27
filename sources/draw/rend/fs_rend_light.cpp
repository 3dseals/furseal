/*
 * fs_rend_light.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"


static const fsID s_rend_light_shader_id = fsID_("REND_LIGHT_SHADER");


static const char s_vert_code[] = //
    "uniform mat4 fs_local_to_screen;" //
    "" //
    "attribute vec4 fs_vertex;" //
    "attribute vec4 fs_color;" //
    "attribute vec2 fs_texcoord;" //
    "" //
    "uniform float fs_uni_00, fs_uni_01, fs_uni_02, fs_uni_03;" // final color
    "" //
    "uniform float fs_uni_04, fs_uni_05;" // u param
    "uniform float fs_uni_06, fs_uni_07;" // v param
    "" //
    "uniform float fs_uni_09, fs_uni_10, fs_uni_11;" // ambient color
    "" //
    "uniform int fs_uni_12;" // light number
    "" //
    "uniform float fs_uni_13, fs_uni_14, fs_uni_15;" // light1 direction
    "uniform float fs_uni_16, fs_uni_17, fs_uni_18;" // light1 color
    "" //
    "uniform float fs_uni_19, fs_uni_20, fs_uni_21;" // light2 direction
    "uniform float fs_uni_22, fs_uni_23, fs_uni_24;" // light2 color
    "" //
    "uniform float fs_uni_25, fs_uni_26, fs_uni_27;" // light3 direction
    "uniform float fs_uni_28, fs_uni_29, fs_uni_30;" // light3 color
    "" //
    "attribute vec3 fs_att_00;" // normal
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = fs_local_to_screen * fs_vertex;" //
    "" //
    "    vary_color = vec4(fs_uni_09, fs_uni_10, fs_uni_11, 255.0) / 255.0;" //
    "" //
    "    if (fs_uni_12 >= 1)" //
    "    {" //
    "        float lumi = max(-dot(vec3(fs_uni_13, fs_uni_14, fs_uni_15), fs_att_00), 0.0);" //
    "        vary_color += vec4(fs_uni_16, fs_uni_17, fs_uni_18, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (fs_uni_12 >= 2)" //
    "    {" //
    "        float lumi = max(-dot(vec3(fs_uni_19, fs_uni_20, fs_uni_21), fs_att_00), 0.0);" //
    "        vary_color += vec4(fs_uni_22, fs_uni_23, fs_uni_24, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (fs_uni_12 >= 3)" //
    "    {" //
    "        float lumi = max(-dot(vec3(fs_uni_25, fs_uni_26, fs_uni_27), fs_att_00), 0.0);" //
    "        vary_color += vec4(fs_uni_28, fs_uni_29, fs_uni_30, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    vary_color = min(vary_color, 1.0) * fs_color;" //
    "" //
    "    vary_texcoord.s = fs_texcoord.s * fs_uni_04 + fs_uni_05;" //
    "    vary_texcoord.t = fs_texcoord.t * fs_uni_06 + fs_uni_07;" //
    "}";


static const char s_frag_code[] = //
    "uniform int fs_uni_08;" // texture format
    "" //
    "uniform sampler2D fs_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    if (fs_uni_08 == 1)" //
    "    {" //
    "        gl_FragColor.rgb = texture2D(fs_tex_00, vary_texcoord.st).rgb * vary_color.rgb;" //
    "        gl_FragColor.a = vary_color.a;" //
    "    }" //
    "    else if (fs_uni_08 == 2)" //
    "    {" //
    "        gl_FragColor = texture2D(fs_tex_00, vary_texcoord.st) * vary_color;" //
    "    }" //
    "    else if (fs_uni_08 == 3)" //
    "    {" //
    "        gl_FragColor.rgb = vary_color.rgb;" //
    "        gl_FragColor.a = texture2D(fs_tex_00, vary_texcoord.st).a * vary_color.a;" //
    "    }" //
    "    else" //
    "    {" //
    "        gl_FragColor = vary_color;" //
    "    }" //
    "}";


void fsRend_Light::init(fsPrim* prim, fsID lts_id)
{
    if (!fsDrawMgr::hasShader(s_rend_light_shader_id))
    {
        fsDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    fsRend::init(prim, sizeof(RendBody), sizeof(RendData));

    setLightSetID(lts_id);
}


void fsRend_Light::init(fsPrim* prim, RendData* rend_data, fsID lts_id)
{
    if (!fsDrawMgr::hasShader(s_rend_light_shader_id))
    {
        fsDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    fsRend::init(prim, sizeof(RendBody), sizeof(RendData), rend_data);

    setLightSetID(lts_id);
}


fsID fsRend_Light::getLightSetID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->lts->getID();
}


void fsRend_Light::setLightSetID(fsID lts_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    rend_body->lts = fsDrawMgr::getLightSet(lts_id);
}


fsVec& fsRend_Light::dataN(u16 index)
{
    RendData* rend_data = getRendData<RendData>(index);

    return rend_data->normal;
}


void fsRend_Light::calcNormalAsTriangles(bool is_smoothing)
{
    fsPrim* prim = getPrim();

    fsUtil::calcNormalAsTriangles(getRendData<fsVec>(0), reinterpret_cast<const fsPrim::PrimData*>(&prim->dataPos(0)), prim->getMaxDataNum(), is_smoothing);
}


fsID fsRend_Light::getClassID()
{
    return fsID_("fsRend_Light");
}


void fsRend_Light::initData(void* data, u16 data_num)
{
    RendData* rend_data = static_cast<RendData*>(data);

    for (s32 i = 0; i < data_num; i++)
    {
        rend_data[i].normal = fsVec::Z_UNIT;
    }
}


void fsRend_Light::render(const fsMat& view)
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


void fsRend_Light::render_soft(const fsMat& view)
{
    fsPrim* prim = getPrim();
    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        reset shader
    */
    renderSetShader(NULL);

    /*
        alloc buffer
    */
    fsTex* tex = renderGetPrimTextureN(prim);

    fsVec* pos_buf;
    fsCol* col_buf;
    r32* uv_buf;
    fsVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, true, (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    fsCol final_col = renderGetPrimFinalColor(prim);

    if (final_col == fsCol::FULL)
    {
        for (s32 i = 0; i < cur_data_num; i++)
        {
            col_buf[i] = prim->dataCol(i);
        }
    }
    else
    {
        renderCalcColorBuffer(col_buf, prim);
    }

    fsLts* lts = getRendBody<RendBody>()->lts;
    fsMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    u8 near_lit_num = lts->getNearLightNum();
    fsVec near_lit_dir[fsLts::MAX_NEAR_LIGHT_NUM];
    fsCol near_lit_col[fsLts::MAX_NEAR_LIGHT_NUM];

    for (s32 i = 0; i < near_lit_num; i++)
    {
        near_lit_dir[i] = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        near_lit_col[i] = lts->getNearLightColor(i);
    }

    fsCol amb_col = lts->getAmbientColor();
    RendData* rend_data = getRendData<RendData>(0);

    for (s32 i = 0; i < cur_data_num; i++)
    {
        fsCol lit_col = amb_col;

        for (s32 j = 0; j < near_lit_num; j++)
        {
            lit_col += near_lit_col[j] * -near_lit_dir[j].dot(rend_data[i].normal);
        }

        lit_col.a = 255;

        col_buf[i] *= lit_col;
    }

    renderSetColorPointer(sizeof(fsCol), reinterpret_cast<const u8*>(col_buf));

    /*
        setup texture
    */
    if (tex)
    {
        renderSetTexture(prim);

        if (uv_buf)
        {
            renderCalcUVBuffer(uv_buf, prim);
            renderSetTexCoordPointer(sizeof(r32) * 2, uv_buf);
        }
        else
        {
            renderSetTexCoordPointer(prim);
        }
    }
    else
    {
        renderSetTexture(NULL, NULL, NULL, false);
        renderSetTexCoordPointer(0, NULL);
    }

    /*
        draw primitives
    */
    renderSetVertexPointer(prim);
    renderDrawArrays(prim->getPrimMode(), 0, cur_data_num);
}


void fsRend_Light::render_shader(const fsMat& view)
{
    fsPrim* prim = getPrim();
    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    fsShd* shd = fsDrawMgr::getShader(s_rend_light_shader_id);

    if (shd->isValid())
    {
        renderSetShader(shd);
    }
    else
    {
        render_soft(view);
        return;
    }

    /*
        setup color
    */
    fsCol final_col = renderGetPrimFinalColor(prim);

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 0), final_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 1), final_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 2), final_col.b);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 3), final_col.a);

    fsLts* lts = getRendBody<RendBody>()->lts;
    fsCol amb_col = lts->getAmbientColor();

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 9), amb_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 10), amb_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 11), amb_col.b);

    renderSetColorPointer(0, NULL);
    renderSetAttribPointer_color(shd, prim);

    fsMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    renderSetUniform_s32(renderGetShaderUniformLocation(shd, 12), lts->getNearLightNum());

    for (s32 i = 0; i < lts->getNearLightNum(); i++)
    {
        fsVec lit_dir = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        fsCol lit_col = lts->getNearLightColor(i);

        s32 index = i * 6 + 13;

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 0), lit_dir.x);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 1), lit_dir.y);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 2), lit_dir.z);

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 3), lit_col.r);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 4), lit_col.g);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 5), lit_col.b);
    }

    RendData* rend_data = getRendData<RendData>(0);

    renderSetAttribPointer_r32(renderGetShaderAttribLocation(shd, 0), 3, sizeof(RendData), reinterpret_cast<const r32*>(&rend_data->normal));

    /*
        setup texture
    */
    renderSetTexture(prim);
    renderSetTexCoordPointer(0, NULL);

    fsTex* tex = renderGetPrimTextureN(prim);

    if (tex)
    {
        r32 u_param_a, u_param_b, v_param_a, v_param_b;
        renderGetTextureParam(&u_param_a, &u_param_b, &v_param_a, &v_param_b, tex);

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 4), u_param_a);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 5), u_param_b);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 6), v_param_a);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 7), v_param_b);

        switch (tex->getFormat())
        {
        case fsTex::FORMAT_RGB:
        case fsTex::FORMAT_PNG_RGB:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 1);
            break;

        case fsTex::FORMAT_RGBA:
        case fsTex::FORMAT_PNG_RGBA:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 2);
            break;

        case fsTex::FORMAT_ALPHA:
        case fsTex::FORMAT_PNG_ALPHA:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 3);
            break;
        }

        renderSetAttribPointer_texCoord(shd, prim);
    }
    else
    {
        renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 0);
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
    this->renderDisableAttribPointers(shd);
}
