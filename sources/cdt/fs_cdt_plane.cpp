/*
 * fs_cdt_plane.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


fsCdt::Plane::Plane()
{
	m_normal=fsVec::ZERO;
	m_offset=0.0f;
}

const fsVec& fsCdt::Plane::getNormal() const
{
	return m_normal;
}

const r32& fsCdt::Plane::getOffset() const
{
	return m_offset;
}

void fsCdt::Plane::setPos(const fsVec& normal,const r32& offset)
{
	r32 scale=1.0f/normal.length();
	m_normal=normal*scale;
	m_offset=offset*scale;
}

void fsCdt::Plane::setPos(const fsVec& normal,const fsVec& positision)
{
	r32 scale=1.0f/normal.length();
	m_normal=normal*scale;
	m_offset=normal.dot(positision)*scale;
}

bool fsCdt::collide(CdtInfo* cdt_info, const Plane& plane, const Sph& sph)
{
	r32 distance = sph.m_pos.dot(plane.m_normal)-plane.m_offset;
	if (distance + fsMath::EPSILON > sph.m_radius)
      return false;

	//r32 scale=0.5f*(distance+sph.m_radius);

	if (cdt_info)
    {
		cdt_info->bafs_dir = plane.m_normal;
        cdt_info->bafs_dist = sph.m_radius-distance;
        cdt_info->pos =sph.m_pos-cdt_info->bafs_dir *(sph.m_radius - cdt_info->bafs_dist / 2.0f);
    }

	return true;
}
