/*
 * fs_draw.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"


#include "fs_base_all.h"


fsDraw::~fsDraw()
{
    if (fsDrawMgr::isCreated() && m_private_flag.isOn(FLAG_INITIALIZED) && m_scr_id != fsDrawMgr::INVISIBLE_SCREEN_ID)
    {
        while (hasChild())
        {
            getFirstChildN()->setScreenID(fsDrawMgr::INVISIBLE_SCREEN_ID);
        }
    }
}


bool fsDraw::hasScreen() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && !m_tree.getParentN()->hasParent());
}


fsID fsDraw::getScreenID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!hasScreen())
    {
        fsThrow(ExceptionInvalidCall);
    }

    return m_tree.getParentN()->getSelf()->m_scr_id;
}


void fsDraw::setScreenID(fsID scr_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDrawMgr::getScreen(scr_id)->m_root_draw.m_tree.addLast(&m_tree);
}


bool fsDraw::hasParent() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return (m_tree.hasParent() && m_tree.getParentN()->hasParent());
}


fsDraw* fsDraw::getParentN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return hasParent() ? m_tree.getParentN()->getSelf() : NULL;
}


void fsDraw::setParent(fsDraw* parent)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!parent)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    parent->m_tree.addLast(&m_tree);
}


bool fsDraw::hasChild() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tree.hasChild();
}


fsDraw* fsDraw::getFirstChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* child = m_tree.getFirstChildN();

    return child ? child->getSelf() : NULL;
}


bool fsDraw::isVisible() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_private_flag.isOn(FLAG_VISIBLE);
}


void fsDraw::setVisible(bool is_visible)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_private_flag.set(FLAG_VISIBLE, is_visible);
}


fsDraw::DepthTest fsDraw::getDepthTest() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_depth_test.getType();
}


void fsDraw::setDepthTest(DepthTest depth_test)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_depth_test = depth_test;
}


fsDraw::BlendMode fsDraw::getBlendMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_blend_mode.getType();
}


void fsDraw::setBlendMode(BlendMode blend_mode, bool is_preset_setting)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_blend_mode = blend_mode;

    if (is_preset_setting)
    {
        setDepthTest(DEPTH_TEST_GEQUAL);
        m_draw_flag.clear();
        m_draw_flag.setOn(FLAG_WRITE_RGB);
        m_draw_flag.setOn(FLAG_BILINEAR);

        switch (m_blend_mode.getType())
        {
        case BLEND_OFF:
            m_draw_flag.setOn(FLAG_WRITE_DEPTH);
            break;

        case BLEND_HALF:
            m_draw_flag.setOn(FLAG_SORT);
            break;

        case BLEND_ADD:
            m_draw_flag.setOn(FLAG_SORT);
            break;

        case BLEND_DEST_ALPHA:
            m_draw_flag.setOn(FLAG_WRITE_DEPTH);
            break;

        default:
            break;
        }
    }
}


bool fsDraw::isDrawFlag(DrawFlag draw_flag) const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_draw_flag.isOn(draw_flag);
}


void fsDraw::setDrawFlag(DrawFlag draw_flag, bool is_on)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_draw_flag.set(draw_flag, is_on);
}


fsDraw::fsDraw()
{
    m_scr_id = fsID::ZERO;
    m_local = fsMat::UNIT;
    m_world = fsMat::UNIT;
    m_local_col = fsCol::FULL;
    m_final_col = fsCol::FULL;
    m_bound_min = fsVec::ZERO;
    m_bound_max = fsVec::ZERO;
    m_sort_center = fsVec::ZERO;
    m_sort_offset = 0.0f;
    m_sort_value = 0.0f;
    m_next_sort = NULL;

    m_tree.init(this);

    m_private_flag.setOn(FLAG_VISIBLE);

    m_private_flag.setOn(FLAG_INITIALIZED); // to call SetPreset_DefaultBlendOff
    setBlendMode(BLEND_OFF, true);
    m_private_flag.setOff(FLAG_INITIALIZED); // restore initialized-flag
}


void fsDraw::setupWorld()
{
    m_world = m_local.toGlobalFrom(m_tree.getParentN()->getSelf()->m_world);
}


void fsDraw::setupFinalColor()
{
    m_final_col = m_tree.getParentN()->getSelf()->m_final_col * m_local_col;
}


void fsDraw::setupSortValue(const fsMat& view)
{
    fsVec sort_center = m_sort_center.toGlobalFrom(m_world);

    m_sort_value = (sort_center - view.trans).dot(view.z_axis) + m_sort_offset;
}


void fsDraw::setupDrawState()
{
    fsLowLevelAPI::setDepthTest(static_cast<fsLowLevelAPI::DepthTest>(m_depth_test.getType()));
    fsLowLevelAPI::setBlendMode(static_cast<fsLowLevelAPI::BlendMode>(m_blend_mode.getType()));
    fsLowLevelAPI::setWriteMode(m_draw_flag.isOn(FLAG_WRITE_RGB), m_draw_flag.isOn(FLAG_WRITE_ALPHA), m_draw_flag.isOn(FLAG_WRITE_DEPTH));
    fsLowLevelAPI::setBackfaceCulling(m_draw_flag.isOn(FLAG_BAfsFACE_CULLING));

    r32 world[16];
    m_world.toR32x16(world);
    fsLowLevelAPI::setModelView(world);
}


void fsDraw::render(const fsMat& view)
{
    fsThrow(ExceptionInvalidCall);
}


FS_DEFINE_COPY_CONSTRUCTOR(fsDraw)


FS_DEFINE_OPERATOR_EQUAL(fsDraw)
