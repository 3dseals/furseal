/*
 * fs_logo.cpp
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


#include "fs.h"


class CubeTask : public fsTask
{
public:
    CubeTask();

private:
    static const u16 CIRCLE_VERT_NUM = 36;
    static const u16 AMEBA_VERT_NUM = CIRCLE_VERT_NUM + 2;
    static const u16 ONE_VERT_DEG = 360 / CIRCLE_VERT_NUM;

    virtual void onUpdate();
    fsPrim m_cube_prim;
    s32 m_phase;
};


void newCubeTask()
{
    fsNewTask(CubeTask);
}


CubeTask::CubeTask() : fsTask(ORDER_ZERO)
{
    m_cube_prim.init(fsPrim::MODE_TRIANGLE_FAN, AMEBA_VERT_NUM, fsDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_cube_prim.setBlendMode(fsDraw::BLEND_HALF, true);
    m_cube_prim.dataPos(0) = fsVec::ZERO;

	for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
	{
		fsVec dir(fsMath::cos_s32((i - 1) * ONE_VERT_DEG), fsMath::sin_s32((i - 1) * ONE_VERT_DEG));
		r32 rad = fsMath::sin_s32((i - 1) * 60) * fsMath::sin_s32(m_phase) * 8.0f + 64.0f;

		m_cube_prim.dataPos(i) = dir * rad;
	}

    m_cube_prim.dataCol(0).set(128, 128, 128, 128);
    for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
    {
    	m_cube_prim.dataCol(i).set(128, 128, 5*i, 255);
    }

    m_phase = 0;
}


void CubeTask::onUpdate()
{
	m_phase += 4;
	m_cube_prim.dataPos(0) = fsVec::ZERO;

	for (s32 i = 1; i < AMEBA_VERT_NUM; i++)
	{
		fsVec dir(fsMath::cos_s32((i - 1) * ONE_VERT_DEG), fsMath::sin_s32((i - 1) * ONE_VERT_DEG));
		r32 rad = fsMath::sin_s32((i - 1) * 60) * fsMath::sin_s32(m_phase) * 8.0f + 64.0f;

		m_cube_prim.dataPos(i) = dir * rad;
	}
}
