/*
 * fs_cdt_ray.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


fsCdt::Ray::Ray()
{
    m_from = m_to = fsVec::ZERO;

    updateAABB();
}


const fsCdt::AABB& fsCdt::Ray::getAABB() const
{
    return m_aabb;
}


const fsVec& fsCdt::Ray::getFrom() const
{
    return m_from;
}


const fsVec& fsCdt::Ray::getTo() const
{
    return m_to;
}


void fsCdt::Ray::setPos(const fsVec& from, const fsVec& to)
{
    m_from = from;
    m_to = to;

    updateAABB();
}


void fsCdt::Ray::updateAABB()
{
    fsVec min, max;

    if (m_from.x < m_to.x)
    {
        min.x = m_from.x;
        max.x = m_to.x;
    }
    else
    {
        min.x = m_to.x;
        max.x = m_from.x;
    }

    if (m_from.y < m_to.y)
    {
        min.y = m_from.y;
        max.y = m_to.y;
    }
    else
    {
        min.y = m_to.y;
        max.y = m_from.y;
    }

    if (m_from.z < m_to.z)
    {
        min.z = m_from.z;
        max.z = m_to.z;
    }
    else
    {
        min.z = m_to.z;
        max.z = m_from.z;
    }

    m_aabb.setBound(min, max);
}


bool fsCdt::intersect(fsVec* pos, const Ray& ray, const Sph& sph)
{
    if (!checkTouch(ray.m_aabb, sph.m_aabb))
    {
        return false;
    }

    fsVec m = sph.getPos() - ray.m_from;
    r32 c = m.sqLength() - sph.getRadius() * sph.getRadius();

    if (c <= 0.0f)
    {
        if (pos)
        {
            *pos = ray.m_from;
        }

        return true;
    }

    fsVec d = ray.m_to - ray.m_from;
    r32 b = m.dot(d);

    if (b < 0.0f)
    {
        return false;
    }

    r32 sq_d = d.sqLength();

    c /= sq_d;
    b /= sq_d;

    r32 discr = b * b - c;

    if (discr < 0.0f)
    {
        return false;
    }

    r32 dist = b - fsMath::sqrt(discr);

    if (dist > 1.0f)
    {
        return false;
    }

    if (pos)
    {
        *pos = ray.m_from + d * dist;
    }

    return true;
}


bool fsCdt::intersect(fsVec* pos, const Ray& ray, const Cyl& cyl)
{
    if (!checkTouch(ray.m_aabb, cyl.m_aabb))
    {
        return false;
    }

    return true;
}


bool fsCdt::intersect(fsVec* pos, const Ray& ray, const Box& box)
{
    if (!checkTouch(ray.m_aabb, box.m_aabb))
    {
        return false;
    }

    fsVec rel_pos = ray.m_from.toLocalOf(box.m_world);
    fsVec dir = ray.m_to - ray.m_from;
    fsVec rel_dir = dir.toLocalOf_noTrans(box.m_world);

    r32 min_dist, max_dist;

    if (intersectLocalBox(&min_dist, &max_dist, rel_pos, rel_dir, box.m_half_size) && min_dist <= 1.0f)
    {
        if (pos)
        {
            *pos = ray.m_from + dir * min_dist;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool fsCdt::intersect(fsVec* pos, const Ray& ray, const Tri& tri)
{
    if (!checkTouch(ray.m_aabb, tri.m_aabb))
    {
        return false;
    }

    fsVec ray_dir = ray.m_to - ray.m_from;

    r32 dist;

    if (intersectTri(&dist, ray.m_from, ray_dir, tri) && dist <= 1.0f)
    {
        if (pos)
        {
            *pos = ray.m_from + ray_dir * dist;
        }

        return true;
    }
    else
    {
        return false;
    }
}


bool fsCdt::intersectLocalBox(r32* min_dist, r32* max_dist, const fsVec& local_ray_pos, const fsVec& local_ray_dir, const fsVec& box_half_size)
{
    *min_dist = -1000000.0f;
    *max_dist = 1000000.0f;

    const r32* p = &local_ray_pos.x;
    const r32* d = &local_ray_dir.x;
    const r32* s = &box_half_size.x;

    for (s32 i = 0; i < 3; i++, p++, d++, s++)
    {
        if (*d < fsMath::EPSILON && *d > -fsMath::EPSILON)
        {
            if (*p < -*s || *p > *s)
            {
                return false;
            }
        }
        else
        {
            r32 ood = 1.0f / *d;
            r32 t1 = (*s - *p) * ood;
            r32 t2 = (-*s - *p) * ood;

            if (t1 > t2)
            {
                r32 t3 = t1;
                t1 = t2;
                t2 = t3;
            }

            if (t1 > *min_dist)
            {
                *min_dist = t1;
            }

            if (t2 < *max_dist)
            {
                *max_dist = t2;
            }

            if (*min_dist > *max_dist)
            {
                return false;
            }
        }
    }

    if (*max_dist < 0.0f)
    {
        return false;
    }

    if (*min_dist < 0.0f)
    {
        *min_dist = 0.0f;
    }

    return true;
}


bool fsCdt::intersectTri(r32* dist, const fsVec& ray_pos, const fsVec& ray_dir, const Tri& tri)
{
    fsVec ab = tri.m_pos2 - tri.m_pos1;
    fsVec ac = tri.m_pos3 - tri.m_pos1;
    fsVec pvec = ray_dir.cross(ac);

    r32 det = ab.dot(pvec);

    if (det < fsMath::EPSILON && det > -fsMath::EPSILON)
    {
        return false;
    }

    fsVec tvec = ray_pos - tri.m_pos1;
    r32 inv_det = 1.0f / det;
    r32 u = tvec.dot(pvec) * inv_det;

    if (u < 0.0f || u > 1.0f)
    {
        return false;
    }

    fsVec qvec = tvec.cross(ab);
    r32 v = ray_dir.dot(qvec) * inv_det;

    if (v < 0.0f || u + v > 1.0f)
    {
        return false;
    }

    *dist = ac.dot(qvec) * inv_det;

    return (*dist < 0.0f) ? false : true;
}
