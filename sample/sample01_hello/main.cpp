/*
    Copyright (c) 2011 netsurfers
*/


#include "fs_main.h"


furseal()
{
    fsCreateFurseal("Sample01 - Hello", 640, 480, 30);

    fsStartFurseal();
    fsDestroyFurseal();
}
