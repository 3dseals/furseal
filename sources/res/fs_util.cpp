/*
 * fs_util.cpp
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_res_all.h"
#include "base/fs_low_level_api.h"


u32 fsUtil::strlen(const char* str)
{
    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    u32 len = 0;

    while (*str)
    {
        str++;
        len++;
    }

    return len;
}


u32 fsUtil::wcslen(const wchar_t* str)
{
    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    u32 len = 0;

    while (*str)
    {
        str++;
        len++;
    }

    return len;
}


void fsUtil::charToWchar(wchar_t* buf, u32 buf_size, const char* str)
{
    if (!buf || buf_size == 0 || !str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    for (u32 i = 0; i < buf_size; i++)
    {
        char c = str[i];

        if (!c)
        {
            buf[i] = L'\0';
            break;
        }

        buf[i] = c;
    }

    buf[buf_size - 1] = L'\0';
}


const char* fsUtil::getBasename(const char* filename)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    const char* base = filename;

    for (const char* c = base; *c != '\0'; c++)
    {
        if (*c == '\\' || *c == '/' || *c == ':')
        {
            base = c + 1;
        }
    }

    return base;
}


const char* fsUtil::getExtension(const char* filename)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    const char* ext = getBasename(filename);

    for ( ; *ext != '\0'; ext++)
    {
        if (*ext == '.')
        {
            return ext + 1;
        }
    }

    return ext;
}


void fsUtil::calcNormalAsTriangles(fsVec* normal, const fsPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing)
{
    if (!normal || !prim_data || vert_num == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (vert_num < 3)
    {
        for (s32 i = 0; i < vert_num; i++)
        {
            normal[i] = fsVec::Z_UNIT;
        }

        return;
    }

    u16 tri_vert_num = (vert_num / 3) * 3;

    for (s32 i = 0; i < tri_vert_num; i += 3)
    {
        fsVec vec1 = prim_data[i + 1].pos - prim_data[i].pos;
        fsVec vec2 = prim_data[i + 2].pos - prim_data[i].pos;

        normal[i] = normal[i + 1] = normal[i + 2] = vec1.cross(vec2).normalize();
    }

    for (s32 i = tri_vert_num; i < vert_num; i++)
    {
        normal[i] = fsVec::Z_UNIT;
    }

    if (!is_smoothing)
    {
        return;
    }

    u32 mark_buf_size = sizeof(u16) * vert_num;
    u16* mark_buf = static_cast<u16*>(fsMalloc(mark_buf_size));

    fsMemHelper::memset(mark_buf, 0, mark_buf_size);

    for (s32 i = 0; i < vert_num; i++)
    {
        if (mark_buf[i] > 0)
        {
            continue;
        }

        u16 mark = i + 1;
        u16 mark_num = 1;

        const fsVec& pos1 = prim_data[i].pos;
        const fsVec& n1 = normal[i];

        fsVec avgn = n1;

        mark_buf[i] = mark;

        for (s32 j = i + 1; j < vert_num; j++)
        {
            const fsVec& pos2 = prim_data[j].pos;
            const fsVec& n2 = normal[j];

            if (pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z)
            {
                r32 inner = n1.dot(n2);

                if (inner > 0.7071f) // cos 45
                {
                    mark_buf[j] = mark;
                    mark_num++;

                    if (inner < 1.0f - fsMath::EPSILON)
                    {
                        avgn += n2;
                    }
                }
            }
        }

        if (mark_num > 1)
        {
            if (avgn.x == 0.0f && avgn.y == 0.0f && avgn.z == 0.0f)
            {
                continue;
            }

            avgn = avgn.normalize();

            for (s32 j = i; j < vert_num; j++)
            {
                if (mark_buf[j] == mark)
                {
                    normal[j] = avgn;
                }
            }
        }
    }

    fsFree(mark_buf);
}


bool fsUtil::readPNGInfo(u16* width, u16* height, fsTex::TexFormat* format, const void* data, u32 data_size)
{
    if (!width || !height || !format || !data || data_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLowLevelAPI::TextureFormat texture_format;

    bool res = fsLowLevelAPI::readPNGInfo(width, height, &texture_format, data, data_size);

    *format = static_cast<fsTex::TexFormat>(fsTex::FORMAT_PNG_RGB + texture_format);

    return res;
}


bool fsUtil::readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
{
    if (!buf || buf_size == 0 || buf_line_size == 0 || !data || data_size == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return fsLowLevelAPI::readPNGImage(buf, buf_size, buf_line_size, data, data_size);
}


void fsUtil::loadWindowsFont(const char* filename)
{
    if (!filename)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    fsLowLevelAPI::getWindowsFontDirectory(buf, 256);

    fsStr<char, 255> str = buf;
    str += filename;

    fsResMgr::loadResource(str.getString(), true);
}


void fsUtil::loadShader(fsID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num)
{
    fsID vert_id = fsID::genID();
    char* vert_code = NULL;

    if (vert_file)
    {
        fsResMgr::loadResourceAs(vert_id, vert_file, false);
        fsRes res = fsResMgr::getResource(vert_id);
        vert_code = static_cast<char*>(fsMalloc(res.getDataSize() + 1));

        fsMemHelper::memcpy(vert_code, res.getData<void>(), res.getDataSize());
        vert_code[res.getDataSize()] = '\0';

        fsResMgr::removeResource(vert_id);
    }

    fsID frag_id = fsID::genID();
    char* frag_code = NULL;

    if (frag_file)
    {
        fsResMgr::loadResourceAs(frag_id, frag_file, false);
        fsRes res = fsResMgr::getResource(frag_id);
        frag_code = static_cast<char*>(fsMalloc(res.getDataSize() + 1));

        fsMemHelper::memcpy(frag_code, res.getData<void>(), res.getDataSize());
        frag_code[res.getDataSize()] = '\0';

        fsResMgr::removeResource(frag_id);
    }

    fsDrawMgr::newShader(shd_id, vert_code, frag_code, uni_num, att_num, tex_num);

    if (vert_code)
    {
        fsFree(vert_code);
    }

    if (frag_code)
    {
        fsFree(frag_code);
    }
}
