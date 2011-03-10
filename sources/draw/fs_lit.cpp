/*
 * fs_lit.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"
#include "base/fs_private_macro.h"


bool fsLit::isActive() const
{
    return m_is_active.getType();
}


void fsLit::setActive(bool is_active)
{
    m_is_active = is_active;
}


const fsVec& fsLit::getPos() const
{
    return m_pos;
}


void fsLit::setPos(const fsVec& pos)
{
    m_pos = pos;

    updateBound();
}


r32 fsLit::getInnerRadius() const
{
    return m_inner_rad;
}


r32 fsLit::getOuterRadius() const
{
    return m_outer_rad;
}


void fsLit::setRadius(r32 inner_rad, r32 outer_rad)
{
    if (inner_rad < 0.0f || outer_rad < 0.0 || inner_rad > outer_rad)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_inner_rad = inner_rad;
    m_outer_rad = outer_rad;

    m_sq_inner_rad = m_inner_rad * m_inner_rad;
    m_sq_outer_rad = m_outer_rad * m_outer_rad;

    updateBound();
}


fsCol fsLit::getColor() const
{
    return m_col;
}


void fsLit::setColor(fsCol col)
{
    m_col = col;
    m_col_int = m_col.r + m_col.g + m_col.b;
}


fsLit::fsLit(fsID lts_id)
{
    m_id = lts_id;

    m_item.init(this);

    setActive(true);
    setPos(fsVec::ZERO);
    setRadius(0.0f, 0.0f);
    setColor(fsCol(255, 255, 0));
}


FS_DEFINE_OPERATOR_EQUAL(fsLit)


void fsLit::updateBound()
{
    m_max_bound.set(m_pos.x + m_outer_rad, m_pos.y + m_outer_rad, m_pos.z + m_outer_rad);
    m_min_bound.set(m_pos.x - m_outer_rad, m_pos.y - m_outer_rad, m_pos.z - m_outer_rad);
}
