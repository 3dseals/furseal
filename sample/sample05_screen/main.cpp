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
void newBlurredScreen();
void newCopyCat();
void newSubScreen();


static const char* s_cmd[] =
{
    "B: TOGGLE BLUR", //
    "C: TOGGLE COPY", //
    "S: TOGGLE SUBSCREEN", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample05 - Screen", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/mask_512x300.png", true);

    fsMath::srand(static_cast<u32>(fsMgr::getUsecTime()));

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newBlurredScreen();
    newCopyCat();
    newSubScreen();

    fsStartFurseal();
    fsDestroyFurseal();
}
