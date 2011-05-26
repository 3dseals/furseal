/*
 * fs_cdt_box.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_cdt_all.h"


const r32 BOX_ENLARGEMENT_RATE = 1.001f;


fsCdt::Box::Box()
{
    m_world = fsMat::UNIT;
    m_half_size = fsVec::ZERO;

    updateAABB();
}


const fsCdt::AABB& fsCdt::Box::getAABB() const
{
    return m_aabb;
}


const fsMat& fsCdt::Box::getWorld() const
{
    return m_world;
}


void fsCdt::Box::setWorld(const fsMat& world)
{
    m_world = world;

    updateAABB();
}


r32 fsCdt::Box::getWidth() const
{
    return m_half_size.x * 2.0f;
}


r32 fsCdt::Box::getHeight() const
{
    return m_half_size.y * 2.0f;
}


r32 fsCdt::Box::getDepth() const
{
    return m_half_size.z * 2.0f;
}


const fsVec& fsCdt::Box::getHalfSize() const
{
    return m_half_size;
}


void fsCdt::Box::setSize(r32 width, r32 height, r32 depth)
{
    if (width < 0.0f || height < 0.0f || depth < 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_half_size.set(width / 2.0f, height / 2.0f, depth / 2.0f);

    updateAABB();
}


void fsCdt::Box::updateAABB()
{
    fsVec half_diag( //
        fsMath::abs(m_world.x_axis.x) * m_half_size.x + fsMath::abs(m_world.y_axis.x) * m_half_size.y + fsMath::abs(m_world.z_axis.x) * m_half_size.z, //
        fsMath::abs(m_world.x_axis.y) * m_half_size.x + fsMath::abs(m_world.y_axis.y) * m_half_size.y + fsMath::abs(m_world.z_axis.y) * m_half_size.z, //
        fsMath::abs(m_world.x_axis.z) * m_half_size.x + fsMath::abs(m_world.y_axis.z) * m_half_size.y + fsMath::abs(m_world.z_axis.z) * m_half_size.z);

    m_aabb.setBound(m_world.trans - half_diag, m_world.trans + half_diag);
}


bool fsCdt::collide(CdtInfo* cdt_info, const Box& box1, const Box& box2)
{
    if (!chefsTouch(box1.m_aabb, box2.m_aabb))
    {
        return false;
    }

    fsMat local_b = box2.m_world.toLocalOf(box1.m_world);

    fsMat abs_local_b( //
        fsVec(fsMath::abs(local_b.x_axis.x), fsMath::abs(local_b.x_axis.y), fsMath::abs(local_b.x_axis.z)), //
        fsVec(fsMath::abs(local_b.y_axis.x), fsMath::abs(local_b.y_axis.y), fsMath::abs(local_b.y_axis.z)), //
        fsVec(fsMath::abs(local_b.z_axis.x), fsMath::abs(local_b.z_axis.y), fsMath::abs(local_b.z_axis.z)), //
        fsVec::ZERO);

    const fsVec& size_a = box1.m_half_size;
    const fsVec& size_b = box2.m_half_size;

    fsVec t = (box2.m_world.trans - box1.m_world.trans).toLocalOf_noTrans(box1.m_world);

    fsVec bafs_dir;
    r32 bafs_dist = 1000000.0f;

    // L = AX
    // L = AY
    // L = AZ
#define CHEFS_BAFS_DIST(ra, rb, tl, axis) \
    do \
    { \
        r32 cur_bafs_dist = (ra) + (rb) - fsMath::abs(tl); \
    \
        if (cur_bafs_dist < fsMath::EPSILON) \
        { \
            return false; \
        } \
        else if (cur_bafs_dist < bafs_dist) \
        { \
            bafs_dir = (tl < 0.0f) ? (axis) : -(axis); \
            bafs_dist = cur_bafs_dist; \
        } \
    } \
    while (false)

#define CHEFS_SEPARATING_AXIS(compo, axis) \
    do \
    { \
        r32 ra = size_a.compo; \
        r32 rb = abs_local_b.x_axis.compo * size_b.x + abs_local_b.y_axis.compo * size_b.y + abs_local_b.z_axis.compo * size_b.z; \
        r32 tl = t.compo; \
    \
        CHEFS_BAFS_DIST(ra, rb, tl, axis); \
    } \
    while (false)

    CHEFS_SEPARATING_AXIS(x, box1.m_world.x_axis);
    CHEFS_SEPARATING_AXIS(y, box1.m_world.y_axis);
    CHEFS_SEPARATING_AXIS(z, box1.m_world.z_axis);

#undef CHEFS_SEPARATING_AXIS

    // L=BX
    // L=BY
    // L=BZ
#define CHEFS_SEPARATING_AXIS(compo1, compo2, axis) \
    do \
    { \
        r32 ra = abs_local_b.compo2.dot(size_a); \
        r32 rb = size_b.compo1; \
        r32 tl = t.dot(local_b.compo2); \
    \
        CHEFS_BAFS_DIST(ra, rb, tl, axis); \
    } \
    while (false)

    CHEFS_SEPARATING_AXIS(x, x_axis, box2.m_world.x_axis);
    CHEFS_SEPARATING_AXIS(y, y_axis, box2.m_world.y_axis);
    CHEFS_SEPARATING_AXIS(z, z_axis, box2.m_world.z_axis);

#undef CHEFS_BAFS_DIST
#undef CHEFS_SEPARATING_AXIS

    fsVec sa_bafs_dir[9];
    r32 sa_bafs_dist[9];
    u8 sa_num = 0;

#define CHEFS_SEPARATING_AXIS(lx, ly, lz) \
    do \
    { \
        r32 tl = t.x * (lx) + t.y * (ly) + t.z * (lz); \
        r32 cur_bafs_dist = ra + rb - fsMath::abs(tl); \
    \
        if (cur_bafs_dist < fsMath::EPSILON) \
        { \
            return false; \
        } \
    \
        sa_bafs_dir[sa_num] = (tl < 0.0f) ? fsVec(lx, ly, lz) : fsVec(-(lx), -(ly), -(lz)); \
        sa_bafs_dist[sa_num] = cur_bafs_dist; \
        sa_num++; \
    } \
    while (false)

#define IS_VALID_AXIS(a, b) ((a) > fsMath::EPSILON || (b) > fsMath::EPSILON)

    // L = AX * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.z, abs_local_b.x_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.x_axis.z + size_a.z * abs_local_b.x_axis.y;
        r32 rb = size_b.y * abs_local_b.z_axis.x + size_b.z * abs_local_b.y_axis.x;

        CHEFS_SEPARATING_AXIS(0.0f, -local_b.x_axis.z, local_b.x_axis.y);
    }

    // L = AX * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.z, abs_local_b.y_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.y_axis.z + size_a.z * abs_local_b.y_axis.y;
        r32 rb = size_b.x * abs_local_b.z_axis.x + size_b.z * abs_local_b.x_axis.x;

        CHEFS_SEPARATING_AXIS(0.0f, -local_b.y_axis.z, local_b.y_axis.y);
    }

    // L = AX * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.z, abs_local_b.z_axis.y))
    {
        r32 ra = size_a.y * abs_local_b.z_axis.z + size_a.z * abs_local_b.z_axis.y;
        r32 rb = size_b.x * abs_local_b.y_axis.x + size_b.y * abs_local_b.x_axis.x;

        CHEFS_SEPARATING_AXIS(0.0f, -local_b.z_axis.z, local_b.z_axis.y);
    }

    // L = AY * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.z, abs_local_b.x_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.x_axis.z + size_a.z * abs_local_b.x_axis.x;
        r32 rb = size_b.y * abs_local_b.z_axis.y + size_b.z * abs_local_b.y_axis.y;

        CHEFS_SEPARATING_AXIS(local_b.x_axis.z, 0.0f, -local_b.x_axis.x);
    }

    // L = AY * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.z, abs_local_b.y_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.y_axis.z + size_a.z * abs_local_b.y_axis.x;
        r32 rb = size_b.x * abs_local_b.z_axis.y + size_b.z * abs_local_b.x_axis.y;

        CHEFS_SEPARATING_AXIS(local_b.y_axis.z, 0.0f, -local_b.y_axis.x);
    }

    // L = AY * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.z, abs_local_b.z_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.z_axis.z + size_a.z * abs_local_b.z_axis.x;
        r32 rb = size_b.x * abs_local_b.y_axis.y + size_b.y * abs_local_b.x_axis.y;

        CHEFS_SEPARATING_AXIS(local_b.z_axis.z, 0.0f, -local_b.z_axis.x);
    }

    // L = AZ * BX
    if (IS_VALID_AXIS(abs_local_b.x_axis.y, abs_local_b.x_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.x_axis.y + size_a.y * abs_local_b.x_axis.x;
        r32 rb = size_b.y * abs_local_b.z_axis.z + size_b.z * abs_local_b.y_axis.z;

        CHEFS_SEPARATING_AXIS(-local_b.x_axis.y, local_b.x_axis.x, 0.0f);
    }

    // L = AZ * BY
    if (IS_VALID_AXIS(abs_local_b.y_axis.y, abs_local_b.y_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.y_axis.y + size_a.y * abs_local_b.y_axis.x;
        r32 rb = size_b.x * abs_local_b.z_axis.z + size_b.z * abs_local_b.x_axis.z;

        CHEFS_SEPARATING_AXIS(-local_b.y_axis.y, local_b.y_axis.x, 0.0f);
    }

    // L = AZ * BZ
    if (IS_VALID_AXIS(abs_local_b.z_axis.y, abs_local_b.z_axis.x))
    {
        r32 ra = size_a.x * abs_local_b.z_axis.y + size_a.y * abs_local_b.z_axis.x;
        r32 rb = size_b.x * abs_local_b.y_axis.z + size_b.y * abs_local_b.x_axis.z;

        CHEFS_SEPARATING_AXIS(-local_b.z_axis.y, local_b.z_axis.x, 0.0f);
    }

#undef CHEFS_SEPARATING_AXIS
#undef IS_VALID_AXIS

    if (!cdt_info)
    {
        return true;
    }

    /*
        calculate cdt_info.bafs_dir and cdt_info.bafs_dist
    */
    bool is_changed = false;

    for (s32 i = 0; i < sa_num; i++)
    {
        if (sa_bafs_dist[i] > fsMath::EPSILON)
        {
            r32 length = sa_bafs_dir[i].length();

            if (length > fsMath::EPSILON)
            {
                sa_bafs_dist[i] /= length;

                if (sa_bafs_dist[i] < bafs_dist)
                {
                    is_changed = true;

                    bafs_dir = sa_bafs_dir[i] / length;
                    bafs_dist = sa_bafs_dist[i];
                }
            }
        }
    }

    if (is_changed)
    {
        bafs_dir = bafs_dir.toGlobalFrom_noTrans(box1.m_world);
    }

    /*
        calculate cdt_info.pos
    */
    fsVec size_b2 = size_b * BOX_ENLARGEMENT_RATE;

    fsVec sx = local_b.x_axis * size_b2.x;
    fsVec sy = local_b.y_axis * size_b2.y;
    fsVec sz = local_b.z_axis * size_b2.z;

    fsVec inter_pos[8];
    u8 inter_num = 0;

    for (s32 i = 0; i < 8; i++)
    {
        fsVec cur_pos = local_b.trans + ((i % 2 == 0) ? sx : -sx) + ((i % 4 < 2) ? sy : -sy) + ((i < 4) ? sz : -sz);

        if (cur_pos.x <= size_a.x && cur_pos.x >= -size_a.x && //
            cur_pos.y <= size_a.y && cur_pos.y >= -size_a.y && //
            cur_pos.z <= size_a.z && cur_pos.z >= -size_a.z)
        {
            cur_pos = cur_pos.toGlobalFrom(box1.m_world);
            inter_pos[inter_num] = cur_pos;
            inter_num++;
        }
    }

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->bafs_dir = bafs_dir;
        cdt_info->bafs_dist = bafs_dist;

        return true;
    }

    ////////

    fsVec vert[8];
    fsMat local_a = box1.m_world.toLocalOf(box2.m_world);

    sx = local_a.x_axis * size_a.x;
    sy = local_a.y_axis * size_a.y;
    sz = local_a.z_axis * size_a.z;

    inter_num = 0;

    for (s32 i = 0; i < 8; i++)
    {
        fsVec cur_pos = vert[i] = local_a.trans + ((i % 2 == 0) ? sx : -sx) + ((i % 4 < 2) ? sy : -sy) + ((i < 4) ? sz : -sz);

        if (cur_pos.x <= size_b2.x && cur_pos.x >= -size_b2.x && //
            cur_pos.y <= size_b2.y && cur_pos.y >= -size_b2.y && //
            cur_pos.z <= size_b2.z && cur_pos.z >= -size_b2.z)
        {
            cur_pos = cur_pos.toGlobalFrom(box2.m_world);
            inter_pos[inter_num] = cur_pos;
            inter_num++;
        }
    }

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->bafs_dir = bafs_dir;
        cdt_info->bafs_dist = bafs_dist;

        return true;
    }

    ////////

    r32 min_dist, max_dist;
    fsVec size_wa = size_a * 2.0f;

    inter_num = 0;

