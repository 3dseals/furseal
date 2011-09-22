/*
 * fs_cdt_aabb.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


const fsVec& fsCdt::AABB::getMin() const
{
    return m_min;
}


const fsVec& fsCdt::AABB::getMax() const
{
    return m_max;
}


void fsCdt::AABB::setBound(const fsVec& min, const fsVec& max)
{
    if (min.x > max.x || min.y > max.y || min.z > max.z)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_min = min;
    m_max = max;
}


bool fsCdt::checkTouch(const AABB& aabb1, const AABB& aabb2)
{
    if (aabb1.m_min.x <= aabb2.m_max.x && aabb1.m_min.y <= aabb2.m_max.y && aabb1.m_min.z <= aabb2.m_max.z && //
        aabb1.m_max.x >= aabb2.m_min.x && aabb1.m_max.y >= aabb2.m_min.y && aabb1.m_max.z >= aabb2.m_min.z)
    {
        return true;
    }
    else
    {
        return false;
    }
}
