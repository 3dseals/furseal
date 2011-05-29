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
void newScenarioPlayer();


static const char* s_cmd[] =
{
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


furseal()
{
    // special method to load a config file before initializing Furseal
    fsScript* conf = fsScript::newPriorConfigBeforeInitialization("../data/setting.fsl");

    fsScriptEntry* ent1 = conf->getEntryFromFirstN("title");
    fsScriptEntry* ent2 = conf->getEntryFromFirstN("framebuffer_size");
    fsScriptEntry* ent3 = conf->getEntryFromFirstN("frame_rate");
    fsScriptEntry* ent4 = conf->getEntryFromFirstN("resources");

    fsCreateFurseal(ent1->getValue_string(0), ent2->getValue_s32(0), ent2->getValue_s32(1), ent3->getValue_s32(0));
    fsInputMgr::setMouseVisible(false);

    for (s32 i = 0; i < ent4->getValueNum(); i++)
    {
        fsResMgr::loadResource(ent4->getValue_string(i), true);
    }

    // normal way to load config file
    fsResMgr::loadResource("../data/scenario.fsl", true);

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newScenarioPlayer();

    fsStartFurseal();
    fsDestroyFurseal();

    fsScript::deletePriorConfig(conf);
}
