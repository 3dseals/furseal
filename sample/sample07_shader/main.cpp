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
void newEmbossFont();
void newWaterdrop(r32 x, r32 y);


static const char* s_cmd[] =
{
    "U: ROTATE/STOP UV OFFSET", //
    "R: CHANGE REFRACTION INDEX", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample07 - Shader", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/bg_512x512.png", true);
    fsResMgr::loadResource("../data/stonsans.ttf", true);
    fsResMgr::loadResource("../data/waterdrop_64x64.png", true);
    fsUtil::loadShader(fsID_("refract.shd"), "../data/refract.vert", "../data/refract.frag", 1, 2, 2);

    fsMath::srand(static_cast<u32>(fsMgr::getUsecTime()));

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newEmbossFont();

    for (s32 i = 0; i < 64; i++)
    {
        newWaterdrop(fsMath::rand(-320.0f, 320.0f, 1.0f), fsMath::rand(-320.0f, 320.0f, 1.0f));
    }

    fsStartFurseal();
    fsDestroyFurseal();
}
