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
void newCollisionChefser();


static const char* s_cmd[] =
{
    "BORDER DRAG: RESIZE WINDOW", //
    "MOUSE WHEEL: ZOOM CAMERA", //
    "L-BUTTON DRAG: MOVE OBJECT/CAMERA", //
    "M-BUTTON DRAG: ZOOM OBJECT/CAMERA", //
    "R-BUTTON DRAG: ROTATE OBJECT/CAMERA", //
    "A: CHANGE OBJECT1 TYPE", //
    "B: CHANGE OBJECT2 TYPE", //
    "P: TOGGLE PUSH BACK", //
    "R: RESET POSITION", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample09 - Collision", 800, 600, 60, fsMgr::FLAG_VARIABLE_SIZE);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newCollisionChefser();

    fsStartFurseal();
    fsDestroyFurseal();
}
