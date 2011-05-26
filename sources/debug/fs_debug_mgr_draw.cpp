/*
 * fs_debug_mgr_draw.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_debug_all.h"


void fsDebugMgr::drawLine(const fsVec& pos1, const fsVec& pos2, fsCol col, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    fsPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(fsPrim::MODE_LINES);
    prim->setCurDataNum(2);
    prim->setSortCenter((pos1 + pos2) / 2.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;

    ins->m_cur_draw_prim_num++;
}


void fsDebugMgr::drawPolygon(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3, fsCol col, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    fsPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(fsPrim::MODE_TRIANGLES);
    prim->setCurDataNum(3);
    prim->setSortCenter((pos1 + pos2 + pos3) / 3.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;
    prim->dataPos(2) = pos3;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;
    prim->dataCol(2) = col;

    ins->m_cur_draw_prim_num++;
}


void fsDebugMgr::drawPolygon(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3, const fsVec& pos4, fsCol col, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    fsPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(fsPrim::MODE_TRIANGLE_STRIP);
    prim->setCurDataNum(4);
    prim->setSortCenter((pos1 + pos2 + pos3 + pos4) / 4.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;
    prim->dataPos(2) = pos3;
    prim->dataPos(3) = pos4;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;
    prim->dataCol(2) = col;
    prim->dataCol(3) = col;

    ins->m_cur_draw_prim_num++;
}


void fsDebugMgr::drawBox(const fsMat& center, const fsVec& size, fsCol poly_col, fsCol line_col, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsVec vec_x = center.x_axis * (size.x / 2.0f);
    fsVec vec_y = center.y_axis * (size.y / 2.0f);
    fsVec vec_z = center.z_axis * (size.z / 2.0f);

    fsVec pos1 = center.trans + vec_x + vec_y + vec_z;
    fsVec pos2 = center.trans - vec_x + vec_y + vec_z;
    fsVec pos3 = center.trans + vec_x - vec_y + vec_z;
    fsVec pos4 = center.trans - vec_x - vec_y + vec_z;
    fsVec pos5 = center.trans + vec_x + vec_y - vec_z;
    fsVec pos6 = center.trans - vec_x + vec_y - vec_z;
    fsVec pos7 = center.trans + vec_x - vec_y - vec_z;
    fsVec pos8 = center.trans - vec_x - vec_y - vec_z;

    if (poly_col.a > 0)
    {
        drawPolygon(pos2, pos4, pos1, pos3, poly_col, scr_id);
        drawPolygon(pos5, pos7, pos6, pos8, poly_col, scr_id);
        drawPolygon(pos1, pos3, pos5, pos7, poly_col, scr_id);
        drawPolygon(pos6, pos8, pos2, pos4, poly_col, scr_id);
        drawPolygon(pos6, pos2, pos5, pos1, poly_col, scr_id);
        drawPolygon(pos4, pos8, pos3, pos7, poly_col, scr_id);
    }

    if (line_col.a > 0)
    {
        drawLine(pos2, pos4, line_col, scr_id);
        drawLine(pos4, pos3, line_col, scr_id);
        drawLine(pos1, pos2, line_col, scr_id);
        drawLine(pos3, pos1, line_col, scr_id);

        drawLine(pos5, pos7, line_col, scr_id);
        drawLine(pos7, pos8, line_col, scr_id);
        drawLine(pos6, pos5, line_col, scr_id);
        drawLine(pos8, pos6, line_col, scr_id);

        drawLine(pos3, pos7, line_col, scr_id);
        drawLine(pos5, pos1, line_col, scr_id);
        drawLine(pos2, pos6, line_col, scr_id);
        drawLine(pos8, pos4, line_col, scr_id);
    }
}


void fsDebugMgr::drawSphere(const fsVec& center, r32 radius, fsCol poly_col, fsCol line_col, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsScr* scr = fsDrawMgr::getScreen(scr_id);

    const fsVec& view_x = scr->view().x_axis;
    const fsVec& view_y = scr->view().y_axis;
    const fsVec& view_z = scr->view().z_axis;

    fsVec front = center + view_z * radius;
    fsVec bafs = center - view_z * radius;

    fsVec pos1;
    fsVec pos2 = center + view_x * (fsMath::cos_s32(24 * 14) * radius) + view_y * (fsMath::sin_s32(24 * 14) * radius);

    for (s32 i = 0; i < 15; i++)
    {
        pos1 = pos2;
        pos2 = center + view_x * (fsMath::cos_s32(24 * i) * radius) + view_y * (fsMath::sin_s32(24 * i) * radius);

        if (poly_col.a > 0)
        {
            drawPolygon(pos1, pos2, front, poly_col, scr_id);
            drawPolygon(pos1, pos2, bafs, poly_col, scr_id);
        }

        if (line_col.a > 0)
        {
            drawLine(pos1, pos2, line_col, scr_id);
        }
    }
}


void fsDebugMgr::drawAxis(const fsMat& world, r32 size, fsID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == fsID::ZERO)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    size /= 2.0f;

    drawLine(world.trans, world.trans + world.x_axis * size, fsCol(255, 0, 0), scr_id);
    drawLine(world.trans, world.trans + world.y_axis * size, fsCol(0, 255, 0), scr_id);
    drawLine(world.trans, world.trans + world.z_axis * size, fsCol(0, 0, 255), scr_id);
}
