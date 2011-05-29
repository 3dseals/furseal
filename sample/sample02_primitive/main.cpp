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
void newTriangle();
void newAmeba(r32 x, r32 y, fsCol color);


static const char* s_cmd[] =
{
    "P: CHANGE PRIM MODE", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample02 - Primitive", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/coin_400x200.png", true);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newTriangle();
    newAmeba(0.0f, 0.0f, fsCol(128, 128, 128));
    newAmeba(-200.0f, 120.0f, fsCol(255, 128, 64));
    newAmeba(0.0f, 240.0f, fsCol(64, 255, 128));
    newAmeba(200.0f, 120.0f, fsCol(128, 64, 255));
    newAmeba(-200.0f, -120.0f, fsCol(255, 64, 128));
    newAmeba(0.0f, -240.0f, fsCol(128, 255, 64));
    newAmeba(200.0f, -120.0f, fsCol(64, 128, 255));

    fsStartFurseal();
    fsDestroyFurseal();
}
