/*
 * fs_tex.cpp
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "fs_base_all.h"


fsTex* fsTex::getPrevN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_tex_map.getPrevKeyN(m_id);

    return id ? *ins->m_tex_map.get(*id) : NULL;
}


fsTex* fsTex::getNextN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_tex_map.getNextKeyN(m_id);

    return id ? *ins->m_tex_map.get(*id) : NULL;
}


fsID fsTex::getID() const
{
    return m_id;
}


u16 fsTex::getWidth() const
{
    return m_width;
}


u16 fsTex::getHeight() const
{
    return m_height;
}


fsTex::TexFormat fsTex::getFormat() const
{
    return m_format.getType();
}


fsTex::TexMode fsTex::getMode() const
{
    return m_mode.getType();
}


const void* fsTex::getImage() const
{
    return m_image;
}


u32 fsTex::getImageSize() const
{
    return m_image_size;
}


void* fsTex::beginEditImage()
{
    if (m_mode != MODE_READ_WRITE || m_flag.isOn(fsTex::FLAG_EDIT))
    {
        fsThrow(ExceptionInvalidCall);
    }

    m_flag.setOn(fsTex::FLAG_EDIT);

    return const_cast<void*>(m_image);
}


void fsTex::endEditImage()
{
    if (m_flag.isOff(fsTex::FLAG_EDIT))
    {
        fsThrow(ExceptionInvalidCall);
    }

    m_flag.setOff(fsTex::FLAG_EDIT);
    m_flag.setOn(fsTex::FLAG_UPLOAD);
}


void fsTex::endEditImage(u16 x, u16 y, u16 width, u16 height)
{
    if (m_flag.isOff(fsTex::FLAG_EDIT))
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (width == 0 || height == 0 || x + width > m_width || y + height > m_height)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_flag.setOff(fsTex::FLAG_EDIT);

    if (m_flag.isOn(fsTex::FLAG_UPLOAD))
    {
        return;
    }

    u16 pixel_size = fsDrawMgr::getTexturePixelSize(m_format.getType());
    u32 image_line_size = m_width * pixel_size;

    u32 sub_image_line_size = width * pixel_size;
    u32 sub_image_size = sub_image_line_size * height;
    u8* sub_image = static_cast<u8*>(fsMalloc(sub_image_size));

    const u8* src = static_cast<const u8*>(m_image) + (m_width * y + x) * pixel_size;
    u8* dest = sub_image;

    for (s32 i = 0; i < height; i++)
    {
        fsMemHelper::memcpy(dest, src, sub_image_line_size);

        src += image_line_size;
        dest += sub_image_line_size;
    }

    fsLowLevelAPI::replaceSubTexture(m_tex_obj, x, y, width, height, //
        static_cast<fsLowLevelAPI::TextureFormat>(m_format.getType()), sub_image);

    fsFree(sub_image);
}


void fsTex::clearImage(fsCol col)
{
    if (m_flag.isOn(fsTex::FLAG_EDIT))
    {
        fsThrow(ExceptionInvalidCall);
    }

    u8* image = static_cast<u8*>(beginEditImage());

    switch (m_format.getType())
    {
    case FORMAT_RGB:
        {
            u32 pixel_num = m_width * m_height;
            u8* pixel = static_cast<u8*>(const_cast<void*>(m_image));

            for (u32 i = 0; i < pixel_num; i++)
            {
                *(pixel + 0) = col.r;
                *(pixel + 1) = col.g;
                *(pixel + 2) = col.b;

                pixel += 3;
            }
        }
        break;

    case FORMAT_RGBA:
        {
            u32 pixel_num = m_width * m_height;
            u8* pixel = static_cast<u8*>(const_cast<void*>(m_image));

            for (u32 i = 0; i < pixel_num; i++)
            {
                *(pixel + 0) = col.r;
                *(pixel + 1) = col.g;
                *(pixel + 2) = col.b;
                *(pixel + 3) = col.a;

                pixel += 4;
            }
        }
        break;

    case FORMAT_ALPHA:
        fsMemHelper::memset(image, col.a, m_width * m_height);
        break;

    default:
        break;
    }

    endEditImage();
}


void fsTex::resizeImage(u16 width, u16 height)
{
    if (m_mode != MODE_READ_WRITE)
    {
        fsThrow(ExceptionInvalidCall);
    }

    if (width == 0 || height == 0 || width > fsDrawMgr::getMaxTextureLength() || height > fsDrawMgr::getMaxTextureLength())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_width = width;
    m_height = height;

    u16 valid_width = fsDrawMgr::getValidTextureLength(width);
    u16 valid_height = fsDrawMgr::getValidTextureLength(height);

    if (width != valid_width || height != valid_height)
    {
        m_flag.setOn(FLAG_UV_ADJUST);

        m_u_param_a = static_cast<r32>(width) / valid_width;
        m_u_param_b = 0.0f;

        m_v_param_a = static_cast<r32>(height) / valid_height;
        m_v_param_b = 0.0f;
    }
    else
    {
        m_flag.setOff(FLAG_UV_ADJUST);

        m_u_param_a = 1.0f;
        m_u_param_b = 0.0f;

        m_v_param_a = 1.0f;
        m_v_param_b = 0.0f;
    }

    if (m_image)
    {
        fsFree(const_cast<void*>(m_image));
    }

    m_image_size = fsDrawMgr::getTexturePixelSize(m_format.getType()) * m_width * m_height;
    m_image = fsMalloc(m_image_size);

    m_flag.setOn(fsTex::FLAG_UPLOAD);
}


void fsTex::setVolatile()
{
    if (m_mode != MODE_READ_ONLY && m_mode != MODE_READ_WRITE)
    {
        fsThrow(ExceptionInvalidCall);
    }

    getTexObj(); // upload the image to the vram

    if (m_mode == MODE_READ_WRITE)
    {
        fsFree(const_cast<void*>(m_image));
        m_image = NULL;
        m_image_size = 0;
    }

    m_mode = MODE_VOLATILE;
}


fsID fsTex::getProxyTextureID() const
{
    return m_proxy_tex ? m_proxy_tex->getID() : fsID::ZERO;
}


void fsTex::setProxyTextureID(fsID tex_id)
{
    if (tex_id == m_id)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_proxy_tex = (tex_id == fsID::ZERO) ? NULL : fsDrawMgr::getTexture(tex_id);
}


fsTex::fsTex(fsID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size)
{
    m_id = tex_id;
    m_width = width;
    m_height = height;
    m_format = format;
    m_mode = mode;
    m_tex_obj = 0;
    m_proxy_tex = NULL;

    m_flag.clear();

    u16 valid_width = fsDrawMgr::getValidTextureLength(width);
    u16 valid_height = fsDrawMgr::getValidTextureLength(height);

    if (width != valid_width || height != valid_height)
    {
        m_flag.setOn(FLAG_UV_ADJUST);

        m_u_param_a = static_cast<r32>(width) / valid_width;
        m_u_param_b = 0.0f;

        m_v_param_a = static_cast<r32>(height) / valid_height;
        m_v_param_b = 0.0f;
    }
    else
    {
        m_u_param_a = 1.0f;
        m_u_param_b = 0.0f;

        m_v_param_a = 1.0f;
        m_v_param_b = 0.0f;
    }

    switch (m_mode.getType())
    {
    case MODE_READ_ONLY:
        m_image = image;
        m_image_size = image_size;
        break;

    case MODE_READ_WRITE:
        m_image_size = fsDrawMgr::getTexturePixelSize(m_format.getType()) * m_width * m_height;
        m_image = fsMalloc(m_image_size);
        break;

    case MODE_FRAMEBUFFER:
        m_image = NULL;
        m_image_size = 0;

        m_flag.setOn(FLAG_UV_ADJUST);

        m_v_param_a = -m_v_param_a;
        m_v_param_b = 1.0f;
        break;

    default:
        fsThrow(ExceptionInvalidArgument);
    }

    m_flag.setOn(fsTex::FLAG_UPLOAD);

    fsDrawMgr::instance()->m_tex_map.add(tex_id, this);
}


fsTex::~fsTex()
{
    fsDrawMgr::instance()->m_tex_map.remove(m_id);

    if (m_tex_obj != 0)
    {
        fsLowLevelAPI::unregisterTexture(m_tex_obj);
    }

    if (m_mode == MODE_READ_WRITE)
    {
        fsFree(const_cast<void*>(m_image));
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsTex)


u32 fsTex::getTexObj()
{
    if (m_flag.isOn(FLAG_EDIT))
    {
        fsThrow(ExceptionEndEditImageNotCalled);
    }

    if (m_flag.isOn(FLAG_UPLOAD))
    {
        if (m_mode == MODE_VOLATILE)
        {
            fsThrow(ExceptionVolatileTextureUploaded);
        }
        else if (m_mode != MODE_FRAMEBUFFER)
        {
            if (m_tex_obj != 0)
            {
                fsLowLevelAPI::unregisterTexture(m_tex_obj);
            }

            if (m_format == FORMAT_PNG_RGB || m_format == FORMAT_PNG_RGBA || m_format == FORMAT_PNG_ALPHA)
            {
                expandAndRegisterTexture_png();
            }
            else if (m_flag.isOn(FLAG_UV_ADJUST))
            {
                expandAndRegisterTexture_ctx();
            }
            else
            {
                m_tex_obj = fsLowLevelAPI::registerTexture( //
                    m_width, m_height, static_cast<fsLowLevelAPI::TextureFormat>(m_format.getType()), m_image);
            }
        }

        m_flag.setOff(FLAG_UPLOAD);
    }

    return m_tex_obj;
}


void fsTex::expandAndRegisterTexture_ctx()
{
    u16 valid_width = fsDrawMgr::getValidTextureLength(m_width);
    u16 valid_height = fsDrawMgr::getValidTextureLength(m_height);
    u16 pixel_size = fsDrawMgr::getTexturePixelSize(m_format.getType());
    u16 src_line_size = m_width * pixel_size;
    u16 dest_line_size = valid_width * pixel_size;

    void* new_image = fsMalloc(valid_width * valid_height * pixel_size);

    const u8* src = static_cast<const u8*>(m_image);
    u8* dest = static_cast<u8*>(new_image);

    for (u32 i = 0; i < m_height; i++)
    {
        fsMemHelper::memcpy(dest, src, src_line_size);

        src += src_line_size - pixel_size;
        dest += src_line_size;

        for (u32 j = m_width; j < valid_width; j++)
        {
            fsMemHelper::memcpy(dest, src, pixel_size);

            dest += pixel_size;
        }

        src += pixel_size;
    }

    src = dest - dest_line_size;

    for (u32 i = m_height; i < valid_height; i++)
    {
        fsMemHelper::memcpy(dest, src, dest_line_size);

        dest += dest_line_size;
    }

    m_tex_obj = fsLowLevelAPI::registerTexture( //
        valid_width, valid_height, static_cast<fsLowLevelAPI::TextureFormat>(m_format.getType()), new_image);

    fsFree(new_image);
}


void fsTex::expandAndRegisterTexture_png()
{
    u16 valid_width = fsDrawMgr::getValidTextureLength(m_width);
    u16 valid_height = fsDrawMgr::getValidTextureLength(m_height);
    u16 pixel_size = fsDrawMgr::getTexturePixelSize(m_format.getType());
    u16 src_line_size = m_width * pixel_size;
    u16 dest_line_size = valid_width * pixel_size;

    void* new_image = fsMalloc(valid_width * valid_height * pixel_size);

    fsUtil::readPNGImage(new_image, valid_width * valid_height * pixel_size, dest_line_size, m_image, m_image_size);

    u8* src = static_cast<u8*>(new_image) + src_line_size - pixel_size;
    u8* dest = src + pixel_size;

    if (m_width < valid_width)
    {
        for (u32 i = 0; i < m_height; i++)
        {
            for (u32 j = m_width; j < valid_width; j++)
            {
                fsMemHelper::memcpy(dest, src, pixel_size);

                dest += pixel_size;
            }

            src += dest_line_size;
            dest += src_line_size;
        }
    }

    src = static_cast<u8*>(new_image) + dest_line_size * (m_height - 1);
    dest = src + dest_line_size;

    for (u32 i = m_height; i < valid_height; i++)
    {
        fsMemHelper::memcpy(dest, src, dest_line_size);

        dest += dest_line_size;
    }

    m_tex_obj = fsLowLevelAPI::registerTexture( //
        valid_width, valid_height, static_cast<fsLowLevelAPI::TextureFormat>(m_format.getType() - FORMAT_PNG_RGB), new_image);

    fsFree(new_image);
}
