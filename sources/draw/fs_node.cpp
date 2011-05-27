/*
 * fs_node.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "base/fs_private_macro.h"


fsNode::fsNode()
{
    m_type = TYPE_NODE;
}


fsNode::~fsNode() {}


void fsNode::init(fsID scr_id)
{
    m_private_flag.setOn(FLAG_INITIALIZED);

    setScreenID(scr_id);
}


void fsNode::init(fsDraw* parent)
{
    m_private_flag.setOn(FLAG_INITIALIZED);

    setParent(parent);
}
