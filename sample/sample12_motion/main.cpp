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
void newZombie(u16 zombie_no, r32 x, r32 y, r32 z);


static const char* s_cmd[] =
{
    "ARROW: ROTATE CAMERA", //
    "CLICK ZOMBIE: SHOT ZOMBIE", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


static void initMotData()
{
    fsMotData mot_data;
    fsMat local;

    mot_data.initAsWriter(6, 3, 1 + 3 + 5);

    /*
        motion 0: normal pose
    */
    mot_data.setMotionInfo(0, 1, 0, 1);
    mot_data.setKeyFramePlayFrame(0, 0);

    /*
        motion 1: walk
    */
    mot_data.setMotionInfo(1, 180, 1, 3);

    mot_data.setKeyFramePlayFrame(1, 0);
    mot_data.setKeyFramePlayFrame(2, 90);
    mot_data.setKeyFramePlayFrame(3, 180);

    mot_data.setNodeLocal(0, 1, fsMat::UNIT.rotateZ_s32(5));
    mot_data.setNodeLocal(0, 2, fsMat::UNIT.rotateZ_s32(-5));
    mot_data.setNodeLocal(0, 3, fsMat::UNIT.rotateZ_s32(5));

    mot_data.setNodeLocal(1, 1, fsMat::UNIT.rotateX_s32(-10));
    mot_data.setNodeLocal(1, 2, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 3, fsMat::UNIT.rotateX_s32(-10));

    mot_data.setNodeLocal(2, 1, fsMat::UNIT.rotateX_s32(3));
    mot_data.setNodeLocal(2, 2, fsMat::UNIT.rotateX_s32(-3));
    mot_data.setNodeLocal(2, 3, fsMat::UNIT.rotateX_s32(3));

    mot_data.setNodeLocal(3, 1, fsMat::UNIT.rotateX_s32(8));
    mot_data.setNodeLocal(3, 2, fsMat::UNIT.rotateX_s32(-5));
    mot_data.setNodeLocal(3, 3, fsMat::UNIT.rotateX_s32(8));

    mot_data.setNodeLocal(4, 1, fsMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(4, 2, fsMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(4, 3, fsMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));

    mot_data.setNodeLocal(5, 1, fsMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(5, 2, fsMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(5, 3, fsMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));

    /*
        motion 2: fall down
    */
    mot_data.setMotionInfo(2, 40, 4, 5);

    mot_data.setKeyFramePlayFrame(4, 0);
    mot_data.setKeyFramePlayFrame(5, 10);
    mot_data.setKeyFramePlayFrame(6, 20);
    mot_data.setKeyFramePlayFrame(7, 30);
    mot_data.setKeyFramePlayFrame(8, 40);

    mot_data.setNodeLocal(0, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(0, 5, fsMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(0, 6, fsMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(0, 7, fsMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(0, 8, fsMat::UNIT.rotateX_s32(10));

    mot_data.setNodeLocal(1, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 5, fsMat::UNIT.translate(0.0f, 16.0f, 56.0f).rotateX_s32(30));
    mot_data.setNodeLocal(1, 6, fsMat::UNIT.translate(0.0f, 8.0f, 88.0f).rotateX_s32(40));
    mot_data.setNodeLocal(1, 7, fsMat::UNIT.translate(0.0f, -16.0f, 108.0f).rotateX_s32(60));
    mot_data.setNodeLocal(1, 8, fsMat::UNIT.translate(0.0f, -32.0f, 116.0f).rotateX_s32(90));

    mot_data.setNodeLocal(2, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(2, 5, fsMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(2, 6, fsMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(2, 7, fsMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(2, 8, fsMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(3, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(3, 5, fsMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(3, 6, fsMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(3, 7, fsMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(3, 8, fsMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(4, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(4, 5, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 6, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 7, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 8, fsMat::UNIT.rotateX_s32(-20));

    mot_data.setNodeLocal(5, 4, fsMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(5, 5, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 6, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 7, fsMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 8, fsMat::UNIT.rotateX_s32(-20));

    /*
        register motion data
    */
    mot_data.registerAsResource(fsID_("zombie.mot"));
}


furseal()
{
    fsCreateFurseal("Sample12 - Motion", 640, 480, 60);

    fsResMgr::loadResource("../data/fs_logo_71x14.png", true);
    fsUtil::loadPixelArtModelAs(fsID_("fs_icon.mdl"), "../data/fs_icon_model.fsl", "../data/fs_icon_model.png", 4.0f);
    fsUtil::loadPixelArtModelAs(fsID_("zombie.mdl"), "../data/zombie_model.fsl", "../data/zombie_model.png", 8.0f);
    fsUtil::loadPixelArtModelAs(fsID_("blood.mdl"), "../data/blood_model.fsl", "../data/blood_model.png", 4.0f);

    initMotData();

    newFursealLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newStage();
    newZombie(0, -20.0f, 64.0f, -150.0f);
    newZombie(1, 100.0f, 64.0f, 50.0f);
    newZombie(2, -120.0f, 64.0f, 150.0f);

    fsStartFurseal();
    fsDestroyFurseal();
}