#define CALC_INTERSECT(ray_pos, ray_dir, compo) \
    do \
    { \
        if (intersectLocalBox(&min_dist, &max_dist, (ray_pos), (ray_dir), size_b2) && min_dist <= size_wa.compo) \
        { \
            inter_pos[inter_num] = ray_pos + ray_dir * ((min_dist + fsMath::min(max_dist, size_wa.compo)) / 2.0f); \
            inter_num++; \
        } \
    } \
    while (false)

    CALC_INTERSECT(vert[1], local_a.x_axis, x);
    CALC_INTERSECT(vert[3], local_a.x_axis, x);
    CALC_INTERSECT(vert[5], local_a.x_axis, x);
    CALC_INTERSECT(vert[7], local_a.x_axis, x);

    if (inter_num == 0)
    {
        CALC_INTERSECT(vert[2], local_a.y_axis, y);
        CALC_INTERSECT(vert[3], local_a.y_axis, y);
        CALC_INTERSECT(vert[6], local_a.y_axis, y);
        CALC_INTERSECT(vert[7], local_a.y_axis, y);
    }

    if (inter_num == 0)
    {
        CALC_INTERSECT(vert[4], local_a.z_axis, z);
        CALC_INTERSECT(vert[5], local_a.z_axis, z);
        CALC_INTERSECT(vert[6], local_a.z_axis, z);
        CALC_INTERSECT(vert[7], local_a.z_axis, z);
    }

