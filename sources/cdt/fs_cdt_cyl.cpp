/*
 * fs_cdt_cyl.cpp
 *
 *  Created on: 2011-8-30
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


fsCdt::Cyl::Cyl()
{
	m_world = fsMat::UNIT;
    m_radius = 0.0f;
    m_half_height = 0.0f;

    updateAABB();
}


const fsCdt::AABB& fsCdt::Cyl::getAABB() const
{
    return m_aabb;
}


const fsMat& fsCdt::Cyl::getWorld() const
{
    return m_world;
}


void fsCdt::Cyl::setWorld(const fsMat& world)
{
	m_world = world;

    updateAABB();
}


r32 fsCdt::Cyl::getRadius() const
{
    return m_radius;
}


void fsCdt::Cyl::setRadius(r32 radius)
{
    if (radius < 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_radius = radius;

    updateAABB();
}


r32 fsCdt::Cyl::getHalfHeight() const
{
    return m_half_height;
}


void fsCdt::Cyl::setHalfHeight(r32 halfheight)
{
    if (halfheight < 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_half_height = halfheight;

    updateAABB();
}


void fsCdt::Cyl::updateAABB()
{
    fsVec half_diag( //
        fsMath::abs(m_world.x_axis.x) * m_radius + fsMath::abs(m_world.y_axis.x) * m_half_height + fsMath::abs(m_world.z_axis.x) * m_radius, //
        fsMath::abs(m_world.x_axis.y) * m_radius + fsMath::abs(m_world.y_axis.y) * m_half_height + fsMath::abs(m_world.z_axis.y) * m_radius, //
        fsMath::abs(m_world.x_axis.z) * m_radius + fsMath::abs(m_world.y_axis.z) * m_half_height + fsMath::abs(m_world.z_axis.z) * m_radius);

    m_aabb.setBound(m_world.trans - half_diag, m_world.trans + half_diag);
}


bool fsCdt::collide(CdtInfo* cdt_info, const Cyl& cyl1, const Cyl& cyl2)
{
    if (!checkTouch(cyl1.m_aabb, cyl2.m_aabb))
    {
        return false;
    }

//    fsVec diff = cyl1.m_world.trans - cyl2.m_world.trans;
//    r32 sq_dist = diff.sqLength();
//    r32 y1_dist = diff.dot(cyl1.m_world.y_axis);
//    r32 y2_dist = diff.dot(cyl2.m_world.y_axis);
//
//    if (y1_dist > cyl1.m_half_height)
//	{
//		// the cylinder2 is above the cylinder1
//        if (y2_dist > cyl2.m_half_height)
//    	{
//    		// the cylinder1 is above the cylinder2
//        	fsVec diff_aa = (cyl1.m_world.trans + cyl1.m_half_height) - (cyl2.m_world.trans + cyl2.m_half_height);
//        	r32 aa_h_dist = diff_aa.dot(cyl1.m_world.y_axis);
//
//        	if(aa_h_dist > cyl2.m_radius)
//        	{
//        		return false;
//        	}
//
//    		r32 aa_sq_dist = diff_aa.sqLength();
//    		r32 aa_w_dist = fsMath::sqrt(aa_sq_dist - aa_h_dist * aa_h_dist);
//
//    		if(aa_sq_dist + cyl1.m_radius * cyl1.m_radius - 2 * cyl1.m_radius * aa_w_dist > (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON))
//    		{
//    			return false;
//    		}
//
//    		if (cdt_info)
//    		{
//    			r32 dist = fsMath::sqrt(sq_dist);
//
//    			cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//    			cdt_info->bafs_dist = (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON) - aa_sq_dist - cyl1.m_radius * cyl1.m_radius + 2 * cyl1.m_radius * aa_w_dist;
//    			cdt_info->pos = (cyl1.m_world.trans + cyl1.m_half_height + diff_aa.x * cyl1.m_world.x_axis + diff_aa.z * cyl1.m_world.z_axis) - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
//    		}
//
//    	}
//    	else if (y2_dist < -cyl2.m_half_height)
//    	{
//    		// the cylinder1 is below the cylinder2
//        	fsVec diff_ab = (cyl1.m_world.trans + cyl1.m_half_height) - (cyl2.m_world.trans - cyl2.m_half_height);
//        	r32 ab_h_dist = diff_ab.dot(cyl1.m_world.y_axis);
//
//        	if(ab_h_dist > cyl2.m_radius)
//        	{
//        		return false;
//        	}
//
//    		r32 ab_sq_dist = diff_ab.sqLength();
//    		r32 ab_w_dist = fsMath::sqrt(ab_sq_dist - ab_h_dist * ab_h_dist);
//
//    		if(ab_sq_dist + cyl1.m_radius * cyl1.m_radius - 2 * cyl1.m_radius * ab_w_dist > (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON))
//    		{
//    			return false;
//    		}
//
//    		if (cdt_info)
//    		{
//    			r32 dist = fsMath::sqrt(sq_dist);
//
//    			cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//    			cdt_info->bafs_dist = (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON) - ab_sq_dist - cyl1.m_radius * cyl1.m_radius + 2 * cyl1.m_radius * ab_w_dist;
//    			cdt_info->pos = (cyl1.m_world.trans + cyl1.m_half_height + diff_ab.x * cyl1.m_world.x_axis + diff_ab.z * cyl1.m_world.z_axis) - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
//    		}
//    	}
//    	else
//    	{
//    		// the cylinder1 is in the middle of the cylinder2
//    		fsVec diff_am = (cyl1.m_world.trans + cyl1.m_half_height) - cyl2.m_world.trans;
//    		r32 am_sq_dist = diff_am.sqLength();
//    		r32 am_h_dist = diff_am.dot(cyl2.m_world.y_axis);
//    		r32 am_w_dist = fsMath::sqrt(am_sq_dist - am_h_dist * am_h_dist);
//
//			if (am_w_dist - cyl1.m_radius < (sum_rad - fsMath::EPSILON) * (sum_rad - fsMath::EPSILON))
//			{
//				return false;
//			}
//
//			if (cdt_info)
//			{
//				r32 dist = fsMath::sqrt(sq_dist);
//
//				cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//				cdt_info->bafs_dist = sum_rad - dist;
//				cdt_info->pos = cyl1.m_world.trans - cdt_info->bafs_dir * (cyl2.m_world.trans - cdt_info->bafs_dist / 2.0f);
//			}
//    	}
//	}
//	else if (y1_dist < -cyl1.m_half_height)
//	{
//		// the cylinder2 is below the cylinder1
//	    if (y2_dist > cyl2.m_half_height)
//		{
//			// the cylinder1 is above the cylinder2
//        	fsVec diff_ba = (cyl1.m_world.trans - cyl1.m_half_height) - (cyl2.m_world.trans + cyl2.m_half_height);
//        	r32 ba_h_dist = diff_ba.dot(cyl1.m_world.y_axis);
//
//        	if(ba_h_dist > cyl2.m_radius)
//        	{
//        		return false;
//        	}
//
//    		r32 ba_sq_dist = diff_ba.sqLength();
//    		r32 ba_w_dist = fsMath::sqrt(ba_sq_dist - ba_h_dist * ba_h_dist);
//
//    		if(ba_sq_dist + cyl1.m_radius * cyl1.m_radius - 2 * cyl1.m_radius * ba_w_dist > (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON))
//    		{
//    			return false;
//    		}
//
//    		if (cdt_info)
//    		{
//    			r32 dist = fsMath::sqrt(sq_dist);
//
//    			cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//    			cdt_info->bafs_dist = (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON) - ba_sq_dist - cyl1.m_radius * cyl1.m_radius + 2 * cyl1.m_radius * ba_w_dist;
//    			cdt_info->pos = (cyl1.m_world.trans - cyl1.m_half_height + diff_ba.x * cyl1.m_world.x_axis + diff_ba.z * cyl1.m_world.z_axis) - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
//    		}
//
//		}
//		else if (y2_dist < -cyl2.m_half_height)
//		{
//			// the cylinder1 is below the cylinder2
//        	fsVec diff_bb = (cyl1.m_world.trans - cyl1.m_half_height) - (cyl2.m_world.trans - cyl2.m_half_height);
//        	r32 bb_h_dist = diff_bb.dot(cyl1.m_world.y_axis);
//
//        	if(bb_h_dist > cyl2.m_radius)
//        	{
//        		return false;
//        	}
//
//    		r32 bb_sq_dist = diff_bb.sqLength();
//    		r32 bb_w_dist = fsMath::sqrt(bb_sq_dist - bb_h_dist * bb_h_dist);
//
//    		if(bb_sq_dist + cyl1.m_radius * cyl1.m_radius - 2 * cyl1.m_radius * bb_w_dist > (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON))
//    		{
//    			return false;
//    		}
//
//    		if (cdt_info)
//    		{
//    			r32 dist = fsMath::sqrt(sq_dist);
//
//    			cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//    			cdt_info->bafs_dist = (cyl2.m_radius - fsMath::EPSILON) * (cyl2.m_radius - fsMath::EPSILON) - bb_sq_dist - cyl1.m_radius * cyl1.m_radius + 2 * cyl1.m_radius * bb_w_dist;
//    			cdt_info->pos = (cyl1.m_world.trans - cyl1.m_half_height + diff_bb.x * cyl1.m_world.x_axis + diff_bb.z * cyl1.m_world.z_axis) - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
//    		}
//		}
//		else
//		{
//			// the cylinder1 is in the middle of the cylinder2
//			r32 sum_rad = cyl1.m_radius + cyl2.m_radius;
//
//			if (sq_dist > (sum_rad - fsMath::EPSILON) * (sum_rad - fsMath::EPSILON))
//			{
//				return false;
//			}
//
//			if (cdt_info)
//			{
//				r32 dist = fsMath::sqrt(sq_dist);
//
//				cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//				cdt_info->bafs_dist = sum_rad - dist;
//				cdt_info->pos = cyl1.m_world.trans - cdt_info->bafs_dir * (cyl2.m_world.trans - cdt_info->bafs_dist / 2.0f);
//			}
//		}
//	}
//	else
//	{
//		// the cylinder2 is in the middle of the cylinder1
//	    if (y2_dist > cyl2.m_half_height)
//		{
//			// the cylinder1 is above the cylinder2
//			r32 h_diff = y_dist - cyl.m_half_height;
//
//			if(h_diff > sph.m_radius)
//			{
//				return false;
//			}
//
//			r32 h_radius = fsMath::sqrt(sph.m_radius * sph.m_radius - h_diff * h_diff);
//
//			if(sq_dist - y_dist * y_dist > (h_radius + cyl.m_radius  - fsMath::EPSILON) * (h_radius + cyl.m_radius - fsMath::EPSILON))
//			{
//				return false;
//			}
//
//			if (cdt_info)
//			{
//				r32 dist = fsMath::sqrt(sq_dist);
//
//				cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//				cdt_info->bafs_dist = sph.m_radius - h_diff;
//				cdt_info->pos = sph.m_pos - cdt_info->bafs_dir * (sph.m_radius - cdt_info->bafs_dist / 2.0f);
//			}
//
//		}
//		else if (y2_dist < -cyl2.m_half_height)
//		{
//			// the cylinder1 is below the cylinder2
//			r32 h_diff = y_dist + cyl.m_half_height;
//
//			if(h_diff > sph.m_radius)
//			{
//				return false;
//			}
//
//			r32 h_radius = fsMath::sqrt(sph.m_radius * sph.m_radius - h_diff * h_diff);
//
//			if(sq_dist - y_dist * y_dist > (h_radius + cyl.m_radius - fsMath::EPSILON) * (h_radius + cyl.m_radius - fsMath::EPSILON))
//			{
//				return false;
//			}
//
//			if (cdt_info)
//			{
//				r32 dist = fsMath::sqrt(sq_dist);
//
//				cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//				cdt_info->bafs_dist = sph.m_radius - h_diff;
//				cdt_info->pos = sph.m_pos - cdt_info->bafs_dir * (sph.m_radius - cdt_info->bafs_dist / 2.0f);
//			}
//		}
//		else
//		{
//			// the cylinder1 is in the middle of the cylinder2
//			r32 sum_rad = cyl1.m_radius + cyl2.m_radius;
//			r32 h_diff = diff.dot(cyl1.m_world.y_axis);
//
//			if (sq_dist - h_diff * h_diff > (sum_rad - fsMath::EPSILON) * (sum_rad - fsMath::EPSILON))
//			{
//				return false;
//			}
//
//			if (cdt_info)
//			{
//				r32 dist = fsMath::sqrt(sq_dist - h_diff * h_diff );
//
//				cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
//				cdt_info->bafs_dist = sum_rad - dist;
//				cdt_info->pos = cyl1.m_world.trans - cdt_info->bafs_dir * (cyl2.m_world.trans - cdt_info->bafs_dist / 2.0f);
//			}
//		}
//	}

    return true;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Cyl& cyl, const Sph& sph)
{
    bool res = collide(cdt_info, sph, cyl);

    if (cdt_info)
    {
        cdt_info->bafs_dir = -cdt_info->bafs_dir;
    }

    return res;
}

bool fsCdt::collide(CdtInfo* cdt_info, const Cyl& cyl, const Box& box)
{
    if (!checkTouch(cyl.m_aabb, box.m_aabb))
    {
        return false;
    }

    return true;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Cyl& cyl, const Tri& tri)
{
    if (!checkTouch(cyl.m_aabb, tri.m_aabb))
    {
        return false;
    }

    return true;
}
