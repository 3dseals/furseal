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
void newStage();
void newTetrahedron();
void newFog();


static const char* s_cmd[] =
{
    "ARROW: ROTATE CAMERA", //
    "L: CHANGE LIGHT SET", //
    "B: CHANGE BLEND MODE", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample08 - Light", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/fog_128x128.png", true);

    fsMath::srand(static_cast<u32>(fsMgr::getUsecTime()));

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newStage();
    newTetrahedron();
    newFog();

    fsStartFurseal();
    fsDestroyFurseal();
}
