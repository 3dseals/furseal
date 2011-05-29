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
void newGolem();
void newJeep();


static const char* s_cmd[] =
{
    "L-BUTTON DRAG: ROTATE CAMERA", //
    "T: TOGGLE TEXTURE", //
    "L: TOGGLE LIGHT", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample11 - Model", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/copyright_304x34.png", true);
    fsResMgr::loadResource("../data/golem.png", true);
    fsResMgr::loadResource("../data/jeep.png", true);
    fsUtil::import3DSModel("../data/golem.3ds", fsID_("golem.png"), true, true, 1.5f);
    fsUtil::import3DSModel("../data/jeep.3ds", fsID_("jeep.png"), true, true, 0.5f);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newGolem();
    newJeep();

    fsStartFurseal();
    fsDestroyFurseal();
}
