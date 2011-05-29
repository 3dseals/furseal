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
void newCat();
void newCoin();


static const char* s_cmd[] =
{
    "MOUSE OVER: ZOOM COIN", //
    "A: CHANGE ALPHA", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample03 - Sprite", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/stephanie_275x196.png", true);
    fsResMgr::loadResource("../data/coin_400x200.png", true);

    fsMath::srand(static_cast<u32>(fsMgr::getUsecTime()));

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newCat();
    newCoin();

    fsStartFurseal();
    fsDestroyFurseal();
}
