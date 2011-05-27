/*
 * fs_mdl.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "base/fs_private_macro.h"


fsMdl::fsMdl()
{
    m_draw_ptr = NULL;
    m_rend_ptr = NULL;
}


fsMdl::~fsMdl()
{
    uninit();
}


void fsMdl::init(fsID mdl_data_id, fsID scr_id)
{
    init2(mdl_data_id, scr_id, NULL);
}


void fsMdl::init(fsID mdl_data_id, fsDraw* parent)
{
    init2(mdl_data_id, fsID::ZERO, parent);
}


const fsMdlData* fsMdl::getModelData() const
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return &m_mdl_data;
}


fsID fsMdl::getTextureID() const
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_root_node.getTextureID();
}


void fsMdl::setTextureID(fsID tex_id)
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_root_node.setTextureID(tex_id);

    for (s32 i = 0; i < m_node_num; i++)
    {
        m_draw_ptr[i]->setTextureID(tex_id);
    }
}


fsID fsMdl::getLightSetID() const
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_lts_id;
}


void fsMdl::setLightSetID(fsID lts_id)
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_lts_id = lts_id;

    if (m_mdl_data.hasNormal())
    {
        if (m_lts_id == fsID::ZERO)
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setActive(false);
                    m_rend_ptr[i]->setLightSetID(fsDrawMgr::DEFAULT_LIGHT_SET_ID);
                }
            }
        }
        else
        {
            for (s32 i = 0; i < m_node_num; i++)
            {
                if (m_rend_ptr[i])
                {
                    m_rend_ptr[i]->setActive(true);
                    m_rend_ptr[i]->setLightSetID(lts_id);
                }
            }
        }
    }
}


fsDraw* fsMdl::getRootDraw()
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    return &m_root_node;
}


fsDraw* fsMdl::getNodeDraw(u16 node_index)
{
    if (!m_draw_ptr)
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (node_index >= m_node_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_draw_ptr[node_index];
}


void fsMdl::init2(fsID mdl_data_id, fsID scr_id, fsDraw* parent)
{
    uninit();

    if (mdl_data_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsRes res;

    fsTry
    {
        res = fsResMgr::getResource(mdl_data_id);
    }
    fsCatch(fsResMgr::ExceptionNotFound)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

    m_node_num = m_mdl_data.getNodeNum();
    m_lts_id = fsID::ZERO;

    if (parent)
    {
        m_root_node.init(parent);
    }
    else
    {
        m_root_node.init(scr_id);
    }

    fsNewArray(m_draw_ptr, fsDraw*, m_node_num);
    fsNewArray(m_rend_ptr, fsRend_Light*, m_node_num);

    for (s32 i = 0; i < m_node_num; i++)
    {
        if (m_mdl_data.getNodeVertNum(i) > 0)
        {
            m_draw_ptr[i] = fsNew(fsPrim);
            m_rend_ptr[i] = m_mdl_data.hasNormal() ? fsNew(fsRend_Light) : NULL;
        }
        else
        {
            m_draw_ptr[i] = fsNew(fsNode);
            m_rend_ptr[i] = NULL;
        }
    }

    for (s32 i = 0; i < m_node_num; i++)
    {
        u16 vert_num = m_mdl_data.getNodeVertNum(i);
        u16 parent_index = m_mdl_data.getNodeParentIndex(i);

        if (vert_num > 0)
        {
            fsPrim* prim = static_cast<fsPrim*>(m_draw_ptr[i]);
            fsPrim::PrimData* prim_data = const_cast<fsPrim::PrimData*>(m_mdl_data.getNodePrimData(i));

            prim->init(m_mdl_data.getNodePrimMode(i), prim_data, vert_num, (parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);
            prim->setBlendMode(m_mdl_data.getNodeBlendMode(i), true);
            prim->setDrawFlag(fsDraw::FLAG_BACKFACE_CULLING, true);

            fsVec bound_min = m_mdl_data.getNodeClipBoundMinForReader(i);
            fsVec bound_max = m_mdl_data.getNodeClipBoundMaxForReader(i);

            prim->setDrawFlag(fsDraw::FLAG_BOUND_CLIP, true);
            prim->setClipBound(bound_min, bound_max);
            prim->setSortCenter((bound_min + bound_max) / 2.0f);

            prim->local() = m_mdl_data.getNodeLocal(i);

            if (m_mdl_data.hasNormal())
            {
                fsRend_Light::RendData* rend_data = const_cast<fsRend_Light::RendData*>(reinterpret_cast<const fsRend_Light::RendData*>(m_mdl_data.getNodeNormalData(i)));

                m_rend_ptr[i]->init(prim, rend_data, fsDrawMgr::DEFAULT_LIGHT_SET_ID);
                m_rend_ptr[i]->setActive(false);
            }
        }
        else
        {
            fsNode* node = static_cast<fsNode*>(m_draw_ptr[i]);

            node->init((parent_index == i) ? &m_root_node : m_draw_ptr[parent_index]);

            node->local() = m_mdl_data.getNodeLocal(i);

            m_rend_ptr[i] = NULL;
        }
    }

    setTextureID(m_mdl_data.getTextureID());
}


FS_DEFINE_COPY_CONSTRUCTOR(fsMdl)


FS_DEFINE_OPERATOR_EQUAL(fsMdl)


void fsMdl::uninit()
{
    if (m_draw_ptr)
    {
        for (s32 i = 0; i < m_node_num; i++)
        {
            fsDelete(m_draw_ptr[i], fsDraw);

            if (m_rend_ptr[i])
            {
                fsDelete(m_rend_ptr[i], fsRend_Light);
            }
        }

        typedef fsDraw* fsDrawPtr;
        fsDeleteArray(m_draw_ptr, fsDrawPtr);

        typedef fsRend* fsRendPtr;
        fsDeleteArray(m_rend_ptr, fsRendPtr);

        m_draw_ptr = NULL;
        m_rend_ptr = NULL;
    }
}
