/*
 * fs_scr.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_base_all.h"


void fsScr::moveLast()
{
    fsDrawMgr::instance()->m_scr_map.moveLast(m_id);
}


void fsScr::setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height)
{
    if (width == 0 || height == 0)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_left_in_framebuffer = left;
    m_top_in_framebuffer = top;
    m_width_in_framebuffer = width;
    m_height_in_framebuffer = height;
}


void fsScr::setViewSize(r32 width, r32 height)
{
    m_view_width = width;
    m_view_height = height;
}


bool fsScr::isActive() const
{
    return m_flag.isOn(FLAG_ACTIVE);
}


void fsScr::setActive(bool is_active)
{
    m_flag.set(FLAG_ACTIVE, is_active);
}


bool fsScr::isClearColor() const
{
    return m_flag.isOn(FLAG_CLEAR_COLOR);
}


bool fsScr::isClearDepth() const
{
    return m_flag.isOn(FLAG_CLEAR_DEPTH);
}


void fsScr::setClearMode(bool is_clear_color, bool is_clear_depth)
{
    m_flag.set(FLAG_CLEAR_COLOR, is_clear_color);
    m_flag.set(FLAG_CLEAR_DEPTH, is_clear_depth);
}


fsCol fsScr::getClearColor() const
{
    return m_clear_col;
}


void fsScr::setClearColor(fsCol col)
{
    m_clear_col = col;
}


bool fsScr::isPerspective() const
{
    return m_flag.isOn(FLAG_PERSPECTIVE);
}


void fsScr::setPerspective(bool is_perspective)
{
    m_flag.set(FLAG_PERSPECTIVE, is_perspective);
}


r32 fsScr::getFocusDist() const
{
    return m_focus_dist;
}


void fsScr::setFocusDist(r32 focus_dist)
{
    if (focus_dist < fsMath::EPSILON)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_focus_dist = focus_dist;
}


r32 fsScr::getNearClipDist() const
{
    return m_near_clip_dist;
}


r32 fsScr::getFarClipDist() const
{
    return m_far_clip_dist;
}


void fsScr::setClipDist(r32 near, r32 far)
{
    if (near < fsMath::EPSILON || far <= near)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_near_clip_dist = near;
    m_far_clip_dist = far;
}


fsMat& fsScr::view()
{
    return m_view;
}


void fsScr::detachScreenTexture()
{

}


fsScr::fsScr(fsID scr_id)
{
    m_id = scr_id;
    m_view = fsMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(DEFAULT_FOCUS_DIST));
    m_root_draw.m_private_flag.setOn(fsDraw::FLAG_INITIALIZED);
    m_root_draw.m_scr_id = m_id;

    m_flag.clear();

    for (u32 i = 0; i < GUEST_SCREEN_NUM; i++)
    {
        m_guest_id[i] = fsID::ZERO;
    }

    setActive(true);
    setAreaInFramebuffer(0, 0, fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
    setViewSize(fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
    setClearMode(true, true);
    setClearColor(fsCol(255, 0, 0, 0));
    setPerspective(true);
    setFocusDist(static_cast<r32>(DEFAULT_FOCUS_DIST));
    setClipDist(static_cast<r32>(DEFAULT_NEAR_CLIP_DIST), static_cast<r32>(DEFAULT_FAR_CLIP_DIST));

    fsDrawMgr::instance()->m_scr_map.add(m_id, this);
}


fsScr::~fsScr()
{
    fsDrawMgr::instance()->m_scr_map.remove(m_id);

    detachScreenTexture();
}


FS_DEFINE_OPERATOR_EQUAL(fsScr)


void fsScr::calcProjection()
{
    r32* m = m_view_to_clip;

    if (isPerspective())
    {
        float inv_sub = 1.0f / (m_far_clip_dist - m_near_clip_dist);

        m[0] = m_focus_dist * 2.0f / m_view_width;
        m[4] = 0.0f;
        m[8] = 0.0f;
        m[12] = 0.0f;

        m[1] = 0.0f;
        m[5] = m_focus_dist * 2.0f / m_view_height;
        m[9] = 0.0f;
        m[13] = 0.0f;

        m[2] = 0.0f;
        m[6] = 0.0f;
        m[10] = (m_far_clip_dist + m_near_clip_dist) * inv_sub;
        m[14] = 2.0f * m_far_clip_dist * m_near_clip_dist * inv_sub;

        m[3] = 0.0f;
        m[7] = 0.0f;
        m[11] = -1.0f;
        m[15] = 0.0f;
    }
    else
    {
        m[0] = 2.0f / m_view_width;
        m[4] = 0.0f;
        m[8] = 0.0f;
        m[12] = 0.0f;

        m[1] = 0.0f;
        m[5] = 2.0f / m_view_height;
        m[9] = 0.0f;
        m[13] = 0.0f;

        m[2] = 0.0f;
        m[6] = 0.0f;
        m[10] = 2.0f / (m_far_clip_dist - m_near_clip_dist);
        m[14] = (m_far_clip_dist + m_near_clip_dist) / (m_far_clip_dist - m_near_clip_dist);

        m[3] = 0.0f;
        m[7] = 0.0f;
        m[11] = 0.0f;
        m[15] = 1.0f;
    }

    fsMat world_to_view = fsMat::UNIT.toLocalOf(m_view);
    world_to_view.toR32x16(m_world_to_view);

    fsMat::mulR32x16(m_world_to_clip, m_view_to_clip, m_world_to_view);
}


void fsScr::setupProjection()
{
    calcProjection();
    fsLowLevelAPI::setProjection(m_world_to_clip);
}


fsVec fsScr::worldToClip_noCalcProjection(const fsVec& pos_in_world) const
{
    r32 v[4];

    pos_in_world.toR32x4(v, 1.0f);
    fsVec::mulR32x4(v, m_world_to_clip, v);

    return fsVec::fromR32x4(v) / v[3];
}


bool fsScr::canBoundClip_noCalcProjection(const fsMat& world, const fsVec& bound_max, const fsVec& bound_min) const
{
    fsVec pos[8];

    pos[0].set(bound_min.x, bound_min.y, bound_min.z);
    pos[1].set(bound_max.x, bound_min.y, bound_min.z);
    pos[2].set(bound_min.x, bound_max.y, bound_min.z);
    pos[3].set(bound_max.x, bound_max.y, bound_min.z);
    pos[4].set(bound_min.x, bound_min.y, bound_max.z);
    pos[5].set(bound_max.x, bound_min.y, bound_max.z);
    pos[6].set(bound_min.x, bound_max.y, bound_max.z);
    pos[7].set(bound_max.x, bound_max.y, bound_max.z);

    for (s32 i = 0; i < 8; i++)
    {
        pos[i] = pos[i].toGlobalFrom(world);
        pos[i] = worldToClip_noCalcProjection(pos[i]);
    }

    fsVec min = pos[0];
    fsVec max = pos[0];

    for (s32 i = 1; i < 8; i++)
    {
        if (min.x > pos[i].x)
        {
            min.x = pos[i].x;
        }

        if (min.y > pos[i].y)
        {
            min.y = pos[i].y;
        }

        if (min.z > pos[i].z)
        {
            min.z = pos[i].z;
        }

        if (max.x < pos[i].x)
        {
            max.x = pos[i].x;
        }

        if (max.y < pos[i].y)
        {
            max.y = pos[i].y;
        }

        if (max.z < pos[i].z)
        {
            max.z = pos[i].z;
        }
    }

    return (min.x > 1.0f || min.y > 1.0f || min.z > 1.0f || max.x < -1.0f || max.y < -1.0f || max.z < -1.0f);
}


void fsScr::copyScreenTexture()
{

}
