/*
 * main.cpp
 *
 *  Created on: 2011-3-17
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


#include "fs_main.h"


void newHelloFurseal();


furseal()
{
    fsCreateFurseal("Hello Furseal", 320, 480, 30);

    newHelloFurseal();

    /*
        ckStartFuseal doesn't block in Furseal for Android.
        So you must not write finalization process such as ckDestroyFurseal after it.
    */
    fsStartFurseal();
}
