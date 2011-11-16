/*
 * fs_cdt_sph.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


fsCdt::Sph::Sph()
{
    m_pos = fsVec::ZERO;
    m_radius = 0.0f;

    updateAABB();
}


const fsCdt::AABB& fsCdt::Sph::getAABB() const
{
    return m_aabb;
}


const fsVec& fsCdt::Sph::getPos() const
{
    return m_pos;
}


void fsCdt::Sph::setPos(const fsVec& pos)
{
    m_pos = pos;

    updateAABB();
}


r32 fsCdt::Sph::getRadius() const
{
    return m_radius;
}


void fsCdt::Sph::setRadius(r32 radius)
{
    if (radius < 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_radius = radius;

    updateAABB();
}


void fsCdt::Sph::updateAABB()
{
    m_aabb.setBound( //
        fsVec(m_pos.x - m_radius, m_pos.y - m_radius, m_pos.z - m_radius), //
        fsVec(m_pos.x + m_radius, m_pos.y + m_radius, m_pos.z + m_radius));
}

bool fsCdt::collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2)
{
    if (!checkTouch(sph1.m_aabb, sph2.m_aabb))
    {
        return false;
    }

    fsVec diff = sph1.m_pos - sph2.m_pos;
    r32 sum_rad = sph1.m_radius + sph2.m_radius;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sum_rad - fsMath::EPSILON) * (sum_rad - fsMath::EPSILON))
    {
        return false;
    }

    r32 dist = fsMath::sqrt(sq_dist);

    if (cdt_info)
    {
        cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
        cdt_info->bafs_dist = sum_rad - dist;
        cdt_info->pos = sph1.m_pos - cdt_info->bafs_dir * (sph1.m_radius - cdt_info->bafs_dist / 2.0f);
    }

    return true;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Cyl& cyl)
{
    if (!checkTouch(sph.m_aabb, cyl.m_aabb))
    {
        return false;
    }

    fsVec diff = sph.m_pos - cyl.m_world.trans;
	r32 sq_dist = diff.sqLength();
    r32 y_dist = diff.dot(cyl.m_world.y_axis);

    if (y_dist < cyl.m_half_height || y_dist > -cyl.m_half_height)
    {
    	// the sphere is in the middle of the cylinder
    	r32 sum_rad = sph.m_radius + cyl.m_radius;
    	r32 sq_sum_dist = sq_dist - y_dist * y_dist;

        if (sq_sum_dist > (sum_rad - fsMath::EPSILON) * (sum_rad - fsMath::EPSILON))
        {
            return false;
        }

        if (cdt_info)
        {
        	r32 dist = fsMath::sqrt(sq_dist);
            r32 sum_dist = fsMath::sqrt(sq_sum_dist);

            cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
            cdt_info->bafs_dist = sum_rad - sum_dist;
            cdt_info->pos = sph.m_pos - cdt_info->bafs_dir * (sph.m_radius - cdt_info->bafs_dist / 2.0f);
        }
    }
    else
    {
    	// the sphere is above the cylinder
    	r32 h_diff = fsMath::abs(y_dist) - cyl.m_half_height;
    	if(h_diff > sph.m_radius)
    	{
    		return false;
    	}

    	r32 h_dist = fsMath::sqrt(sq_dist - y_dist * y_dist);
    	if(h_dist > sph.m_radius + cyl.m_radius)
    	{
    		return false;
    	}

    	r32 h_radius = fsMath::sqrt(sph.m_radius * sph.m_radius - h_diff * h_diff);

    	if(h_dist - cyl.m_radius > h_radius - fsMath::EPSILON)
    	{
    		return false;
    	}

        if (cdt_info)
        {
        	r32 dist = fsMath::sqrt(sq_dist);

            cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
            cdt_info->bafs_dist = sph.m_radius - dist * h_diff / fsMath::abs(y_dist);
            cdt_info->pos = sph.m_pos - cdt_info->bafs_dir * (sph.m_radius - cdt_info->bafs_dist / 2.0f);
        }
    }

	return true;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Box& box)
{
    if (!checkTouch(sph.m_aabb, box.m_aabb))
    {
        return false;
    }

    fsVec diff = sph.m_pos - box.m_world.trans;
    fsVec closest = box.m_world.trans;

#define CALC_CLOSEST_POINT(compo, axis) \
    do \
    { \
        r32 dist = diff.dot(axis); \
    \
        if (dist > box.m_half_size.compo) \
        { \
            dist = box.m_half_size.compo; \
        } \
        else if (dist < -box.m_half_size.compo) \
        { \
            dist = -box.m_half_size.compo; \
        } \
    \
        closest += (axis) * dist; \
    } \
    while (0)

    CALC_CLOSEST_POINT(x, box.m_world.x_axis);
    CALC_CLOSEST_POINT(y, box.m_world.y_axis);
    CALC_CLOSEST_POINT(z, box.m_world.z_axis);

#undef CALC_CLOSEST_POINT

    diff = sph.m_pos - closest;

    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sph.m_radius - fsMath::EPSILON) * (sph.m_radius - fsMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = fsMath::sqrt(sq_dist);

        cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
        cdt_info->bafs_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
    }

    return true;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri)
{
    if (!checkTouch(sph.m_aabb, tri.m_aabb))
    {
        return false;
    }

    fsVec closest;

    do
    {
        const fsVec& p = sph.m_pos;
        const fsVec& a = tri.m_pos1;
        const fsVec& b = tri.m_pos2;
        const fsVec& c = tri.m_pos3;

        fsVec ap = p - a;
        fsVec ab = b - a;
        fsVec ac = c - a;

        r32 ap_ab = ap.dot(ab);
        r32 ab_ac = ap.dot(ac);

        if (ap_ab <= 0.0f && ab_ac <= 0.0f)
        {
            closest = a;
            break;
        }

        fsVec bp = p - b;
        fsVec bc = c - b;

        r32 bp_ba = -bp.dot(ab);
        r32 bp_bc = bp.dot(bc);

        if (bp_ba <= 0.0f && bp_bc <= 0.0f)
        {
            closest = b;
            break;
        }

        fsVec cp = p - c;

        r32 cp_ca = -cp.dot(ac);
        r32 cp_cb = -cp.dot(bc);

        if (cp_ca <= 0.0f && cp_cb <= 0.0f)
        {
            closest = c;
            break;
        }

        fsVec n = ab.cross(ac);

        r32 vc = n.dot(ap.cross(bp));

        if (vc <= 0.0f && ap_ab >= 0.0f && bp_ba >= 0.0f)
        {
            closest = a + ab * (ap_ab / (ap_ab + bp_ba));
            break;
        }

        r32 va = n.dot(bp.cross(cp));

        if (va <= 0.0f && bp_bc >= 0.0f && cp_cb >= 0.0f)
        {
            closest = b + bc * (bp_bc / (bp_bc + cp_cb));
            break;
        }

        r32 vb = n.dot(cp.cross(ap));

        if (vb <= 0.0f && ab_ac >= 0.0f && cp_ca >= 0.0f)
        {
            closest = a + ac * (ab_ac / (ab_ac + cp_ca));
            break;
        }

        r32 s = va + vb + vc;
        r32 u = va / s;
        r32 v = vb / s;
        r32 w = 1.0f - u - v;

        closest = a * u + b * v + c * w;
    }
    while (0);

    fsVec diff = sph.m_pos - closest;
    r32 sq_dist = diff.sqLength();

    if (sq_dist > (sph.m_radius - fsMath::EPSILON) * (sph.m_radius - fsMath::EPSILON))
    {
        return false;
    }

    if (cdt_info)
    {
        r32 dist = fsMath::sqrt(sq_dist);

        cdt_info->bafs_dir = (dist > fsMath::EPSILON) ? diff / dist : fsVec::X_UNIT;
        cdt_info->bafs_dist = sph.m_radius - dist;
        cdt_info->pos = closest - cdt_info->bafs_dir * (cdt_info->bafs_dist / 2.0f);
    }

    return true;
}
