/*
 * main.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_main.h"


void newFursealLogo(const char** cmd, u8 cmd_num);
void newDebugDraw();
void newDebugMode();


static const char* s_cmd[] =
{
    "V: DUMP VECTOR", //
    "M: DUMP MATRIX", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample01 - Debug", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newDebugDraw();
    newDebugMode();

    fsStartFurseal();
    fsDestroyFurseal();
}
