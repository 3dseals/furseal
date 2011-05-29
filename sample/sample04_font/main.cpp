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
void newFontDraw();
void newMassFont();


static const char* s_cmd[] =
{
    "V: CHANGE VIEW", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample04 - Font", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/stonsans.ttf", true);
#ifdef WIN32
    fsUtil::loadWindowsFont("msgothic.ttc");
#endif

    fsMath::srand(static_cast<u32>(fsMgr::getUsecTime()));

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newFontDraw();

    for (s32 i = 0; i < 50; i++)
    {
        newMassFont();
    }

    fsStartFurseal();
    fsDestroyFurseal();
}
