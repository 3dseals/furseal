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


fsDraw* fsDraw::getPrevAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* prev = m_tree.getPrevAllN();

    return (prev && prev->hasParent()) ? prev->getSelf() : NULL;
}


fsDraw* fsDraw::getNextAllN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* next = m_tree.getNextAllN();

    return next ? next->getSelf() : NULL;
}


fsDraw* fsDraw::getPrevSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* sibling = m_tree.getPrevSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


fsDraw* fsDraw::getNextSiblingN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* sibling = m_tree.getNextSiblingN();

    return sibling ? sibling->getSelf() : NULL;
}


fsDraw* fsDraw::getLastDescendant() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tree.getLastDescendant()->getSelf();
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


fsDraw* fsDraw::getLastChildN() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsTree<fsDraw>* child = m_tree.getLastChildN();

    return child ? child->getSelf() : NULL;
}


void fsDraw::moveFirst()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addFirst(&m_tree);
}


void fsDraw::moveLast()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_tree.getParentN()->addLast(&m_tree);
}


void fsDraw::moveBefore(fsDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_tree.joinBefore(&draw->m_tree);
}


void fsDraw::moveAfter(fsDraw* draw)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!draw || !draw->getParentN())
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_tree.joinAfter(&draw->m_tree);
}


fsDraw::DrawType fsDraw::getType() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_type.getType();
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


fsCol fsDraw::getColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_local_col;
}


void fsDraw::setColor(fsCol col)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_local_col = col;
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


void fsDraw::clearDrawFlag()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_draw_flag.clear();
}


void fsDraw::copyDrawFlag(const fsDraw* src)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (!src)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (src->m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_draw_flag = src->m_draw_flag;
}


const fsVec& fsDraw::getClipBoundMin() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_bound_min;
}


const fsVec& fsDraw::getClipBoundMax() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_bound_max;
}


void fsDraw::setClipBound(const fsVec& bound_min, const fsVec& bound_max)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    if (bound_min.x > bound_max.x || bound_min.y > bound_max.y || bound_min.z > bound_max.z)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    m_bound_min = bound_min;
    m_bound_max = bound_max;
}


const fsVec& fsDraw::getSortCenter() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_sort_center;
}


void fsDraw::setSortCenter(const fsVec& sort_center)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_sort_center = sort_center;
}


r32 fsDraw::getSortOffset() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_sort_offset;
}


void fsDraw::setSortOffset(r32 sort_offset)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_sort_offset = sort_offset;
}


fsID fsDraw::getTextureID() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_tex ? m_tex->getID() : fsID::ZERO;
}


void fsDraw::setTextureID(fsID tex_id)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    m_tex = (tex_id != fsID::ZERO) ? fsDrawMgr::getTexture(tex_id) : NULL;
}


fsMat& fsDraw::local()
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    return m_local;
}


fsMat fsDraw::calcWorld() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsMat world = m_local;

    for (fsDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        world = world.toGlobalFrom(parent->m_local);
    }

    return world;
}


fsCol fsDraw::calcFinalColor() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        fsThrow(ExceptionNotInitialized);
    }

    fsCol final_col = m_local_col;

    for (fsDraw* parent = getParentN(); parent; parent = parent->getParentN())
    {
        final_col *= parent->m_local_col;
    }

    return final_col;
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
