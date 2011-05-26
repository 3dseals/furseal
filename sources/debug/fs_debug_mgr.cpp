/*
 * fs_debug_mgr.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */



#include <stdarg.h>

#include "fs_debug_all.h"

#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_base_all.h"


const fsID fsDebugMgr::DEBUG_MODE_SCREEN_ID = fsID_("DEBUG_MODE_SCREEN");
const fsID fsDebugMgr::DEBUG_FONT_TEXTURE_ID = fsID_("DEBUG_FONT_TEXTURE");
fsDebugMgr* fsDebugMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsDebugMgr)


FS_DEFINE_MANAGER_CREATE(fsDebugMgr, Last, Second)


FS_DEFINE_MANAGER_DESTROY(fsDebugMgr, Second)


fsDebugMgr::DebugMode fsDebugMgr::getDebugMode()
{
    if (!m_instance)
    {
        return MODE_OFF;
    }

    return instance()->m_dbg_mode.getType();
}


void fsDebugMgr::setDebugMode(DebugMode dbg_mode)
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    if (ins->m_dbg_mode == MODE_OFF)
    {
        ins->m_aim_row = ins->m_cur_row = ins->m_end_row;
        ins->m_dbg_mode_scr->moveLast();
    }

    instance()->m_dbg_mode = dbg_mode;
}


void fsDebugMgr::pageUpConsole()
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    if (ins->m_cur_row >= row_num)
    {
        ins->m_aim_row = ins->m_cur_row - row_num;
    }
}


void fsDebugMgr::pageDownConsole()
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    ins->m_aim_row = ins->m_cur_row + row_num;

    if (ins->m_aim_row > ins->m_end_row)
    {
        ins->m_aim_row = ins->m_end_row;
    }
}


void fsDebugMgr::trace(const char* str, ...)
{
    if (!m_instance)
    {
        return;
    }

    fsDebugMgr* ins = instance();

    if (!str)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    FS_VSPRINTF(buf, 256, str);

    fsMgr::printf(buf);

    for (char* c = buf; *c != '\0'; c++)
    {
        u32 index = ins->m_end_row % CONSOLE_BUFFER_SIZE;

        if (*c == '\n' || ins->m_console_buf[index].getLength() >= CONSOLE_COLUMN_NUM)
        {
            ins->m_end_row++;
            index = ins->m_end_row % CONSOLE_BUFFER_SIZE;

            if (*c == '\n')
            {
                ins->m_console_buf[index] = "";
            }
            else
            {
                ins->m_console_buf[index] = *c;
            }

            if (ins->m_end_row == ins->m_aim_row + 1)
            {
                ins->m_aim_row = ins->m_end_row;
            }
        }
        else if (*c == '\t')
        {
            u32 space_num = CONSOLE_TAB_WIDTH - ins->m_console_buf[index].getLength() % CONSOLE_TAB_WIDTH;

            for (u32 i = 0; i < space_num; i++)
            {
                ins->m_console_buf[index] += " ";
            }
        }
        else
        {
            ins->m_console_buf[index] += *c;
        }
    }
}


class fsDebugDaemon : public fsTask
{
public:
    fsDebugDaemon() : fsTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        if (!fsDebugMgr::m_instance)
        {
            return;
        }

        fsDebugMgr* ins = fsDebugMgr::instance();

