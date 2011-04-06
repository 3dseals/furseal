/*
 * fs_shd.cpp
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_kernel_all.h"
#include "fs_res_all.h"
#include "fs_base_all.h"


fsShd* fsShd::getPrevN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_shd_map.getPrevKeyN(m_id);

    return id ? *ins->m_shd_map.get(*id) : NULL;
}


fsShd* fsShd::getNextN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_shd_map.getNextKeyN(m_id);

    return id ? *ins->m_shd_map.get(*id) : NULL;
}


fsID fsShd::getID() const
{
    return m_id;
}


u8 fsShd::getUniformNum() const
{
    return m_uni_num;
}


u8 fsShd::getAttribNum() const
{
    return m_att_num;
}


u8 fsShd::getTextureNum() const
{
    return m_tex_num;
}


bool fsShd::isValid() const
{
    return m_flag.isOff(FLAG_COMPILE_ERROR);
}


fsShd::fsShd(fsID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
{
    m_id = shd_id;
    m_vert_code = NULL;
    m_frag_code = NULL;
    m_uni_num = uni_num;
    m_att_num = att_num;
    m_tex_num = tex_num;
    m_shd_obj = 0;
    m_local_to_screen_loc = 0;
    m_vertex_loc = 0;
    m_color_loc = 0;
    m_texcoord_loc = 0;
    m_uni_loc_tbl = NULL;
    m_att_loc_tbl = NULL;

    m_flag.clear();

    if (fsDrawMgr::isShaderAvailable())
    {
        if (vert_code)
        {
            u32 vert_code_size = fsUtil::strlen(vert_code) + 1;

            m_vert_code = static_cast<char*>(fsMalloc(vert_code_size));
            fsMemHelper::memcpy(m_vert_code, vert_code, vert_code_size);
        }

        if (frag_code)
        {
            u32 frag_code_size = fsUtil::strlen(frag_code) + 1;

            m_frag_code = static_cast<char*>(fsMalloc(frag_code_size));
            fsMemHelper::memcpy(m_frag_code, frag_code, frag_code_size);
        }

        if (m_uni_num > 0)
        {
            fsNewArray(m_uni_loc_tbl, u32, m_uni_num);
        }

        if (m_att_num > 0)
        {
            fsNewArray(m_att_loc_tbl, u32, m_att_num);
        }
    }

    m_flag.setOn(FLAG_UPLOAD);
    getShdObj(); // upload the shader code to the vram

    fsDrawMgr::instance()->m_shd_map.add(shd_id, this);
}


fsShd::~fsShd()
{
    fsDrawMgr::instance()->m_shd_map.remove(m_id);

    if (m_shd_obj != 0)
    {
        fsLowLevelAPI::unregisterShader(m_shd_obj);
    }

    if (m_vert_code)
    {
        fsFree(m_vert_code);
    }

    if (m_frag_code)
    {
        fsFree(m_frag_code);
    }

    if (m_uni_loc_tbl)
    {
        fsDeleteArray(m_uni_loc_tbl, u32);
    }

    if (m_att_loc_tbl)
    {
        fsDeleteArray(m_att_loc_tbl, u32);
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsShd)


u32 fsShd::getShdObj()
{
    if (m_flag.isOn(FLAG_UPLOAD))
    {
        m_shd_obj = fsLowLevelAPI::registerShader(m_vert_code, m_frag_code);

        if (m_shd_obj == 0)
        {
            m_flag.setOn(fsShd::FLAG_COMPILE_ERROR);
        }
        else
        {
            m_local_to_screen_loc = fsLowLevelAPI::getUniformLocation(m_shd_obj, "fs_local_to_screen");
            m_vertex_loc = fsLowLevelAPI::getAttribLocation(m_shd_obj, "fs_vertex");
            m_color_loc = fsLowLevelAPI::getAttribLocation(m_shd_obj, "fs_color");
            m_texcoord_loc = fsLowLevelAPI::getAttribLocation(m_shd_obj, "fs_texcoord");

            char buf[16];

            for (s32 i = 0; i < m_uni_num; i++)
            {
                fsMgr::sprintf(buf, 16, "fs_uni_%02d", i);
                m_uni_loc_tbl[i] = fsLowLevelAPI::getUniformLocation(m_shd_obj, buf);
            }

            for (s32 i = 0; i < m_att_num; i++)
            {
                fsMgr::sprintf(buf, 16, "fs_att_%02d", i);
                m_att_loc_tbl[i] = fsLowLevelAPI::getAttribLocation(m_shd_obj, buf);
            }

            for (s32 i = 0; i < m_tex_num; i++)
            {
                fsMgr::sprintf(buf, 16, "fs_tex_%02d", i);
                m_tex_loc_tbl[i] = fsLowLevelAPI::getUniformLocation(m_shd_obj, buf);
            }
        }

        m_flag.setOff(FLAG_UPLOAD);
    }

    return m_shd_obj;
}
