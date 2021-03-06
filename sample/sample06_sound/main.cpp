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
void newSoundMonitor();
void newSoundMixer();


static const char* s_cmd[] =
{
    "CLICK & DRAG: CONTROL SOUND", //
    "1-6: CHANGE SOUND QUALITY", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    fsCreateFurseal("Sample06 - Sound", 640, 480, 30);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsResMgr::loadResource("../data/mono_11khz.wav", true);
    fsResMgr::loadResource("../data/mono_22khz.wav", true);
    fsResMgr::loadResource("../data/mono_44khz.wav", true);
    fsResMgr::loadResource("../data/stereo_11khz.wav", true);
    fsResMgr::loadResource("../data/stereo_22khz.wav", true);
    fsResMgr::loadResource("../data/stereo_44khz.wav", true);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newSoundMonitor();
    newSoundMixer();

    fsStartFurseal();
    fsDestroyFurseal();
}
