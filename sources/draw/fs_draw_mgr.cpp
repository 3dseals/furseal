/*
 * fs_draw_mgr.cpp
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs.h"

#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


const fsID fsDrawMgr::INVISIBLE_SCREEN_ID = fsID_("INVISIBLE_SCREEN");
const fsID fsDrawMgr::DEFAULT_3D_SCREEN_ID = fsID_("DEFAULT_3D_SCREEN");
const fsID fsDrawMgr::DEFAULT_2D_SCREEN_ID = fsID_("DEFAULT_2D_SCREEN");
const fsID fsDrawMgr::DEFAULT_LIGHT_SET_ID = fsID_("DEFAULT_LIGHT_SET");


fsDrawMgr* fsDrawMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsDrawMgr)


void fsDrawMgr::createAfterRes()
{
    destroyBeforeRes();

    m_instance = fsNew(fsDrawMgr);

    newScreen(INVISIBLE_SCREEN_ID);
    newScreen(DEFAULT_3D_SCREEN_ID);
    newScreen(DEFAULT_2D_SCREEN_ID);

    fsScr* scr0d = getScreen(INVISIBLE_SCREEN_ID);
    scr0d->setActive(false);

    fsScr* scr3d = getScreen(DEFAULT_3D_SCREEN_ID);
    scr3d->setClearColor(fsCol::ZERO);

    fsScr* scr2d = getScreen(DEFAULT_2D_SCREEN_ID);
    scr2d->setClearMode(false, true);
    scr2d->setPerspective(false);

    newLightSet(DEFAULT_LIGHT_SET_ID);

    fsLowLevelAPI::resetDrawState();

}


FS_DEFINE_MANAGER_DESTROY(fsDrawMgr, BeforeRes)


fsScr* fsDrawMgr::getScreen(fsID scr_id)
{
    fsDrawMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsScr** scr = ins->m_scr_map.getN(scr_id);

    if (!scr)
    {
        fsThrow(ExceptionNotFound);
    }

    return *scr;
}


fsScr* fsDrawMgr::newScreen(fsID scr_id)
{
    fsDrawMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_scr_map.getN(scr_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsScr)(scr_id);
}


fsLts* fsDrawMgr::getLightSet(fsID lts_id)
{
    fsDrawMgr* ins = instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsLts** lts = ins->m_lts_map.getN(lts_id);

    if (!lts)
    {
        fsThrow(ExceptionNotFound);
    }

    return *lts;
}


fsLts* fsDrawMgr::newLightSet(fsID lts_id)
{
    fsDrawMgr* ins = instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_lts_map.getN(lts_id))
    {
        fsThrow(ExceptionSameIDExists);
    }

    return fsNew(fsLts)(lts_id);
}


void fsDrawMgr::deleteLightSet(fsID lts_id)
{
    instance();

    if (lts_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsDelete(getLightSet(lts_id), fsLts);
}


void fsDrawMgr::renderForEngine()
{
	fsDrawMgr* ins = instance();

	    if (!ins->m_is_render)
	    {
	        return;
	    }

	    fsLowLevelAPI::resetDrawState(); // TODO

	    for (const fsID* id = ins->m_scr_map.getFirstKeyN(); id; id = ins->m_scr_map.getNextKeyN(*id))
	    {
	        fsScr* scr = *ins->m_scr_map.get(*id);

	        if (!scr->isActive())
	        {
	            continue;
	        }

	        fsLowLevelAPI::setViewport( //
	            scr->m_left_in_framebuffer, scr->m_top_in_framebuffer, //
	            scr->m_width_in_framebuffer, scr->m_height_in_framebuffer);

	        fsLowLevelAPI::clearFramebuffer(scr->isClearColor(), scr->isClearDepth(), reinterpret_cast<const u8*>(&scr->m_clear_col));

	        scr->setupProjection();

	        fsDraw* sort_list = NULL;

	        renderScreen(scr, &sort_list, scr->m_view);

	        for (u32 i = 0; i < fsScr::GUEST_SCREEN_NUM; i++)
	        {
	            if (scr->m_guest_id[i] != fsID::ZERO)
	            {
	                renderScreen(*ins->m_scr_map.get(scr->m_guest_id[i]), &sort_list, scr->m_view);
	            }
	        }

	        if (sort_list)
	        {
	            fsDraw* dummy;
	            sortList(&sort_list, &dummy, sort_list);
	        }

	        for (fsDraw* draw = sort_list; draw; draw = draw->m_next_sort)
	        {
	            draw->setupDrawState();
	            draw->render(scr->m_view);
	        }

	        //scr->copyScreenTexture();
	    }
}


fsDrawMgr::fsDrawMgr()
{
    m_is_render = true;

    m_scr_map.init(SCREEN_HASH_SIZE);
    m_lts_map.init(LIGHTSET_HASH_SIZE);
}


fsDrawMgr::~fsDrawMgr()
{
    getScreen(INVISIBLE_SCREEN_ID)->moveLast();

    while (const fsID* scr_id = m_scr_map.getFirstKeyN())
    {
        fsDelete(getScreen(*scr_id), fsScr);
    }

    while (const fsID* lts_id = m_lts_map.getFirstKeyN())
    {
        deleteLightSet(*lts_id);
    }
}


FS_DEFINE_OPERATOR_EQUAL(fsDrawMgr)


FS_DEFINE_MANAGER_INSTANCE(fsDrawMgr)


void fsDrawMgr::renderScreen(fsScr* scr, fsDraw** sort_list, const fsMat& view)
{
    for (fsTree<fsDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        fsDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            draw->setupWorld();
            draw->setupFinalColor();
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }

    for (fsTree<fsDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        fsDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            if (draw->m_type != fsDraw::TYPE_NODE)
            {
                if (draw->m_draw_flag.isOn(fsDraw::FLAG_BOUND_CLIP) && //
                    scr->canBoundClip_noCalcProjection(draw->m_world, draw->m_bound_max, draw->m_bound_min))
                {
                    continue;
                }

                if (draw->isDrawFlag(fsDraw::FLAG_SORT))
                {
                    draw->setupSortValue(scr->view());

                    draw->m_next_sort = *sort_list;
                    *sort_list = draw;
                }
                else
                {
                    draw->setupDrawState();
                    draw->render(view);
                }
            }
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }
}


void fsDrawMgr::sortList(fsDraw** sorted_start, fsDraw** sorted_end, fsDraw* target_list)
{
    fsDraw* center = target_list;
    target_list = target_list->m_next_sort;

    if (!target_list)
    {
        center->m_next_sort = NULL;

        *sorted_start = center;
        *sorted_end = center;

        return;
    }

    fsDraw* left_list = NULL;
    fsDraw* right_list = NULL;
    fsDraw* next_sort;

    for (fsDraw* draw = target_list; draw; draw = next_sort)
    {
        next_sort = draw->m_next_sort;

        if (draw->m_sort_value <= center->m_sort_value)
        {
            draw->m_next_sort = left_list;
            left_list = draw;
        }
        else
        {
            draw->m_next_sort = right_list;
            right_list = draw;
        }
    }

    if (left_list)
    {
        fsDraw* start;
        fsDraw* end;

        sortList(&start, &end, left_list);

        *sorted_start = start;
        end->m_next_sort = center;
    }
    else
    {
        *sorted_start = center;
    }

    if (right_list)
    {
        fsDraw* start;
        fsDraw* end;

        sortList(&start, &end, right_list);

        *sorted_end = end;
        center->m_next_sort = start;
    }
    else
    {
        *sorted_end = center;
        center->m_next_sort = NULL;
    }
}
