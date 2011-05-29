/*
 * debug_mode.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class DebugMode : public fsTask
{
public:
    DebugMode();

private:
    virtual void onUpdate();
};


void newDebugMode()
{
    fsNewTask(DebugMode);
}


DebugMode::DebugMode() : fsTask(ORDER_ZERO) {}


void DebugMode::onUpdate()
{
    fsCol col(0, 255, 0);

    switch (fsDebugMgr::getDebugMode())
    {
    case fsDebugMgr::MODE_OFF:
        fsDebugMgr::drawString(-162.0f, 12.0f, col, 2, "Please press 'D' and '1'");
        fsDebugMgr::drawString(-162.0f, -12.0f, col, 2, " to turn on the debug mode.");
        break;

    case fsDebugMgr::MODE_MONITOR:
        fsDebugMgr::drawString(-144.0f, 40.0f, col, 2, "This is \"DEBUG MONITOR\".");
        fsDebugMgr::drawString(-186.0f, -24.0f, col, 2, "Please press 'D' and '1' again.");
        break;

    case fsDebugMgr::MODE_CONSOLE:
        fsDebugMgr::drawString(-144.0f, 90.0f, col, 2, "This is \"DEBUG CONSOLE\".");
        fsDebugMgr::drawString(-198.0f, 58.0f, col, 2, "If you press 'D' and '2'-'8',");
        fsDebugMgr::drawString(-198.0f, 38.0f, col, 2, " you can get various information.");
        fsDebugMgr::drawString(-210.0f, 2.0f, col, 2, "You can also scroll the console");
        fsDebugMgr::drawString(-210.0f, -18.0f, col, 2, " by 'D' and 'PageUp' or 'PageDown'.");
        fsDebugMgr::drawString(-180.0f, -54.0f, col, 2, "Please press 'D' and '1' again");
        fsDebugMgr::drawString(-180.0f, -74.0f, col, 2, " to turn off the debug mode.");
        break;
    }
}
