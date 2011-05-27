/*
 * fs_input_mgr.h
 *
 *  Created on: 2011-3-8
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_INPUT_MGR_H_
#define FS_INPUT_MGR_H_


class fsInputDaemon;


/*!
    @ingroup fsInput
    引擎的输入管理器.
*/
class FS_API fsInputMgr
{
    friend class fsInputDaemon;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond


    /*!
        额外的数组个数.
    */
    static const u32 EXTRA_VALUE_NUM = 16;


    /*!
        键值.
    */
    enum KeyType
    {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, //

        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, //
        KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, //

        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, //

        KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, //
        KEY_SHIFT, KEY_CTRL, KEY_ALT, //

        KEY_BACKSPACE, KEY_TAB, KEY_ENTER, KEY_ESCAPE, KEY_SPACE, //
        KEY_PAGEUP, KEY_PAGEDOWN, KEY_END, KEY_HOME, KEY_INSERT, KEY_DELETE, //

        KEY_NUMPAD0, KEY_NUMPAD1, KEY_NUMPAD2, KEY_NUMPAD3, KEY_NUMPAD4, //
        KEY_NUMPAD5, KEY_NUMPAD6, KEY_NUMPAD7, KEY_NUMPAD8, KEY_NUMPAD9, //
        KEY_MULTIPLY, KEY_ADD, KEY_SEPARATOR, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE, //

        KEY_LBUTTON, KEY_MBUTTON, KEY_RBUTTON, KEY_WHEELUP, KEY_WHEELDOWN, //

        KEY_EXT_00, KEY_EXT_01, KEY_EXT_02, KEY_EXT_03, KEY_EXT_04, KEY_EXT_05, KEY_EXT_06, KEY_EXT_07, //
        KEY_EXT_08, KEY_EXT_09, KEY_EXT_10, KEY_EXT_11, KEY_EXT_12, KEY_EXT_13, KEY_EXT_14, KEY_EXT_15, //

        KEY_ANY, KEY_NONE
    };

    /*!
        按键状态.
    */
    enum KeyState
    {
        STATE_DOWN, //!< A key is on.
        STATE_UP //!< A key is off.
    };


    /*!
        按键事件类型.
        @param[in] key 键值.
        @param[in] key_state 按键状态.
    */
    typedef void (*KeyEventHandler)(KeyType key, KeyState key_state);


