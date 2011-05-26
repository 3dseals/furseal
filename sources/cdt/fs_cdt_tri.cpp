/*
 * fs_cdt_tri.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


fsCdt::Tri::Tri()
{
    m_pos1 = m_pos2 = m_pos3 = fsVec::ZERO;

    updateAABB();
}


const fsCdt::AABB& fsCdt::Tri::getAABB() const
{
    return m_aabb;
}


const fsVec& fsCdt::Tri::getPos1() const
{
    return m_pos1;
}


const fsVec& fsCdt::Tri::getPos2() const
{
    return m_pos2;
}


const fsVec& fsCdt::Tri::getPos3() const
{
    return m_pos3;
}


void fsCdt::Tri::setPos(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3)
{
    m_pos1 = pos1;
    m_pos2 = pos2;
    m_pos3 = pos3;

    updateAABB();
}


void fsCdt::Tri::updateAABB()
{
#define MIN3(a, b, c) ((a < b && a < c) ? a : ((b < c) ? b : c))
#define MAX3(a, b, c) ((a > b && a > c) ? a : ((b > c) ? b : c))

    m_aabb.setBound( //
        fsVec(MIN3(m_pos1.x, m_pos2.x, m_pos3.x), MIN3(m_pos1.y, m_pos2.y, m_pos3.y), MIN3(m_pos1.z, m_pos2.z, m_pos3.z)), //
        fsVec(MAX3(m_pos1.x, m_pos2.x, m_pos3.x), MAX3(m_pos1.y, m_pos2.y, m_pos3.y), MAX3(m_pos1.z, m_pos2.z, m_pos3.z)));

#undef MIN3
#undef MAX3
}
