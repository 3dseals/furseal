/*
 * fs_body.cpp
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#include "fs_physics_all.h"
#include "base/fs_private_macro.h"



fsBody::fsBody(int body_count, int pos_num, int vel_num)
{
    m_body_count = body_count;
    m_pos = new float[pos_num];
    m_vel = new float[vel_num];
    m_start_index_pos = 0;
    m_start_index_vel = 0;
    changed_pos = true;
    changed_vel = true;
}

fsBody::~fsBody()
{
	if(this->m_body_count != 0)
	{
		delete [] m_pos;
		delete [] m_vel;
	}
}


fsBody::fsBody()
{
    this->m_body_count = 0;
    m_pos = NULL;
    m_vel = NULL;
    m_start_index_pos = 0;
    m_start_index_vel = 0;
    changed_pos = true;
    changed_vel = true;
}


FS_DEFINE_COPY_CONSTRUCTOR(fsBody)


FS_DEFINE_OPERATOR_EQUAL(fsBody)