        if (fsMgr::isFramebufferSizeChanged())
        {
            ins->m_dbg_mode_scr->setAreaInFramebuffer(0, 0, fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
            ins->m_dbg_mode_scr->setViewSize(fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
        }

        u32 cur_draw_prim_num = ins->m_cur_draw_prim_num;

        for (u32 i = 0; i < cur_draw_prim_num; i++)
        {
            ins->m_draw_prim[i].setScreenID(fsDrawMgr::INVISIBLE_SCREEN_ID); // TODO
        }

        ins->m_cur_draw_prim_num = 0;
        ins->m_font_sprt.setCurDataNum(0);

        ins->chefsSpecialCommand();

        if (ins->m_dbg_mode != fsDebugMgr::MODE_OFF)
        {
            ins->drawConsole();
        }
    }
};


fsDebugMgr::fsDebugMgr()
{
    m_dbg_mode = MODE_OFF;
    m_draw_prim = NULL;
    m_cur_draw_prim_num = m_max_draw_prim_num = 0;
    m_end_row = m_aim_row = m_cur_row = 0;
    m_dbg_mode_tap_cntr = 0;
    m_dbg_dump_tap_cntr = 0;
    m_scroll_hold_cntr = 0;

    newDebugFontTexture();

    m_dbg_mode_scr = fsDrawMgr::newScreen(DEBUG_MODE_SCREEN_ID);
    m_dbg_mode_scr->setClearMode(false, true);
    m_dbg_mode_scr->setPerspective(false);

    reallocDrawPrim();

    m_font_sprt.init(INITIAL_FONT_SPRT_NUM, DEBUG_MODE_SCREEN_ID);
    m_font_sprt.setTextureID(DEBUG_FONT_TEXTURE_ID);
    m_font_sprt.setCurDataNum(0);
    m_font_sprt.setBlendMode(fsDraw::BLEND_HALF, true);
    m_font_sprt.setDrawFlag(fsDraw::FLAG_BILINEAR, false);

    m_dbg_daemon = fsNewTask(fsDebugDaemon);
}


fsDebugMgr::~fsDebugMgr()
{
    if (fsTaskMgr::isCreated())
    {
        fsDeleteTask(m_dbg_daemon);
    }

    fsDeleteArray(m_draw_prim, fsPrim);

    fsDrawMgr::deleteScreen(DEBUG_MODE_SCREEN_ID);
    fsDrawMgr::deleteTexture(DEBUG_FONT_TEXTURE_ID);
}


FS_DEFINE_OPERATOR_EQUAL(fsDebugMgr)


void fsDebugMgr::reallocDrawPrim()
{
    u16 new_draw_prim_num;
    fsPrim* new_draw_prim;

    if (m_draw_prim)
    {
        new_draw_prim_num = m_max_draw_prim_num * 2;
    }
    else
    {
        new_draw_prim_num = INITIAL_DRAW_PRIM_NUM;
    }

    fsNewArray(new_draw_prim, fsPrim, new_draw_prim_num);

    for (u32 i = 0; i < new_draw_prim_num; i++)
    {
        fsPrim* prim = &new_draw_prim[i];

        prim->init(fsPrim::MODE_LINES, 4, fsDrawMgr::INVISIBLE_SCREEN_ID);
        prim->setBlendMode(fsDraw::BLEND_HALF, true);
    }

    if (m_draw_prim)
    {
        for (u32 i = 0; i < m_cur_draw_prim_num; i++)
        {
            fsPrim* src_prim = &m_draw_prim[i];
            fsPrim* dest_prim = &new_draw_prim[i];

            u16 cur_data_num = src_prim->getCurDataNum();

            dest_prim->setScreenID(src_prim->getScreenID());
            dest_prim->setPrimMode(src_prim->getPrimMode());
            dest_prim->setCurDataNum(cur_data_num);
            dest_prim->setSortCenter(dest_prim->getSortCenter());

            for (u32 j = 0; j < cur_data_num; j++)
            {
                dest_prim->copyData(j, src_prim, j);
            }
        }

        fsDeleteArray(m_draw_prim, fsPrim);
    }

    m_draw_prim = new_draw_prim;
    m_max_draw_prim_num = new_draw_prim_num;
}


void fsDebugMgr::chefsSpecialCommand()
{
    u8 cmd_no = 0;

    if (fsInputMgr::isOn(fsInputMgr::KEY_D))
    {
        if (fsInputMgr::isPressed(fsInputMgr::KEY_1))
        {
            cmd_no = 1;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_2))
        {
            cmd_no = 2;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_3))
        {
            cmd_no = 3;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_4))
        {
            cmd_no = 4;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_5))
        {
            cmd_no = 5;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_6))
        {
            cmd_no = 6;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_7))
        {
            cmd_no = 7;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_8))
        {
            cmd_no = 8;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_PAGEUP))
        {
            cmd_no = 100;
        }
        else if (fsInputMgr::isPressed(fsInputMgr::KEY_PAGEDOWN))
        {
            cmd_no = 101;
        }
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_LBUTTON))
    {
        r32 mouse_x = m_dbg_mode_scr->framebufferXToScreenX(fsInputMgr::getMouseX());
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(fsInputMgr::getMouseY());

        if (mouse_x > 0.0f && mouse_y > 0.0f)
        {
            if (m_dbg_mode_tap_cntr < 2)
            {
                m_dbg_mode_tap_cntr++;
            }
            else if (m_dbg_mode_tap_cntr > 2)
            {
                m_dbg_mode_tap_cntr = 1;
            }
        }
        else if ((m_dbg_mode_tap_cntr >= 2 && m_dbg_mode_tap_cntr <= 3 && mouse_x < 0.0f && mouse_y > 0.0f) || //
            (m_dbg_mode_tap_cntr >= 4 && m_dbg_mode_tap_cntr <= 5 && mouse_x < 0.0f && mouse_y < 0.0f) || //
            (m_dbg_mode_tap_cntr >= 6 && m_dbg_mode_tap_cntr <= 7 && mouse_x > 0.0f && mouse_y < 0.0f))
        {
            m_dbg_mode_tap_cntr++;
        }
        else
        {
            m_dbg_mode_tap_cntr = 0;
        }

        if (m_dbg_mode_tap_cntr == 8)
        {
            cmd_no = 1;

            m_dbg_mode_tap_cntr = 0;
            m_dbg_dump_tap_cntr = 0;
            m_scroll_hold_cntr = 0;
        }

        if (m_dbg_mode.getType() == MODE_CONSOLE)
        {
            if (mouse_x > 0.0f && mouse_y > 0.0f)
            {
                if (m_dbg_dump_tap_cntr >= 3 && m_dbg_dump_tap_cntr < 100)
                {
                    cmd_no = m_dbg_dump_tap_cntr - 1;

                    m_dbg_mode_tap_cntr = 0;
                    m_scroll_hold_cntr = 0;
                }

                m_dbg_dump_tap_cntr = 1;
            }
            else if (mouse_x < 0.0f && mouse_y < 0.0f)
            {
                m_dbg_dump_tap_cntr++;
            }
            else
            {
                m_dbg_dump_tap_cntr = 0;
            }
        }
    }

    if (m_dbg_mode.getType() == MODE_CONSOLE)
    {
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(fsInputMgr::getMouseY());

        if (fsInputMgr::isOn(fsInputMgr::KEY_LBUTTON))
        {
            if (mouse_y > 0.0f)
            {
                m_scroll_hold_cntr++;
            }
            else
            {
                m_scroll_hold_cntr--;
            }

            if (fsMath::abs(m_scroll_hold_cntr) >= fsTaskMgr::getAimFPS())
            {
                cmd_no = (m_scroll_hold_cntr > 0) ? 100 : 101;

                m_dbg_mode_tap_cntr = 0;
                m_dbg_dump_tap_cntr = 0;
                m_scroll_hold_cntr = 0;
            }
        }
        else
        {
            m_scroll_hold_cntr = 0;
        }
    }

    switch (cmd_no)
    {
    case 1:
        if (m_dbg_mode.getType() == MODE_OFF)
        {
            setDebugMode(MODE_MONITOR);
        }
        else if (m_dbg_mode.getType() == MODE_MONITOR)
        {
            setDebugMode(MODE_CONSOLE);
        }
        else
        {
            setDebugMode(MODE_OFF);
        }

        break;

    case 2:
        dumpMemory();
        break;

    case 3:
        dumpTask();
        break;

    case 4:
        dumpResource();
        break;

    case 5:
        dumpConfig();
        break;

    case 6:
        dumpScreen();
        break;

    case 7:
        dumpTexture();
        break;

    case 8:
        dumpShader();
        break;

    case 100:
        pageUpConsole();
        break;

    case 101:
        pageDownConsole();
        break;

    default:
        break;
    }
}


