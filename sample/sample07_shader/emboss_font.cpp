/*
 * emboss.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


static const char s_vert_code[] = //
    "uniform mat4 fs_local_to_screen;" //
    "" //
    "attribute vec4 fs_vertex;" //
    "attribute vec4 fs_color;" //
    "attribute vec2 fs_texcoord;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main(void)" //
    "{" //
    "    gl_Position = fs_local_to_screen * fs_vertex;" //
    "" //
    "    vary_color = fs_color;" //
    "    vary_texcoord = fs_texcoord;" //
    "}";


static const char s_frag_code[] = //
    "uniform float fs_uni_00, fs_uni_01;" // texture size
    "uniform float fs_uni_02, fs_uni_03;" // texcoord offset
    "" //
    "uniform sampler2D fs_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    const vec2 uv1 = vary_texcoord;" //
    "    const vec2 uv2 = uv1 + vec2(fs_uni_02 / fs_uni_00, fs_uni_03 / fs_uni_01);" //
    "" //
    "    gl_FragColor.rgb = vary_color.rgb;" //
    "    gl_FragColor.a = (texture2D(fs_tex_00, uv1).a - texture2D(fs_tex_00, uv2).a) * vary_color.a;" //
    "}";


class EmbossFont : public fsTask
{
public:
    EmbossFont();
    virtual ~EmbossFont();

private:
    virtual void onUpdate();

    fsSprt m_bg_sprt;

    fsFont m_font;
    fsPrim m_font_prim;
    fsRend_Shader m_font_rend;

    bool m_is_uv_rot;
    s32 m_uv_rot;
};


void newEmbossFont()
{
    fsNewTask(EmbossFont);
}


EmbossFont::EmbossFont() : fsTask(ORDER_ZERO)
{
    fsDrawMgr::newShader(fsID_("emboss.shd"), s_vert_code, s_frag_code, 4, 0, 1);

    m_bg_sprt.init(1, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_bg_sprt.setTextureID(fsID_("bg_512x512.png"));
    m_bg_sprt.setDataSize(0, 640.0f, 480.0f);

    fsDrawMgr::setFont(fsID_("stonsans.ttf"), 0);
    fsDrawMgr::setFontSize(49);

    m_font.init(512, 64);
    r32 draw_width = m_font.drawString(0, 0, fsDrawMgr::isShaderAvailable() ? "Furseal Shader Sample" : "Shader Not Available");

    m_font_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_font_prim.setTextureID(m_font.getTextureID());
    m_font_prim.setBlendMode(fsDraw::BLEND_HALF, true);
    m_font_prim.setDataRect(0, fsVec::ZERO, 512.0f, 64.0f, fsCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
    m_font_prim.local().trans.set((512.0f - draw_width) / 2.0f, -7.0f, 10.0f);

    m_font_rend.init(&m_font_prim, fsID_("emboss.shd"));
    m_font_rend.uniform(0) = 512.0f;
    m_font_rend.uniform(1) = 64.0f;
    m_font_rend.uniform(2) = 0.0f;
    m_font_rend.uniform(3) = 0.0f;

    m_is_uv_rot = true;
    m_uv_rot = 0;
}


EmbossFont::~EmbossFont()
{
    fsDrawMgr::deleteShader(fsID_("emboss.shd"));
}


void EmbossFont::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_F))
    {
        fsMgr::toggleFullScreen(640, 480);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_U))
    {
        m_is_uv_rot = !m_is_uv_rot;
    }

    if (m_is_uv_rot)
    {
        m_uv_rot += 2;

        if (m_uv_rot > 360)
        {
            m_uv_rot -= 360;
        }
    }

    m_font_rend.uniform(2) = fsMath::cos_s32(m_uv_rot) * 2.0f;
    m_font_rend.uniform(3) = fsMath::sin_s32(m_uv_rot) * 1.0f;
}
