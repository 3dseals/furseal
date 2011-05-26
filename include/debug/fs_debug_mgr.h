/*
 * fs_debug_mgr.h
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/

#ifndef FS_DEBUG_MGR_H_
#define FS_DEBUG_MGR_H_


class fsDebugDaemon;


/*!
    @ingroup fsDebug
    The debug manager singleton.
*/
class FS_API fsDebugMgr
{

	friend class fsDebugDaemon;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    static const fsID DEBUG_MODE_SCREEN_ID; //!< The debug console screen ID.
    static const fsID DEBUG_FONT_TEXTURE_ID; //!< The debug font texture ID.

    /*!
        The debug modes.
    */
    enum DebugMode
    {
        MODE_OFF, //!< Debug output is invisible.
        MODE_MONITOR, //!< Monitor mode.
        MODE_CONSOLE //!< Console mode.
    };

    /*!
        Returns whether the debugging manager singleton is created.
        @return Whether the debugging manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the debugging manager singleton.
    */
    static void createLast();

    /*!
        Destroys the debugging manager singleton.
    */
    static void destroySecond();

    /*!
        Returns the debugging mode.
        @return The debugging mode.
    */
    static DebugMode getDebugMode();

    /*!
        Sets the debugging mode.
        @param[in] dbg_mode A debugging mode.
    */
    static void setDebugMode(DebugMode dbg_mode);

    /*!
        Scrolls up the viewing area of the debugging console.
    */
    static void pageUpConsole();

    /*!
        Scrolls down the viewing area of the debugging console.
    */
    static void pageDownConsole();

    /*!
        Draws a line.
        @param[in] pos1 A vertex of a line.
        @param[in] pos2 A vertex of a line.
        @param[in] col The color of a line.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawLine(const fsVec& pos1, const fsVec& pos2, fsCol col, fsID scr_id);

    /*!
        Draws a polygon.
        @param[in] pos1 A vertex of a polygon.
        @param[in] pos2 A vertex of a polygon.
        @param[in] pos3 A vertex of a polygon.
        @param[in] col The color of a polygon.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawPolygon(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3, fsCol col, fsID scr_id);

    /*!
        Draws a polygon.
        @param[in] pos1 A vertex of a polygon.
        @param[in] pos2 A vertex of a polygon.
        @param[in] pos3 A vertex of a polygon.
        @param[in] pos4 A vertex of a polygon.
        @param[in] col The color of a polygon.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawPolygon(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3, const fsVec& pos4, fsCol col, fsID scr_id);

    /*!
        Draws a box.
        @param[in] center The center of a box.
        @param[in] size The size of a box.
        @param[in] poly_col The color of the faces.
        @param[in] line_col The color of the sides.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawBox(const fsMat& center, const fsVec& size, fsCol poly_col, fsCol line_col, fsID scr_id);

    /*!
        Draws a sphere.
        @param[in] center The center of a sphere.
        @param[in] radius The radius of a sphere.
        @param[in] poly_col The color of the face.
        @param[in] line_col The color of the side.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawSphere(const fsVec& center, r32 radius, fsCol poly_col, fsCol line_col, fsID scr_id);

    /*!
        Draws an axis.
        @param[in] world The world of an axis.
        @param[in] size The size of an axis.
        @param[in] scr_id The ID of a target screen.
    */
    static void drawAxis(const fsMat& world, r32 size, fsID scr_id);

    /*!
        Draws a string on the debugging console.
        @param[in] left The left position of a string.
        @param[in] top The top position of a string.
        @param[in] col The color of a string.
        @param[in] scale The scale factor of a string.
        @param[in] str A string.
        @param[in] ... An argument.
    */
    static void drawString(r32 left, r32 top, fsCol col, u8 scale, const char* str, ...);

    /*!
        Prints a trace log on the debugging console.
        @param[in] str A string.
        @param[in] ... An argument.
    */
    static void trace(const char* str, ...);

    /*!
        Prints the specified vector.
        @param[in] vec A vector.
        @param[in] name The name of vector.
    */
    static void dumpVector(const fsVec& vec, const char* name = NULL);

    /*!
        Prints the specified matrix.
        @param[in] mat A matrix.
        @param[in] name The name of matrix.
    */
    static void dumpMatrix(const fsMat& mat, const char* name = NULL);

    /*!
        Prints all the memory blocks.
    */
    static void dumpMemory();

    /*!
        Prints all the tasks.
    */
    static void dumpTask();

    /*!
        Prints all the resources.
    */
    static void dumpResource();

    /*!
        Prints all the configuration scripts.
    */
    static void dumpConfig();

    /*!
        Prints all the screens.
    */
    static void dumpScreen();

    /*!
        Prints all the textures.
    */
    static void dumpTexture();

    /*!
        Prints all the shaders.
    */
    static void dumpShader();

private:
    static const u32 INITIAL_DRAW_PRIM_NUM = 64;
    static const u32 INITIAL_FONT_SPRT_NUM = 256;
    static const u32 DEBUG_FONT_WIDTH = 6;
    static const u32 DEBUG_FONT_HEIGHT = 10;
    static const u32 CONSOLE_COLUMN_NUM = 80;
    static const u32 CONSOLE_BUFFER_SIZE = 1000;
    static const u32 CONSOLE_TAB_WIDTH = 4;

    fsDebugMgr();
    ~fsDebugMgr();
    void operator=(const fsDebugMgr&);

    void newDebugFontTexture();
    void reallocDrawPrim();
    void chefsSpecialCommand();

    void drawConsole();
    static void drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, fsCol col);
    static void drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, fsCol col);
    s32 calcConsoleRowNum();

    static fsDebugMgr* instance();

    fsType<u8, DebugMode> m_dbg_mode;
    fsScr* m_dbg_mode_scr;
    fsPrim* m_draw_prim;
    u32 m_cur_draw_prim_num;
    u32 m_max_draw_prim_num;
    fsSprt m_font_sprt;
    fsStr<char, CONSOLE_COLUMN_NUM> m_console_buf[CONSOLE_BUFFER_SIZE];
    s32 m_end_row;
    s32 m_aim_row;
    s32 m_cur_row;
    u8 m_dbg_mode_tap_cntr;
    u8 m_dbg_dump_tap_cntr;
    s16 m_scroll_hold_cntr;
    fsDebugDaemon* m_dbg_daemon;

    static fsDebugMgr* m_instance;
};

#endif // !FS_DEBUG_MGR_H_