#undef CALC_INTERSECT

    if (inter_num > 0)
    {
        cdt_info->pos = inter_pos[0];

        if (inter_num > 1)
        {
            for (s32 i = 1; i < inter_num; i++)
            {
                cdt_info->pos += inter_pos[i];
            }

            cdt_info->pos /= inter_num;
        }

        cdt_info->pos = cdt_info->pos.toGlobalFrom(box2.m_world) + bafs_dir * (bafs_dist / 2.0f);
        cdt_info->bafs_dir = bafs_dir;
        cdt_info->bafs_dist = bafs_dist;

        return true;
    }

    cdt_info->pos = box1.m_world.trans;
    cdt_info->bafs_dir = fsVec::X_UNIT;
    cdt_info->bafs_dist = 0.0f;

    return true; // unexpected case
}


bool fsCdt::collide(CdtInfo* cdt_info, const Box& box, const Sph& sph)
{
    bool res = collide(cdt_info, sph, box);

    if (cdt_info)
    {
        cdt_info->bafs_dir = -cdt_info->bafs_dir;
    }

    return res;
}


bool fsCdt::collide(CdtInfo* cdt_info, const Box& box, const Tri& tri)
{
    if (!chefsTouch(box.m_aabb, tri.m_aabb))
    {
        return false;
    }

    fsVec bafs_dir;
    r32 bafs_dist = 1000000.0f;

    fsVec center = (tri.m_pos1 + tri.m_pos2 + tri.m_pos3) / 3.0f;

    // L = Box.X
    // L = Box.Y
    // L = Box.Z
    fsVec a = tri.m_pos1.toLocalOf(box.m_world);
    fsVec b = tri.m_pos2.toLocalOf(box.m_world);
    fsVec c = tri.m_pos3.toLocalOf(box.m_world);

    r32 min_dist, max_dist;

#define CHEFS_SEPARATING_AXIS(compo, axis) \
    do \
    { \
        min_dist = (a.compo < b.compo && a.compo < c.compo) ? a.compo : ((b.compo < c.compo) ? b.compo : c.compo); \
    \
        if (min_dist > box.m_half_size.compo - fsMath::EPSILON) \
        { \
            return false; \
        } \
    \
        r32 cur_bafs_dist = box.m_half_size.compo - min_dist; \
    \
        if (cur_bafs_dist < bafs_dist) \
        { \
            bafs_dir = -(axis); \
            bafs_dist = cur_bafs_dist; \
        } \
    \
        max_dist = (a.compo > b.compo && a.compo > c.compo) ? a.compo : ((b.compo > c.compo) ? b.compo : c.compo); \
    \
        if (max_dist < -box.m_half_size.compo + fsMath::EPSILON) \
        { \
            return false; \
        } \
    \
        cur_bafs_dist = max_dist + box.m_half_size.compo; \
    \
        if (cur_bafs_dist < bafs_dist) \
        { \
            bafs_dir = (axis); \
            bafs_dist = cur_bafs_dist; \
        } \
    } \
    while (false)

    CHEFS_SEPARATING_AXIS(x, box.m_world.x_axis);
    CHEFS_SEPARATING_AXIS(y, box.m_world.y_axis);
    CHEFS_SEPARATING_AXIS(z, box.m_world.z_axis);

#undef CHEFS_SEPARATING_AXIS

    fsVec p = center.toLocalOf(box.m_world);

    fsVec ap = p - a;
    fsVec bp = p - b;
    fsVec cp = p - c;

    fsVec sa_bafs_dir[10];
    r32 sa_bafs_dist[10];
    u8 sa_num;

    // L = Normal of Triangle
    {
        fsVec n = (a - b).cross(a - c);

        r32 ra = 0.0f;
        r32 rb = fsMath::abs(n.x) * box.m_half_size.x + fsMath::abs(n.y) * box.m_half_size.y + fsMath::abs(n.z) * box.m_half_size.z;
        r32 tl = p.dot(n);

        r32 cur_bafs_dist = ra + rb - fsMath::abs(tl);

        if (cur_bafs_dist < fsMath::EPSILON)
        {
            return false;
        }

        sa_bafs_dir[0] = (tl < 0.0f) ? n : -n;
        sa_bafs_dist[0] = cur_bafs_dist;
        sa_num = 1;
    }

#define CHEFS_SEPARATING_AXIS(lx, ly, lz, abs_lx, abs_ly, abs_lz) \
    do \
    { \
        if (abs_lx > fsMath::EPSILON || abs_ly > fsMath::EPSILON || abs_lz > fsMath::EPSILON) \
        { \
            r32 apl = a.x * (lx) + a.y * (ly) + a.z * (lz); \
            r32 bpl = b.x * (lx) + b.y * (ly) + b.z * (lz); \
            r32 cpl = c.x * (lx) + c.y * (ly) + c.z * (lz); \
    \
            r32 ra = abs_lx * box.m_half_size.x + abs_ly * box.m_half_size.y + abs_lz * box.m_half_size.z; \
            max_dist = (apl > bpl && apl > cpl) ? apl : ((bpl > cpl) ? bpl : cpl); \
            min_dist = (apl < bpl && apl < cpl) ? apl : ((bpl < cpl) ? bpl : cpl); \
    \
            r32 cur_bafs_dist1 = ra + max_dist; \
            r32 cur_bafs_dist2 = ra - min_dist; \
    \
            if (cur_bafs_dist1 < fsMath::EPSILON || cur_bafs_dist2 < fsMath::EPSILON) \
            { \
                return false; \
            } \
    \
            r32 tl = p.x * (lx) + p.y * (ly) + p.z * (lz); \
    \
            sa_bafs_dir[sa_num] = (tl < 0.0f) ? fsVec(lx, ly, lz) : fsVec(-(lx), -(ly), -(lz)); \
            sa_bafs_dist[sa_num] = fsMath::min(cur_bafs_dist1, cur_bafs_dist2); \
            sa_num++; \
        } \
    } \
    while (false)

#define CHEFS_SEPARATING_AXIES(edge) \
    do \
    { \
        CHEFS_SEPARATING_AXIS(0.0f, -(edge).z, (edge).y, 0.0f, fsMath::abs((edge).z), fsMath::abs((edge).y)); \
        CHEFS_SEPARATING_AXIS((edge).z, 0.0f, -(edge).x, fsMath::abs((edge).z), 0.0f, fsMath::abs((edge).x)); \
        CHEFS_SEPARATING_AXIS(-(edge).y, (edge).x, 0.0f, fsMath::abs((edge).y), fsMath::abs((edge).x), 0.0f); \
    } \
    while (false)

    // L = Box.X * AB
    // L = Box.Y * AB
    // L = Box.Z * AB
    fsVec ab = b - a;
    CHEFS_SEPARATING_AXIES(ab);

    // L = Box.X * BC
    // L = Box.Y * BC
    // L = Box.Z * BC
    fsVec bc = c - b;
    CHEFS_SEPARATING_AXIES(bc);

    // L = Box.X * CA
    // L = Box.Y * CA
    // L = Box.Z * CA
    fsVec ca = a - c;
    CHEFS_SEPARATING_AXIES(ca);

#undef CHEFS_SEPARATING_AXIES
#undef CHEFS_SEPARATING_AXIS

    if (!cdt_info)
    {
        return true;
    }

    /*
        calculate cdt_info.bafs_dir and cdt_info.bafs_dist
    */
    bool is_changed = false;

    for (s32 i = 0; i < sa_num; i++)
    {
        if (sa_bafs_dist[i] > fsMath::EPSILON)
        {
            r32 length = sa_bafs_dir[i].length();

            if (length > fsMath::EPSILON)
            {
                sa_bafs_dist[i] /= length;

                if (sa_bafs_dist[i] < bafs_dist)
                {
                    is_changed = true;

                    bafs_dir = sa_bafs_dir[i] / length;
                    bafs_dist = sa_bafs_dist[i];
                }
            }
        }
    }

    if (is_changed)
    {
        bafs_dir = bafs_dir.toGlobalFrom_noTrans(box.m_world);
    }

    /*
        calculate cdt_info.pos
    */
    fsVec inter_pos[16];
    u8 inter_num = 0;

    fsVec box_half_size2 = box.m_half_size * BOX_ENLARGEMENT_RATE;

#define CHEFS_VERT_IN_BOX(local_vert, global_vert) \
    do \
    { \
        if ((local_vert).x <= box_half_size2.x && (local_vert).x >= -box_half_size2.x && /**/ \
            (local_vert).y <= box_half_size2.y && (local_vert).y >= -box_half_size2.y && /**/ \
            (local_vert).z <= box_half_size2.z && (local_vert).z >= -box_half_size2.z) \
        { \
            inter_pos[inter_num] = global_vert; \
            inter_num++; \
        } \
    } \
    while (false)

#define CALC_INTERSECT_POS() \
    do \
    { \
        if (inter_num > 0) \
        { \
            cdt_info->pos = inter_pos[0]; \
    \
            if (inter_num > 1) \
            { \
                for (s32 i = 1; i < inter_num; i++) \
                { \
                    cdt_info->pos += inter_pos[i]; \
                } \
    \
                cdt_info->pos /= inter_num; \
            } \
    \
            cdt_info->bafs_dir = bafs_dir; \
            cdt_info->bafs_dist = bafs_dist; \
    \
            return true; \
        } \
    } \
    while (false)

    CHEFS_VERT_IN_BOX(a, tri.m_pos1);
    CHEFS_VERT_IN_BOX(b, tri.m_pos2);
    CHEFS_VERT_IN_BOX(c, tri.m_pos3);

    CALC_INTERSECT_POS();

    ////////

    inter_num = 0;

    if (intersectLocalBox(&min_dist, &max_dist, a, ab, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (a + ab * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    if (intersectLocalBox(&min_dist, &max_dist, b, bc, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (b + bc * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    if (intersectLocalBox(&min_dist, &max_dist, c, ca, box_half_size2) && min_dist <= 1.0f)
    {
        inter_pos[inter_num] = (c + ca * ((min_dist + ((max_dist > 1.0f) ? 1.0f : max_dist)) / 2.0f)).toGlobalFrom(box.m_world);
        inter_num++;
    }

    CALC_INTERSECT_POS();

    ////////

    inter_num = 0;

    r32 dist;
    fsVec ray_pos;

#define INTERSECT_TRIANGLE(ray_dir, max_dist) \
    do \
    { \
        if (intersectTri(&dist, ray_pos, ray_dir, tri) && dist <= (max_dist)) \
        { \
            inter_pos[inter_num] = ray_pos + (ray_dir) * dist; \
            inter_num++; \
        } \
    } \
    while (false)

    fsVec vec_x = box.m_world.x_axis * box_half_size2.x;
    fsVec vec_y = box.m_world.y_axis * box_half_size2.y;
    fsVec vec_z = box.m_world.z_axis * box_half_size2.z;

    r32 size_x = box_half_size2.x * 2.0f;
    r32 size_y = box_half_size2.y * 2.0f;
    r32 size_z = box_half_size2.z * 2.0f;

    ray_pos = box.m_world.trans - vec_x + vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.x_axis, size_x);

    ray_pos = box.m_world.trans + vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans - vec_x - vec_y + vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans + vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.y_axis, size_y);

    ray_pos = box.m_world.trans + vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans - vec_x + vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans + vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    ray_pos = box.m_world.trans - vec_x - vec_y - vec_z;
    INTERSECT_TRIANGLE(box.m_world.z_axis, size_z);

    CALC_INTERSECT_POS();

    cdt_info->pos = box.m_world.trans;
    cdt_info->bafs_dir = fsVec::X_UNIT;
    cdt_info->bafs_dist = 0.0f;

    return true; // unexpected case
}