void fsDebugMgr::drawConsole()
{
    static const r32 FONT_WIDTH = static_cast<r32>(DEBUG_FONT_WIDTH);
    static const r32 FONT_HEIGHT = static_cast<r32>(DEBUG_FONT_HEIGHT);

    static const r32 LEFT_MARGIN = 9.0f;
    static const r32 RIGHT_MARGIN = 7.0f;
    static const r32 TOP_MARGIN = 7.0f;
    static const r32 BOTTOM_MARGIN = 7.0f;

    static const r32 FRAME_MARGIN = 4.0f;

    /*
        draw background
    */
    r32 scr_width = m_dbg_mode_scr->getViewWidth();
    r32 scr_height = m_dbg_mode_scr->getViewHeight();

    s32 row_num = (m_dbg_mode == MODE_CONSOLE) ? calcConsoleRowNum() : 0;

    r32 mon_width = FONT_WIDTH * CONSOLE_COLUMN_NUM + LEFT_MARGIN + RIGHT_MARGIN;
    r32 mon_height = FONT_HEIGHT * (3 + row_num) + TOP_MARGIN + BOTTOM_MARGIN;

    r32 bar_width = FONT_WIDTH * CONSOLE_COLUMN_NUM - 1.0f;

    r32 left = (mon_width > scr_width) ? -scr_width / 2.0f : -mon_width / 2.0f;
    r32 top = (mon_height > scr_height) ? scr_height / 2.0f : mon_height / 2.0f;

    r32 frame_left = left + FRAME_MARGIN;
    r32 frame_top = top - FRAME_MARGIN;
    r32 frame_width = mon_width - FRAME_MARGIN * 2.0f;
    r32 frame_height = mon_height - FRAME_MARGIN * 2.0f;

    drawRect(frame_left, frame_top, frame_width, frame_height, -1.0f, fsCol(0, 0, 0, 128));
    drawFrame(frame_left, frame_top, frame_width, frame_height, 0.0f, fsCol(192, 192, 192, 128));

    /*
        draw monitor
    */
    left += LEFT_MARGIN;
    top -= TOP_MARGIN;

    drawString(left, top, fsCol(255, 192, 192, 192), 1, "%.2f FPS", fsTaskMgr::getCurFPS());

    top -= FONT_HEIGHT;

    r32 execute_rate = static_cast<r32>(static_cast<r64>(fsTaskMgr::getExecuteUsecTime()) * fsTaskMgr::getAimFPS() / 1000000.0);
    drawRect(left, top, execute_rate * bar_width, 2.0f, 0.0f, fsCol(255, 0, 0, 192));

    top -= FONT_HEIGHT * 0.5f;

    drawString(left, top, fsCol(192, 255, 192, 192), 1, "%d/%d byte", fsMemHelper::getCurUsedMemorySize(), fsMemHelper::getMaxUsedMemorySize());

    top -= FONT_HEIGHT;

    r32 memory_rate = static_cast<r32>(fsMemHelper::getCurUsedMemorySize()) / fsMemHelper::getMaxUsedMemorySize();
    drawRect(left, top, memory_rate * bar_width, 2.0f, 0.0f, fsCol(0, 255, 0, 192));

    top -= FONT_HEIGHT * 0.5f;

    /*
        draw console
    */
    s32 diff = m_aim_row - m_cur_row;

    if (diff > 0 && diff < 4)
    {
        m_cur_row++;
    }
    else if (diff < 0 && diff > -4)
    {
        m_cur_row--;
    }
    else
    {
        m_cur_row += diff / 4;
    }

    if (m_end_row - m_cur_row + row_num > static_cast<s32>(CONSOLE_BUFFER_SIZE))
    {
        m_cur_row = m_end_row + row_num - CONSOLE_BUFFER_SIZE;
    }

    s32 start_row = (m_cur_row < row_num) ? 0 : m_cur_row - row_num + 1;

    for (s32 i = 0; i < row_num; i++)
    {
        s32 row = start_row + i;

        if (row > m_end_row)
        {
            break;
        }

        fsCol col = (row + row_num > m_end_row) ? fsCol(255, 255, 255, 192) : fsCol(192, 192, 255, 192);

        drawString(left, top - FONT_HEIGHT * i, col, 1, m_console_buf[row % CONSOLE_BUFFER_SIZE].getString());
    }
}


void fsDebugMgr::drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, fsCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    fsVec vec1 = fsVec(x1, y1, z);
    fsVec vec2 = fsVec(x1, y2, z);
    fsVec vec3 = fsVec(x2, y1, z);
    fsVec vec4 = fsVec(x2, y2, z);

    fsDebugMgr::drawPolygon(vec1, vec2, vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
}


void fsDebugMgr::drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, fsCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    fsVec vec1 = fsVec(x1, y1, z);
    fsVec vec2 = fsVec(x1, y2, z);
    fsVec vec3 = fsVec(x2, y2, z);
    fsVec vec4 = fsVec(x2, y1, z);

    drawLine(vec1, vec2, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec2, vec3, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec4, vec1, col, DEBUG_MODE_SCREEN_ID);
}


s32 fsDebugMgr::calcConsoleRowNum()
{
    return static_cast<s32>(m_dbg_mode_scr->getViewHeight() / DEBUG_FONT_HEIGHT) - 5;
}


FS_DEFINE_MANAGER_INSTANCE(fsDebugMgr)
