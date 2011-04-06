/*
 * fs_draw_mgr.cpp
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include <stdarg.h>


#include "fs_draw_all.h"

#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_res_all.h"
#include "fs_base_all.h"


const fsID fsDrawMgr::INVISIBLE_SCREEN_ID = fsID_("INVISIBLE_SCREEN");
const fsID fsDrawMgr::DEFAULT_3D_SCREEN_ID = fsID_("DEFAULT_3D_SCREEN");
const fsID fsDrawMgr::DEFAULT_2D_SCREEN_ID = fsID_("DEFAULT_2D_SCREEN");
const fsID fsDrawMgr::DEFAULT_LIGHT_SET_ID = fsID_("DEFAULT_LIGHT_SET");
const fsID fsDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID = fsID_("DEFAULT_NO_TEXTURE_SHADER");
const fsID fsDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID = fsID_("DEFAULT_RGB_TEXTURE_SHADER");
const fsID fsDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID = fsID_("DEFAULT_RGBA_TEXTURE_SHADER");
const fsID fsDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID = fsID_("DEFAULT_ALPHA_TEXTURE_SHADER");


fsDrawMgr* fsDrawMgr::m_instance = NULL;


static const char s_default_vert_code[] = //
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
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = fs_local_to_screen * fs_vertex;" //
    "" //
    "    vary_color = fs_color * vec4(fs_uni_00, fs_uni_01, fs_uni_02, fs_uni_03) / 255.0;" //
    "" //
    "    vary_texcoord.s = fs_texcoord.s * fs_uni_04 + fs_uni_05;" //
    "    vary_texcoord.t = fs_texcoord.t * fs_uni_06 + fs_uni_07;" //
    "}";

static const char s_default_no_texture_frag_code[] = //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_FragColor = vary_color;" //
    "}";

static const char s_default_rgb_texture_frag_code[] = //
    "uniform sampler2D fs_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_FragColor.rgb = texture2D(fs_tex_00, vary_texcoord.st).rgb * vary_color.rgb;" //
    "    gl_FragColor.a = vary_color.a;" //
    "}";

static const char s_default_rgba_texture_frag_code[] = //
    "uniform sampler2D fs_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_FragColor = texture2D(fs_tex_00, vary_texcoord.st) * vary_color;" //
    "}";

static const char s_default_alpha_texture_frag_code[] = //
    "uniform sampler2D fs_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_FragColor.rgb = vary_color.rgb;" //
    "    gl_FragColor.a = texture2D(fs_tex_00, vary_texcoord.st).a * vary_color.a;" //
    "}";


bool fsDrawMgr::isShaderAvailable()
{
    return fsLowLevelAPI::isShaderAvailable();
}


u16 fsDrawMgr::getMaxTextureLength()
{
    return fsLowLevelAPI::getMaxTextureLength();
}


u16 fsDrawMgr::getValidTextureLength(u16 length)
{
    return fsLowLevelAPI::getValidTextureLength(length);
}


u16 fsDrawMgr::getTexturePixelSize(fsTex::TexFormat format)
{
    switch (format)
    {
    case fsTex::FORMAT_PNG_RGB:
        format = fsTex::FORMAT_RGB;
        break;

    case fsTex::FORMAT_PNG_RGBA:
        format = fsTex::FORMAT_RGBA;
        break;

    case fsTex::FORMAT_PNG_ALPHA:
        format = fsTex::FORMAT_ALPHA;
        break;

    default:
        break;
    }

    return fsLowLevelAPI::getTexturePixelSize(static_cast<fsLowLevelAPI::TextureFormat>(format));
}


FS_DEFINE_MANAGER_IS_CREATED(fsDrawMgr)


void fsDrawMgr::createAfterRes()
{
    destroyBeforeRes();

    m_instance = fsNew(fsDrawMgr);

    newScreen(INVISIBLE_SCREEN_ID);
    newScreen(DEFAULT_3D_SCREEN_ID);
    newScreen(DEFAULT_2D_SCREEN_ID);

    fsScr* scr0d = getScreen(INVISIBLE_SCREEN_ID);
    scr0d->setActive(false);

    fsScr* scr3d = getScreen(DEFAULT_3D_SCREEN_ID);
    scr3d->setClearColor(fsCol::ZERO);

    fsScr* scr2d = getScreen(DEFAULT_2D_SCREEN_ID);
    scr2d->setClearMode(false, true);
    scr2d->setPerspective(false);

    newLightSet(DEFAULT_LIGHT_SET_ID);

    fsLowLevelAPI::resetDrawState();

    newShader(DEFAULT_NO_TEXTURE_SHADER_ID, s_default_vert_code, s_default_no_texture_frag_code, 8, 0, 0);
    newShader(DEFAULT_RGB_TEXTURE_SHADER_ID, s_default_vert_code, s_default_rgb_texture_frag_code, 8, 0, 1);
    newShader(DEFAULT_RGBA_TEXTURE_SHADER_ID, s_default_vert_code, s_default_rgba_texture_frag_code, 8, 0, 1);
    newShader(DEFAULT_ALPHA_TEXTURE_SHADER_ID, s_default_vert_code, s_default_alpha_texture_frag_code, 8, 0, 1);
}


FS_DEFINE_MANAGER_DESTROY(fsDrawMgr, BeforeRes)


bool fsDrawMgr::isRender()
{
    return instance()->m_is_render;
}


void fsDrawMgr::setRender(bool is_render)
{
    instance()->m_is_render = is_render;
}


bool fsDrawMgr::hasScreen(fsID scr_id)
{
    fsDrawMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_scr_map.getN(scr_id) ? true : false;
}


fsScr* fsDrawMgr::getScreen(fsID scr_id)
{
    fsDrawMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsScr** scr = ins->m_scr_map.getN(scr_id);

    if (!scr)
    {
        fsThrow(ExceptionNotFound);
    }

    return *scr;
}


fsScr* fsDrawMgr::newScreen(fsID scr_id)
{
    fsDrawMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_scr_map.getN(scr_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsScr)(scr_id);
}


void fsDrawMgr::deleteScreen(fsID scr_id)
{
    instance();

    if (scr_id == fsID::ZERO || scr_id == INVISIBLE_SCREEN_ID)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDelete(getScreen(scr_id), fsScr);
}


fsScr* fsDrawMgr::getFirstScreenN()
{
    fsDrawMgr* ins = instance();
    const fsID* scr_id = ins->m_scr_map.getFirstKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


fsScr* fsDrawMgr::getLastScreenN()
{
    fsDrawMgr* ins = instance();
    const fsID* scr_id = ins->m_scr_map.getLastKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


bool fsDrawMgr::hasTexture(fsID tex_id)
{
    fsDrawMgr* ins = instance();

    if (tex_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_tex_map.getN(tex_id) ? true : false;
}


fsTex* fsDrawMgr::getTexture(fsID tex_id)
{
    fsDrawMgr* ins = instance();

    if (tex_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsTex** tex = ins->m_tex_map.getN(tex_id);

    if (!tex)
    {
        fsThrow(ExceptionNotFound);
    }

    return *tex;
}


fsTex* fsDrawMgr::newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format)
{
    fsDrawMgr* ins = instance();

    if (tex_id == fsID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        (format != fsTex::FORMAT_RGB && format != fsTex::FORMAT_RGBA && format != fsTex::FORMAT_ALPHA))
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsTex)(tex_id, width, height, format, fsTex::MODE_READ_WRITE, NULL, 0);
}


fsTex* fsDrawMgr::newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format, const void* image, u32 image_size)
{
    fsDrawMgr* ins = instance();

    if (tex_id == fsID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        format == fsTex::FORMAT_PNG_RGB || format == fsTex::FORMAT_PNG_RGBA || format == fsTex::FORMAT_PNG_ALPHA || //
        !image || image_size != static_cast<u32>(getTexturePixelSize(format) * width * height))
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsTex)(tex_id, width, height, format, fsTex::MODE_READ_ONLY, image, image_size);
}


void fsDrawMgr::deleteTexture(fsID tex_id)
{
    instance();

    if (tex_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsTex* tex = getTexture(tex_id);

    fsDelete(tex, fsTex);
}


fsTex* fsDrawMgr::getFirstTextureN()
{
    fsDrawMgr* ins = instance();
    const fsID* tex_id = ins->m_tex_map.getFirstKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


fsTex* fsDrawMgr::getLastTextureN()
{
    fsDrawMgr* ins = instance();
    const fsID* tex_id = ins->m_tex_map.getLastKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


u16 fsDrawMgr::getFontIndexNum(fsID font_id)
{
    instance();

    if (font_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(font_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return *res.getExInfo<u32>();
}


fsID fsDrawMgr::getFontID()
{
    return instance()->m_font_id;
}


u16 fsDrawMgr::getFontIndex()
{
    return instance()->m_font_index;
}


void fsDrawMgr::setFont(fsID font_id, u16 font_index)
{
    fsDrawMgr* ins = instance();

    if (font_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(font_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (font_index >= *res.getExInfo<u32>())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    ins->m_font_id = font_id;
    ins->m_font_index = font_index;
    ins->m_font_info = res.getExInfo<void>();
}


u16 fsDrawMgr::getFontSize()
{
    return instance()->m_font_size;
}


u16 fsDrawMgr::setFontSize(u16 font_size)
{
    fsDrawMgr* ins = instance();

    if (font_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_font_size = font_size;
}


u16 fsDrawMgr::calcFontDrawWidth(const char* str, ...)
{
    fsDrawMgr* ins = instance();

    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!ins->m_font_info)
    {
        fsThrow(ExceptionInvalidCall);
    }

    char buf1[256];
    FS_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    fsUtil::charToWchar(buf2, 256, buf1);

    s32 draw_width = fsLowLevelAPI::drawFreeTypeFont(NULL, 0, 0, ins->m_font_info, ins->m_font_index, ins->m_font_size, 0, 0, buf2);

    if (draw_width < 0)
    {
        fsThrow(ExceptionCalcFontDrawWidthFailed);
    }

    return draw_width;
}


u16 fsDrawMgr::calcFontDrawWidth(const wchar_t* str, ...)
{
    fsDrawMgr* ins = instance();

    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (!ins->m_font_info)
    {
        fsThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    FS_VSWPRINTF(buf, 256, str);

    s32 draw_width = fsLowLevelAPI::drawFreeTypeFont(NULL, 0, 0, ins->m_font_info, ins->m_font_index, ins->m_font_size, 0, 0, buf);

    if (draw_width < 0)
    {
        fsThrow(ExceptionCalcFontDrawWidthFailed);
    }

    return draw_width;
}


bool fsDrawMgr::hasShader(fsID shd_id)
{
    fsDrawMgr* ins = instance();

    if (shd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_shd_map.getN(shd_id) ? true : false;
}


fsShd* fsDrawMgr::getShader(fsID shd_id)
{
    fsDrawMgr* ins = instance();

    if (shd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsShd** shd = ins->m_shd_map.getN(shd_id);

    if (!shd)
    {
        fsThrow(ExceptionNotFound);
    }

    return *shd;
}


fsShd* fsDrawMgr::newShader(fsID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
{
    fsDrawMgr* ins = instance();

    if (shd_id == fsID::ZERO || !vert_code || !frag_code || uni_num > 100 || att_num > 100 || tex_num > 3)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_shd_map.getN(shd_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsShd)(shd_id, vert_code, frag_code, uni_num, att_num, tex_num);
}


void fsDrawMgr::deleteShader(fsID shd_id)
{
    instance();

    if (shd_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDelete(getShader(shd_id), fsShd);
}


fsShd* fsDrawMgr::getFirstShaderN()
{
    fsDrawMgr* ins = instance();
    const fsID* shd_id = ins->m_shd_map.getFirstKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


fsShd* fsDrawMgr::getLastShaderN()
{
    fsDrawMgr* ins = instance();
    const fsID* shd_id = ins->m_shd_map.getLastKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


bool fsDrawMgr::hasLightSet(fsID lts_id)
{
    fsDrawMgr* ins = instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return ins->m_lts_map.getN(lts_id) ? true : false;
}


fsLts* fsDrawMgr::getLightSet(fsID lts_id)
{
    fsDrawMgr* ins = instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLts** lts = ins->m_lts_map.getN(lts_id);

    if (!lts)
    {
        fsThrow(ExceptionNotFound);
    }

    return *lts;
}


fsLts* fsDrawMgr::newLightSet(fsID lts_id)
{
    fsDrawMgr* ins = instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_lts_map.getN(lts_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsLts)(lts_id);
}



void fsDrawMgr::deleteLightSet(fsID lts_id)
{
    instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDelete(getLightSet(lts_id), fsLts);
}


fsLts* fsDrawMgr::getFirstLightSetN()
{
    fsDrawMgr* ins = instance();
    const fsID* lts_id = ins->m_lts_map.getFirstKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


fsLts* fsDrawMgr::getLastLightSetN()
{
    fsDrawMgr* ins = instance();
    const fsID* lts_id = ins->m_lts_map.getLastKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


void fsDrawMgr::deleteAllVramObjForEngine()
{
    fsDrawMgr* ins = instance();

    for (const fsID* tex_id = ins->m_tex_map.getFirstKeyN(); tex_id; tex_id = ins->m_tex_map.getNextKeyN(*tex_id))
    {
        fsTex* tex = getTexture(*tex_id);

        if (tex->m_tex_obj != 0)
        {
            fsLowLevelAPI::unregisterTexture(tex->m_tex_obj);
            tex->m_tex_obj = 0;
        }

        tex->m_flag.setOn(fsTex::FLAG_UPLOAD);
    }

    for (const fsID* shd_id = ins->m_shd_map.getFirstKeyN(); shd_id; shd_id = ins->m_shd_map.getNextKeyN(*shd_id))
    {
        fsShd* shd = getShader(*shd_id);

        if (shd->m_shd_obj != 0)
        {
            fsLowLevelAPI::unregisterShader(shd->m_shd_obj);
            shd->m_shd_obj = 0;
        }

        shd->m_flag.setOn(fsShd::FLAG_UPLOAD);
    }
}


void fsDrawMgr::renderForEngine()
{
	fsDrawMgr* ins = instance();

	    if (!ins->m_is_render)
	    {
	        return;
	    }

	    fsLowLevelAPI::resetDrawState(); // TODO

	    for (const fsID* id = ins->m_scr_map.getFirstKeyN(); id; id = ins->m_scr_map.getNextKeyN(*id))
	    {
	        fsScr* scr = *ins->m_scr_map.get(*id);

	        if (!scr->isActive())
	        {
	            continue;
	        }

	        fsLowLevelAPI::setViewport( //
	            scr->m_left_in_framebuffer, scr->m_top_in_framebuffer, //
	            scr->m_width_in_framebuffer, scr->m_height_in_framebuffer);

	        fsLowLevelAPI::clearFramebuffer(scr->isClearColor(), scr->isClearDepth(), reinterpret_cast<const u8*>(&scr->m_clear_col));

	        scr->setupProjection();

	        fsDraw* sort_list = NULL;

	        renderScreen(scr, &sort_list, scr->m_view);

	        for (u32 i = 0; i < fsScr::GUEST_SCREEN_NUM; i++)
	        {
	            if (scr->m_guest_id[i] != fsID::ZERO)
	            {
	                renderScreen(*ins->m_scr_map.get(scr->m_guest_id[i]), &sort_list, scr->m_view);
	            }
	        }

	        if (sort_list)
	        {
	            fsDraw* dummy;
	            sortList(&sort_list, &dummy, sort_list);
	        }

	        for (fsDraw* draw = sort_list; draw; draw = draw->m_next_sort)
	        {
	            draw->setupDrawState();
	            draw->render(scr->m_view);
	        }

	        scr->copyScreenTexture();
	    }
}


fsDrawMgr::fsDrawMgr()
{
    m_is_render = true;
    m_font_id = fsID::ZERO;
    m_font_index = 0;
    m_font_size = 16;
    m_font_info = NULL;

    m_scr_map.init(SCREEN_HASH_SIZE);
    m_tex_map.init(TEXTURE_HASH_SIZE);
    m_shd_map.init(SHADER_HASH_SIZE);
    m_lts_map.init(LIGHTSET_HASH_SIZE);

    if (!fsLowLevelAPI::createFreeType())
    {
        fsThrow(ExceptionCreateFreeTypeFailed);
    }

    fsResMgr::addType("PNG", textureInitializer, textureFinalizer);
    fsResMgr::addType("TTF", fontInitializer, fontFinalizer);
    fsResMgr::addType("TTC", fontInitializer, fontFinalizer);
    fsResMgr::addType("OTF", fontInitializer, fontFinalizer);
}


fsDrawMgr::~fsDrawMgr()
{
    getScreen(INVISIBLE_SCREEN_ID)->moveLast();

    while (const fsID* scr_id = m_scr_map.getFirstKeyN())
    {
        fsDelete(getScreen(*scr_id), fsScr);
    }

    while (const fsID* tex_id = m_tex_map.getFirstKeyN())
    {
        deleteTexture(*tex_id);
    }

    while (const fsID* shd_id = m_shd_map.getFirstKeyN())
    {
        deleteShader(*shd_id);
    }

    while (const fsID* lts_id = m_lts_map.getFirstKeyN())
    {
        deleteLightSet(*lts_id);
    }

    for (const fsRes* res = fsResMgr::getFirstResourceN(); res; res = fsResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "TTF" || res->getExtension() == "TTC" || res->getExtension() == "OTF")
        {
            fontFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    if (!fsLowLevelAPI::destroyFreeType())
    {
        fsThrow(ExceptionDestroyFreeTypeFailed);
    }

    fsResMgr::removeType("PNG");
    fsResMgr::removeType("TTF");
    fsResMgr::removeType("TTC");
    fsResMgr::removeType("OTF");
}


FS_DEFINE_OPERATOR_EQUAL(fsDrawMgr)


FS_DEFINE_MANAGER_INSTANCE(fsDrawMgr)


void fsDrawMgr::renderScreen(fsScr* scr, fsDraw** sort_list, const fsMat& view)
{
    for (fsTree<fsDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        fsDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            draw->setupWorld();
            draw->setupFinalColor();
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }

    for (fsTree<fsDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        fsDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            if (draw->m_type != fsDraw::TYPE_NODE)
            {
                if (draw->m_draw_flag.isOn(fsDraw::FLAG_BOUND_CLIP) && //
                    scr->canBoundClip_noCalcProjection(draw->m_world, draw->m_bound_max, draw->m_bound_min))
                {
                    continue;
                }

                if (draw->isDrawFlag(fsDraw::FLAG_SORT))
                {
                    draw->setupSortValue(scr->view());

                    draw->m_next_sort = *sort_list;
                    *sort_list = draw;
                }
                else
                {
                    draw->setupDrawState();
                    draw->render(view);
                }
            }
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }
}


void fsDrawMgr::sortList(fsDraw** sorted_start, fsDraw** sorted_end, fsDraw* target_list)
{
    fsDraw* center = target_list;
    target_list = target_list->m_next_sort;

    if (!target_list)
    {
        center->m_next_sort = NULL;

        *sorted_start = center;
        *sorted_end = center;

        return;
    }

    fsDraw* left_list = NULL;
    fsDraw* right_list = NULL;
    fsDraw* next_sort;

    for (fsDraw* draw = target_list; draw; draw = next_sort)
    {
        next_sort = draw->m_next_sort;

        if (draw->m_sort_value <= center->m_sort_value)
        {
            draw->m_next_sort = left_list;
            left_list = draw;
        }
        else
        {
            draw->m_next_sort = right_list;
            right_list = draw;
        }
    }

    if (left_list)
    {
        fsDraw* start;
        fsDraw* end;

        sortList(&start, &end, left_list);

        *sorted_start = start;
        end->m_next_sort = center;
    }
    else
    {
        *sorted_start = center;
    }

    if (right_list)
    {
        fsDraw* start;
        fsDraw* end;

        sortList(&start, &end, right_list);

        *sorted_end = end;
        center->m_next_sort = start;
    }
    else
    {
        *sorted_end = center;
        center->m_next_sort = NULL;
    }
}


void fsDrawMgr::textureInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    fsDrawMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    u16 width, height;
    fsTex::TexFormat format;

    if (!fsUtil::readPNGInfo(&width, &height, &format, data, data_size))
    {
        fsThrow(ExceptionTextureInitializerFailed);
    }

    fsNew(fsTex)(id, width, height, format, fsTex::MODE_READ_ONLY, data, data_size);
}


void fsDrawMgr::textureFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    fsDrawMgr* ins = instance();

    if (id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsTex** tex = ins->m_tex_map.getN(id);

    if (tex && (*tex)->getMode() != fsTex::MODE_VOLATILE)
    {
        deleteTexture(id);
    }
}


void fsDrawMgr::fontInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    *exinfo = fsLowLevelAPI::newFreeTypeFont(data, data_size);

    if (!*exinfo)
    {
        fsThrow(ExceptionFontInitializerFailed);
    }
}


void fsDrawMgr::fontFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    if (!fsLowLevelAPI::deleteFreeTypeFont(exinfo))
    {
        fsThrow(ExceptionFontFinalizerFailed);
    }
}