    /*!
        鼠标事件类型.
        @param[in] mouse_x 鼠标x轴坐标.
        @param[in] mouse_y 鼠标y轴坐标.
    */
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);


    /*!
        额外的事件类型.
        @param[in] val_index 值的索引.
        @param[in] value 额外的值.
    */
    typedef void (*ExtraEventHandler)(u8 val_index, r32 value);


    /*!
        判断输入管理器是否创建.
        @return 输入管理器是否创建.
    */
    static bool isCreated();


    /*!
        创建输入管理器,该方法只被引擎自己调用.
    */
    static void createAfterTask();


    /*!
        销毁输入管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeSys();


    /*!
        Returns the key event handler. If the key event handler doen't exist, returns NULL.
        @return The key event handler.
    */
    static KeyEventHandler getKeyEventHandlerN();


    /*!
        Sets the key event handler.
        @param[in] handler A key event handler.
    */
    static void setKeyEventHandler(KeyEventHandler handler);


    /*!
        默认按键事件类型.
        @param[in] key 键值.
        @param[in] key_state 按键状态.
    */
    static void defaultKeyEventHandler(KeyType key, KeyState key_state);


    /*!
        返回按键事件类型,如果不存在则返回NULL.
        @return 按键事件类型.
    */
    static MouseEventHandler getMouseEventHandlerN();

    /*!
        Sets the mouse event handler.
        @param[in] handler A mouse event handler.
    */
    static void setMouseEventHandler(MouseEventHandler handler);

    /*!
        默认鼠标事件类型.
        @param[in] mouse_x 鼠标x轴坐标.
        @param[in] mouse_y 鼠标y轴坐标.
    */
    static void defaultMouseEventHandler(s16 mouse_x, s16 mouse_y);


    /*!
        返回鼠标事件类型,如果不存在则返回NULL.
        @return 鼠标事件类型.
    */
    static ExtraEventHandler getExtraEventHandlerN();


    /*!
        Sets the extra event handler.
        @param[in] handler An extra event handler.
    */
    static void setExtraEventHandler(ExtraEventHandler handler);


    /*!
        默认额外的事件类型.
        @param[in] val_index 值的索引.
        @param[in] value 额外的值.
    */
    static void defaultExtraEventHandler(u8 val_index, r32 value);


    /*!
        Returns whether the specified key is on.
        @param[in] key A key.
        @return Whether the specified key is on.
    */
    static bool isOn(KeyType key);


    /*!
        Returns whether the specified key is off.
        @param[in] key A key.
        @return Whether the specified key is off.
    */
    static bool isOff(KeyType key);


    /*!
        Returns the specified key is pressed.
        @param[in] key A key.
        @return The specified key is pressed.
    */
    static bool isPressed(KeyType key);


    /*!
        Returns the specified key is released.
        @param[in] key A key.
        @return The specified key is released.
    */
    static bool isReleased(KeyType key);


    /*!
        Returns the x-coordinate of the mouse.
        @return The x-coordinate of the mouse.
    */
    static s16 getMouseX();


    /*!
        Returns the y-coordinate of the mouse.
        @return The y-coordinate of the mouse.
    */
    static s16 getMouseY();


    /*!
        Returns the wheel offset of the mouse.
        @return The wheel offset of the mouse.
    */
    static s16 getMouseWheel();


    /*!
        Sets the position of the mouse.
        @param[in] mouse_x The x-coordinate of the mouse.
        @param[in] mouse_y The y-coordinate of the mouse.
    */
    static void setMousePos(s16 mouse_x, s16 mouse_y);


    /*!
        Returns whether the mouse is visible.
        @return Whether the mouse is visible.
    */
    static bool isMouseVisible();


    /*!
        设置鼠标是否可见.
        @param[in] is_visible 鼠标是否可见.
    */
    static void setMouseVisible(bool is_visible);

    /*!
        Returns the extra value as 32-bit integer number.
        @param[in] val_index The index of the extra value.
        @return The extra value.
    */
    static s32 getExtraValue_s32(u8 val_index);
	
	
    /*!
        Returns the extra value as 32-bit floating point number.
        @param[in] val_index The index of the extra value.
        @return The extra value.
    */
    static r32 getExtraValue_r32(u8 val_index);
	
	
    /*!
        更新输入状态,该方法只被引擎自己调用.
    */
    static void updateKeyStateForEngine();

    /*!
        Resets the states of the kyes. This method is only for system.
    */
    static void resetKeyStateForEngine();

    /*!
        更新额外事件值,该方法只被引擎自己调用.
    */
    static void updateExtraValueForEngine();


private:
    static const u32 KEY_TYPE_NUM = KEY_NONE + 1;

    enum KeyFlag
    {
        FLAG_REAL_ON = 0x01, //
        FLAG_DELAY_ON = 0x02, //
        FLAG_CUR_ON = 0x04, //
        FLAG_LAST_ON = 0x08
    };

    fsInputMgr();
    ~fsInputMgr();
    void operator=(const fsInputMgr&);

    static fsInputMgr* instance();

    KeyEventHandler m_key_event_handler;
    MouseEventHandler m_mouse_event_handler;
    ExtraEventHandler m_extra_event_handler;
    u8 m_key_flag[KEY_TYPE_NUM];
    r32 m_cur_ext_val[EXTRA_VALUE_NUM];
    r32 m_real_ext_val[EXTRA_VALUE_NUM];
    s16 m_mouse_x;
    s16 m_mouse_y;
    s16 m_real_mouse_wheel;
    s16 m_cur_mouse_wheel;
    fsInputDaemon* m_key_daemon;

    static fsInputMgr* m_instance;
};


#endif /* FS_INPUT_MGR_H_ */
