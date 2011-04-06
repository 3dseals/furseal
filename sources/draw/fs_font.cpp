/*
 * fs_font.cpp
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include <stdarg.h>

#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "fs_base_all.h"


fsFont::fsFont()
{
    m_tex = NULL;
}


fsFont::~fsFont()
{
    if (fsDrawMgr::isCreated() && m_tex)
    {
        fsDrawMgr::deleteTexture(m_tex->getID());
    }
}


void fsFont::init(u16 width, u16 height)
{
    if (m_tex)
    {
        fsTry
        {
            m_tex->resizeImage(width, height);
        }
        fsCatch(fsTex::ExceptionInvalidArgument)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }
    else
    {
        fsTry
        {
            m_tex = fsDrawMgr::newTexture(fsID::genID(), width, height, fsTex::FORMAT_ALPHA);
        }
        fsCatch(fsDrawMgr::ExceptionInvalidArgument)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }

    m_tex->clearImage(fsCol::ZERO);
}


fsID fsFont::getTextureID() const
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->getID();
}


u16 fsFont::getWidth() const
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->getWidth();
}


u16 fsFont::getHeight() const
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->getHeight();
}


const void* fsFont::getImage() const
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->getImage();
}


u32 fsFont::getImageSize() const
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->getImageSize();
}


void* fsFont::beginEditImage()
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->beginEditImage();
}


void fsFont::endEditImage()
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->endEditImage();
}


void fsFont::endEditImage(u16 x, u16 y, u16 width, u16 height)
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->endEditImage(x, y, width, height);
}


void fsFont::clearImage()
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex->clearImage(fsCol::ZERO);
}


u16 fsFont::drawString(s16 x, s16 y, const char* str, ...)
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDrawMgr* ins = fsDrawMgr::instance();

    if (!ins->m_font_info)
    {
        fsThrow(ExceptionInvalidCall);
    }

    char buf1[256];
    FS_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    fsUtil::charToWchar(buf2, 256, buf1);

    s32 draw_width = fsLowLevelAPI::drawFreeTypeFont(m_tex->beginEditImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf2);
    m_tex->endEditImage();

    if (draw_width < 0)
    {
        fsThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
}


u16 fsFont::drawString(s16 x, s16 y, const wchar_t* str, ...)
{
    if (!m_tex)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDrawMgr* ins = fsDrawMgr::instance();

    if (!ins->m_font_info)
    {
        fsThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    FS_VSWPRINTF(buf, 256, str);

    s32 draw_width = fsLowLevelAPI::drawFreeTypeFont(m_tex->beginEditImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf);
    m_tex->endEditImage();

    if (draw_width < 0)
    {
        fsThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
}
