/*
 * fs_lts.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"
#include "base/fs_private_macro.h"


fsLts* fsLts::getPrevN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_lts_map.getPrevKeyN(m_id);

    return id ? *ins->m_lts_map.get(*id) : NULL;
}


fsLts* fsLts::getNextN() const
{
    fsDrawMgr* ins = fsDrawMgr::instance();
    const fsID* id = ins->m_lts_map.getNextKeyN(m_id);

    return id ? *ins->m_lts_map.get(*id) : NULL;
}


fsID fsLts::getID() const
{
    return m_id;
}


fsCol fsLts::getAmbientColor() const
{
    return m_amb_col;
}


void fsLts::setAmbientColor(fsCol col)
{
    m_amb_col = col;
}


bool fsLts::isParaLightActive(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].is_active.getType();
}


void fsLts::setParaLightActive(u8 index, bool is_active)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_para_lit[index].is_active = is_active;
}


const fsVec& fsLts::getParaLightDir(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].dir;
}


void fsLts::setParaLightDir(u8 index, const fsVec& dir)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
    {
        m_para_lit[index].dir.set(0.0f, -1.0f, 0.0f);
    }
    else
    {
        m_para_lit[index].dir = dir.normalize();
    }
}


fsCol fsLts::getParaLightColor(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].col;
}


void fsLts::setParaLightColor(u8 index, fsCol col)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    ParaLight* para_lit = &m_para_lit[index];

    para_lit->col = col;
    para_lit->col_int = col.r + col.g + col.b;
}


u8 fsLts::getNearLightNum() const
{
    return m_near_lit_num;
}


fsLit* fsLts::getPointLight(fsID lit_id)
{
    fsLit* lit = getPointLightN(lit_id);

    if (!lit)
    {
        fsThrow(ExceptionNotFound);
    }

    return lit;
}


fsLit* fsLts::newPointLight(fsID lit_id)
{
    if (lit_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (getPointLightN(lit_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    fsLit* lit = fsNew(fsLit)(lit_id);

    m_lit_list.addLast(&lit->m_item);

    return lit;
}


void fsLts::deletePointLight(fsID lit_id)
{
    if (lit_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_near_lit_num = 0;

    fsDelete(getPointLight(lit_id), fsLit);
}


fsLit* fsLts::getFirstPointLightN() const
{
    fsList<fsLit>::Item* item = m_lit_list.getFirstN();

    return item ? item->getSelf() : NULL;
}


fsLit* fsLts::getLastPointLightN() const
{
    fsList<fsLit>::Item* item = m_lit_list.getLastN();

    return item ? item->getSelf() : NULL;
}


u32 fsLts::getPointLightNum() const
{
    return m_lit_list.getItemNum();
}


void fsLts::findNearLight(const fsVec& pos)
{
    m_near_lit_num = 0;

    for (u32 i = 0; i < MAX_PARA_LIGHT_NUM; i++)
    {
        ParaLight* para_lit = &m_para_lit[i];

        if (para_lit->is_active.getValue())
        {
            addLightToNearLight(para_lit->dir, para_lit->col, para_lit->col_int);
        }
    }

    for (fsList<fsLit>::Item* item = m_lit_list.getFirstN(); item; item = item->getNextN())
    {
        fsLit* lit = item->getSelf();

        if (!lit->m_is_active.getValue() || //
            pos.x < lit->m_min_bound.x || pos.y < lit->m_min_bound.y || pos.z < lit->m_min_bound.z || //
            pos.x > lit->m_max_bound.x || pos.y > lit->m_max_bound.y || pos.z > lit->m_max_bound.z)
        {
            continue;
        }

        fsVec diff = pos - lit->m_pos;
        r32 sq_dist = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

        if (sq_dist <= lit->m_sq_outer_rad)
        {
            r32 dist = fsMath::sqrt(sq_dist);
            fsVec dir = (dist < fsMath::EPSILON) ? fsVec(0.0f, -1.0f, 0.0f) : diff / dist;
            fsCol col = (dist <= lit->m_inner_rad) ? lit->m_col : lit->m_col * ((lit->m_outer_rad - dist) / (lit->m_outer_rad - lit->m_inner_rad));
            col.a = 255;

            addLightToNearLight(dir, col, col.r + col.g + col.b);
        }
    }
}


const fsVec& fsLts::getNearLightDir(u8 index) const
{
    if (index >= m_near_lit_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_near_lit[m_near_lit_index_tbl[index]].dir;
}


fsCol fsLts::getNearLightColor(u8 index) const
{
    if (index >= m_near_lit_num)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    return m_near_lit[m_near_lit_index_tbl[index]].col;
}
void fsLts::clearPointLight()
{
    while (m_lit_list.hasItem())
    {
        fsDelete(m_lit_list.getFirstN()->getSelf(), fsLit);
    }
}


fsLts::fsLts(fsID lts_id)
{
    m_id = lts_id;
    m_near_lit_num = 0;

    setAmbientColor(fsCol::ZERO);

    for (u32 i = 0; i < MAX_PARA_LIGHT_NUM; i++)
    {
        setParaLightActive(i, false);
        setParaLightDir(i, fsVec::X_UNIT);
        setParaLightColor(i, fsCol::ZERO);
    }

    clearPointLight();

    fsDrawMgr::instance()->m_lts_map.add(m_id, this);
}


fsLts::~fsLts()
{
    fsDrawMgr::instance()->m_lts_map.remove(m_id);

    clearPointLight();
}


FS_DEFINE_OPERATOR_EQUAL(fsLts)


fsLit* fsLts::getPointLightN(fsID lit_id)
{
    if (lit_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    for (fsList<fsLit>::Item* item = m_lit_list.getFirstN(); item; item = item->getNextN())
    {
        fsLit* lit = item->getSelf();

        if (lit->m_id == lit_id)
        {
            m_lit_list.addFirst(&lit->m_item);

            return lit;
        }
    }

    return NULL;
}

void fsLts::addLightToNearLight(const fsVec& dir, fsCol col, u16 col_int)
{
    if (m_near_lit_num == 0)
    {
        NearLight* near_lit = &m_near_lit[0];

        near_lit->dir = dir;
        near_lit->col = col;
        near_lit->col_int = col_int;

        m_near_lit_index_tbl[0] = 0;

        m_near_lit_num = 1;

        return;
    }
    else if (m_near_lit_num < MAX_NEAR_LIGHT_NUM)
    {
        NearLight* near_lit = &m_near_lit[m_near_lit_num];

        near_lit->dir = dir;
        near_lit->col = col;
        near_lit->col_int = col_int;

        m_near_lit_index_tbl[m_near_lit_num] = m_near_lit_num;

        m_near_lit_num++;
    }
    else
    {
        u8 index = m_near_lit_index_tbl[m_near_lit_num - 1];
        NearLight* near_lit = &m_near_lit[index];

        if (col_int > near_lit->col_int)
        {
            near_lit->dir = dir;
            near_lit->col = col;
            near_lit->col_int = col_int;
        }
        else
        {
            return;
        }
    }

    if (m_near_lit_num > 1)
    {
        for (s32 i = m_near_lit_num - 1; i > 0; i--)
        {
            u8 cur_index = m_near_lit_index_tbl[i];
            u8 prev_index = m_near_lit_index_tbl[i - 1];

            if (m_near_lit[cur_index].col_int > m_near_lit[prev_index].col_int)
            {
                m_near_lit_index_tbl[i] = prev_index;
                m_near_lit_index_tbl[i - 1] = cur_index;
            }
            else
            {
                return;
            }
        }
    }
}
