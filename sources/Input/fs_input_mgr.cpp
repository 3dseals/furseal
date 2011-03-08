/*
 * fs_input_mgr.cpp
 *
 *  Created on: 2011-3-8
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"
#include "base/fs_low_level_api.h"
#include "base/fs_private_macro.h"


fsInputMgr* fsInputMgr::m_instance = NULL;


FS_DEFINE_MANAGER_IS_CREATED(fsInputMgr)


FS_DEFINE_MANAGER_CREATE(fsInputMgr, AfterTask, BeforeSys)


FS_DEFINE_MANAGER_DESTROY(fsInputMgr, BeforeSys)


void fsInputMgr::defaultKeyEventHandler(KeyType key, KeyState key_state)
{
    fsInputMgr* ins = instance();

    if (key < KEY_ANY)
    {
        if (key_state == STATE_DOWN)
        {
            ins->m_key_flag[key] |= FLAG_REAL_ON | FLAG_DELAY_ON;

            if (key == KEY_WHEELUP)
            {
                ins->m_real_mouse_wheel++;
            }
            else if (key == KEY_WHEELDOWN)
            {
                ins->m_real_mouse_wheel--;
            }
        }
        else // STATE_UP
        {
            ins->m_key_flag[key] &= ~FLAG_REAL_ON;
        }
    }
}


fsInputMgr::KeyEventHandler fsInputMgr::getKeyEventHandlerN()
{
    return instance()->m_key_event_handler;
}


void fsInputMgr::defaultMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
    fsInputMgr* ins = fsInputMgr::instance();

    ins->m_mouse_x = mouse_x;
    ins->m_mouse_y = mouse_y;
}


fsInputMgr::MouseEventHandler fsInputMgr::getMouseEventHandlerN()
{
    return instance()->m_mouse_event_handler;
}


void fsInputMgr::defaultExtraEventHandler(u8 val_index, r32 value)
{
	fsInputMgr* ins = fsInputMgr::instance();

    if (val_index < EXTRA_VALUE_NUM)
    {
        ins->m_real_ext_val[val_index] = value;
    }
}


fsInputMgr::ExtraEventHandler fsInputMgr::getExtraEventHandlerN()
{
    return instance()->m_extra_event_handler;
}


void fsInputMgr::setMouseVisible(bool is_visible)
{
    return fsLowLevelAPI::setMouseVisible(is_visible);
}


void fsInputMgr::updateKeyStateForEngine()
{
    fsInputMgr* ins = instance();
    u8* key_flag = ins->m_key_flag;

    key_flag[KEY_WHEELUP] &= ~FLAG_REAL_ON;
    key_flag[KEY_WHEELDOWN] &= ~FLAG_REAL_ON;

    u8* flag;
    bool is_any_key_on = false;

    for (u32 i = 0; i < KEY_ANY; i++)
    {
        flag = &key_flag[i];

        if (*flag & FLAG_CUR_ON)
        {
            *flag |= FLAG_LAST_ON;
        }
        else
        {
            *flag &= ~FLAG_LAST_ON;
        }

        if (*flag & (FLAG_DELAY_ON | FLAG_REAL_ON))
        {
            *flag |= FLAG_CUR_ON;
            is_any_key_on = true;
        }
        else
        {
            *flag &= ~FLAG_CUR_ON;
        }

        *flag &= ~FLAG_DELAY_ON;
    }

    flag = &key_flag[KEY_ANY];

    if (*flag & FLAG_CUR_ON)
    {
        *flag |= FLAG_LAST_ON;
    }
    else
    {
        *flag &= ~FLAG_LAST_ON;
    }

    if (is_any_key_on)
    {
        *flag |= FLAG_CUR_ON;
    }
    else
    {
        *flag &= ~FLAG_CUR_ON;
    }

    ins->m_cur_mouse_wheel = ins->m_real_mouse_wheel;
    ins->m_real_mouse_wheel = 0;
}


void fsInputMgr::updateExtraValueForEngine()
{
    fsInputMgr* ins = instance();

    fsMemHelper::memcpy(ins->m_cur_ext_val, ins->m_real_ext_val, sizeof(ins->m_cur_ext_val));
}


static void lowLevelKeyEventHandler(u8 keycode, bool is_down)
{
    fsInputMgr::KeyEventHandler handler = fsInputMgr::getKeyEventHandlerN();

    if (handler)
    {
        (*handler)(static_cast<fsInputMgr::KeyType>(keycode), is_down ? fsInputMgr::STATE_DOWN : fsInputMgr::STATE_UP);
    }
}


static void lowLevelMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
	fsInputMgr::MouseEventHandler handler = fsInputMgr::getMouseEventHandlerN();

    if (handler)
    {
        (*handler)(mouse_x, mouse_y);
    }
}


static void lowLevelExtraEventHandler(u8 index, r32 value)
{
	fsInputMgr::ExtraEventHandler handler = fsInputMgr::getExtraEventHandlerN();

    if (handler)
    {
        (*handler)(index, value);
    }
}


class fsInputDaemon : public fsTask
{
public:
    fsInputDaemon() : fsTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        fsInputMgr::updateKeyStateForEngine();
        fsInputMgr::updateExtraValueForEngine();
    }
};


fsInputMgr::fsInputMgr()
{
    fsLowLevelAPI::setKeyEventHandler(lowLevelKeyEventHandler);
    fsLowLevelAPI::setMouseEventHandler(lowLevelMouseEventHandler);
    fsLowLevelAPI::setExtraEventHandler(lowLevelExtraEventHandler);

    m_key_event_handler = defaultKeyEventHandler;
    m_mouse_event_handler = defaultMouseEventHandler;
    m_extra_event_handler = defaultExtraEventHandler;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_real_mouse_wheel = m_cur_mouse_wheel = 0;

    fsMemHelper::memset(m_key_flag, 0, sizeof(m_key_flag));
    fsMemHelper::memset(m_cur_ext_val, 0, sizeof(m_cur_ext_val));
    fsMemHelper::memset(m_real_ext_val, 0, sizeof(m_real_ext_val));

    setMouseVisible(true);

    m_key_daemon = fsNewTask(fsInputDaemon);
}


fsInputMgr::~fsInputMgr()
{
    if (fsTaskMgr::isCreated())
    {
        fsDeleteTask(m_key_daemon);
    }

    fsLowLevelAPI::setKeyEventHandler(NULL);
    fsLowLevelAPI::setMouseEventHandler(NULL);
}


FS_DEFINE_OPERATOR_EQUAL(fsInputMgr)


FS_DEFINE_MANAGER_INSTANCE(fsInputMgr)
