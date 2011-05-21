/*
 * fs_low_level_macosx.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/


#ifdef FS_MACOSX

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>
#include <GLUT/glut.h>

#include "base/fs_low_level_api.h"

#include "fs_kernel_all.h" // for fsMgr::SysFlag
#include "fs_input_all.h" // for fsInputMgr::KeyType


static fsLowLevelAPI::KeyEventHandler s_key_event_handler = NULL;
static fsLowLevelAPI::MouseEventHandler s_mouse_event_handler = NULL;
static fsLowLevelAPI::ExtraEventHandler s_extra_event_handler = NULL;

static const char* s_app_name;
static u16 s_framebuffer_width;
static u16 s_framebuffer_height;
static u16 s_sys_flag;
static bool s_is_framebuffer_size_changed;
static bool s_is_fullscreen;
static bool s_is_mouse_visible;

static s32 s_argc;
static char** s_argv;
static bool (*s_update_func)(void);

struct KeyCode
{
    unsigned char key;
    fsInputMgr::KeyType type;
};

struct AlphaKeyCode
{
    unsigned char keyL;
    unsigned char keyC;
    fsInputMgr::KeyType type;
};

struct SpecialKeyCode
{
    int key;
    fsInputMgr::KeyType type;
};

static const KeyCode s_key_code[] =
{
    {(unsigned char)'1', fsInputMgr::KEY_1},
    {(unsigned char)'2', fsInputMgr::KEY_2},
    {(unsigned char)'3', fsInputMgr::KEY_3},
    {(unsigned char)'4', fsInputMgr::KEY_4},
    {(unsigned char)'5', fsInputMgr::KEY_5},
    {(unsigned char)'6', fsInputMgr::KEY_6},
    {(unsigned char)'7', fsInputMgr::KEY_7},
    {(unsigned char)'8', fsInputMgr::KEY_8},
    {(unsigned char)'9', fsInputMgr::KEY_9},
    {(unsigned char)'0', fsInputMgr::KEY_0},
    {(unsigned char)'*', fsInputMgr::KEY_MULTIPLY},
    {(unsigned char)'+', fsInputMgr::KEY_ADD},
    {(unsigned char)'-', fsInputMgr::KEY_SUBTRACT},
    {(unsigned char)'.', fsInputMgr::KEY_DECIMAL},
    {(unsigned char)'/', fsInputMgr::KEY_DIVIDE},
    {(unsigned char)' ', fsInputMgr::KEY_SPACE},
    {(unsigned char)'\n', fsInputMgr::KEY_ENTER},
    {(unsigned char)'\t', fsInputMgr::KEY_TAB},
    {0x1b, fsInputMgr::KEY_ESCAPE},
};

static const size_t s_key_code_count = sizeof(s_key_code) / sizeof(s_key_code[0]);

static const AlphaKeyCode s_alpha_key_code[] =
{
    {'a', 'A', fsInputMgr::KEY_A},
    {'b', 'B', fsInputMgr::KEY_B},
    {'c', 'C', fsInputMgr::KEY_C},
    {'d', 'D', fsInputMgr::KEY_D},
    {'e', 'E', fsInputMgr::KEY_E},
    {'f', 'F', fsInputMgr::KEY_F},
    {'g', 'G', fsInputMgr::KEY_G},
    {'h', 'H', fsInputMgr::KEY_H},
    {'i', 'I', fsInputMgr::KEY_I},
    {'j', 'J', fsInputMgr::KEY_J},
    {'k', 'K', fsInputMgr::KEY_K},
    {'l', 'L', fsInputMgr::KEY_L},
    {'m', 'M', fsInputMgr::KEY_M},
    {'n', 'N', fsInputMgr::KEY_N},
    {'o', 'O', fsInputMgr::KEY_O},
    {'p', 'P', fsInputMgr::KEY_P},
    {'q', 'Q', fsInputMgr::KEY_Q},
    {'r', 'R', fsInputMgr::KEY_R},
    {'s', 'S', fsInputMgr::KEY_S},
    {'t', 'T', fsInputMgr::KEY_T},
    {'u', 'U', fsInputMgr::KEY_U},
    {'v', 'V', fsInputMgr::KEY_V},
    {'w', 'W', fsInputMgr::KEY_W},
    {'x', 'X', fsInputMgr::KEY_X},
    {'y', 'Y', fsInputMgr::KEY_Y},
    {'z', 'Z', fsInputMgr::KEY_Z},
};

static const size_t s_alpha_key_code_count = sizeof(s_alpha_key_code) / sizeof(s_alpha_key_code[0]);

static const SpecialKeyCode s_special_key_code[] =
{
    {GLUT_KEY_F1, fsInputMgr::KEY_F1},
    {GLUT_KEY_F2, fsInputMgr::KEY_F2},
    {GLUT_KEY_F3, fsInputMgr::KEY_F3},
    {GLUT_KEY_F4, fsInputMgr::KEY_F4},
    {GLUT_KEY_F5, fsInputMgr::KEY_F5},
    {GLUT_KEY_F6, fsInputMgr::KEY_F6},
    {GLUT_KEY_F7, fsInputMgr::KEY_F7},
    {GLUT_KEY_F8, fsInputMgr::KEY_F8},
    {GLUT_KEY_F9, fsInputMgr::KEY_F9},
    {GLUT_KEY_F10, fsInputMgr::KEY_F10},
    {GLUT_KEY_F11, fsInputMgr::KEY_F11},
    {GLUT_KEY_F12, fsInputMgr::KEY_F12},
    {GLUT_KEY_LEFT, fsInputMgr::KEY_LEFT},
    {GLUT_KEY_UP, fsInputMgr::KEY_UP},
    {GLUT_KEY_RIGHT, fsInputMgr::KEY_RIGHT},
    {GLUT_KEY_DOWN, fsInputMgr::KEY_DOWN},
    {GLUT_KEY_PAGE_UP, fsInputMgr::KEY_PAGEUP},
    {GLUT_KEY_PAGE_DOWN, fsInputMgr::KEY_PAGEDOWN},
    {GLUT_KEY_HOME, fsInputMgr::KEY_HOME},
    {GLUT_KEY_END, fsInputMgr::KEY_END},
    {GLUT_KEY_INSERT, fsInputMgr::KEY_INSERT},
};

static const size_t s_special_key_code_count = sizeof(s_special_key_code) / sizeof(s_special_key_code[0]);

static fsInputMgr::KeyType s_key = fsInputMgr::KEY_NONE;
static bool s_last_shift_mask = false;
static bool s_last_alt_mask = false;
static bool s_last_ctrl_mask = false;


static fsInputMgr::KeyState s_mouse_button_state = fsInputMgr::STATE_UP;
static fsInputMgr::KeyType s_mouse_button = fsInputMgr::KEY_NONE;
static s16 s_mouse_x = 0;
static s16 s_mouse_y = 0;
static bool s_is_mouse_moved = false;


static void destroyFramebuffer()
{
    // TODO
}


static void onDisplay()
{
    // TODO
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
    glutInit(&s_argc, s_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(s_framebuffer_width, s_framebuffer_height);
    glutCreateWindow(s_app_name);
    glutDisplayFunc(onDisplay);

    return true;
}


bool fsLowLevelAPI::createApplication(const char* title, u16 width, u16 height, u16 sys_flag)
{
    s_app_name = title;
    s_framebuffer_width = width;
    s_framebuffer_height = height;
    s_sys_flag = sys_flag;
    s_is_framebuffer_size_changed = false;
    s_is_fullscreen = (sys_flag & fsMgr::FLAG_FULLSCREEN_START) ? true : false;
    s_is_mouse_visible = true;

    if (!createFramebuffer(width, height))
    {
        return false;
    }

    setupShaderAPI((sys_flag & fsMgr::FLAG_DISABLE_SHADER) ? false : true);

    return true;
}


void fsLowLevelAPI::destroyApplication()
{
    destroyFramebuffer();
}


static void onIdle()
{
    if (s_key_event_handler != NULL)
    {
        if (s_last_shift_mask)
            //(*s_key_event_handler)(fsInputMgr::KEY_SHIFT, fsInputMgr::STATE_DOWN);
            (*s_key_event_handler)(fsInputMgr::KEY_SHIFT, true);
        if (s_last_alt_mask)
            //(*s_key_event_handler)(fsInputMgr::KEY_ALT, fsInputMgr::STATE_DOWN);
            (*s_key_event_handler)(fsInputMgr::KEY_ALT, true);
        if (s_last_ctrl_mask)
            //(*s_key_event_handler)(fsInputMgr::KEY_CTRL, fsInputMgr::STATE_DOWN);
            (*s_key_event_handler)(fsInputMgr::KEY_CTRL, true);
        if (s_key != fsInputMgr::KEY_NONE)
            //(*s_key_event_handler)(s_key, fsInputMgr::STATE_DOWN);
            (*s_key_event_handler)(s_key, true);
        if (s_mouse_button != fsInputMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_mouse_button, s_mouse_button_state);
            bool is_down = s_mouse_button_state == fsInputMgr::STATE_DOWN;
            (*s_key_event_handler)(s_mouse_button, is_down);
            s_mouse_button = fsInputMgr::KEY_NONE;
        }
    }
    if (s_mouse_event_handler != NULL && s_is_mouse_moved)
    {
        (*s_mouse_event_handler)(s_mouse_x, s_mouse_y);
        s_is_mouse_moved = false;
    }

    s_update_func();

    if (s_key_event_handler != NULL)
    {
        if (s_key != fsInputMgr::KEY_NONE)
        {
            //(*s_key_event_handler)(s_key, fsInputMgr::STATE_UP);
            (*s_key_event_handler)(s_key, false);
            s_key = fsInputMgr::KEY_NONE;
        }
        if (s_last_ctrl_mask)
        {
            //(*s_key_event_handler)(fsInputMgr::KEY_CTRL, fsInputMgr::STATE_UP);
            (*s_key_event_handler)(fsInputMgr::KEY_CTRL, false);
            s_last_ctrl_mask = false;
        }
        if (s_last_alt_mask)
        {
            //(*s_key_event_handler)(fsInputMgr::KEY_ALT, fsInputMgr::STATE_UP);
            (*s_key_event_handler)(fsInputMgr::KEY_ALT, false);
            s_last_alt_mask = false;
        }
        if (s_last_shift_mask)
        {
            //(*s_key_event_handler)(fsInputMgr::KEY_SHIFT, fsInputMgr::STATE_UP);
            (*s_key_event_handler)(fsInputMgr::KEY_SHIFT, false);
            s_last_shift_mask = false;
        }
    }
}


static void onKeyboard(unsigned char key, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    for (size_t i = 0; i < s_key_code_count; i++)
        if (s_key_code[i].key == key)
        {
            s_key = s_key_code[i].type;
            break;
        }

    if (s_key == fsInputMgr::KEY_NONE)
    {
        for (size_t i = 0; i < s_alpha_key_code_count; i++)
            if (s_alpha_key_code[i].keyL == key || s_alpha_key_code[i].keyC == key)
            {
                s_key = s_alpha_key_code[i].type;
                break;
            }
    }
}


static void onSpecial(int key, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    for (size_t i = 0; i < s_special_key_code_count; i++)
        if (s_special_key_code[i].key == key)
        {
            s_key = s_special_key_code[i].type;
            break;
        }
}


static void onMouse(int button, int state, int x, int y)
{
    int m = glutGetModifiers();
    s_last_shift_mask = (m & GLUT_ACTIVE_SHIFT) != 0;
    s_last_ctrl_mask = (m & GLUT_ACTIVE_CTRL) != 0;
    s_last_alt_mask = (m & GLUT_ACTIVE_ALT) != 0;

    s_mouse_button_state = (state == GLUT_DOWN) ? fsInputMgr::STATE_DOWN : fsInputMgr::STATE_UP;

    if (button == GLUT_LEFT_BUTTON)
        s_mouse_button = fsInputMgr::KEY_LBUTTON;
    else if (button == GLUT_MIDDLE_BUTTON)
        s_mouse_button = fsInputMgr::KEY_MBUTTON;
    else if (button == GLUT_RIGHT_BUTTON)
        s_mouse_button = fsInputMgr::KEY_RBUTTON;
    else if (button == 3)
        s_mouse_button = fsInputMgr::KEY_WHEELUP;
    else if (button == 4)
        s_mouse_button = fsInputMgr::KEY_WHEELDOWN;
}


static void onMotion(int x, int y)
{
    s_is_mouse_moved = true;
    s_mouse_x = (s16)x;
    s_mouse_y = (s16)y;
}


static void onPassiveMotion(int x, int y)
{
    s_is_mouse_moved = true;
    s_mouse_x = (s16)x;
    s_mouse_y = (s16)y;
}


void fsLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;

    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc(onSpecial);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMotion);
    glutPassiveMotionFunc(onPassiveMotion);
    glutIdleFunc(onIdle);
    glutMainLoop();
}


u16 fsLowLevelAPI::getFramebufferWidth()
{
    return s_framebuffer_width;
}


u16 fsLowLevelAPI::getFramebufferHeight()
{
    return s_framebuffer_height;
}


void fsLowLevelAPI::updateFramebufferSize()
{
    // TODO
}


bool fsLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void fsLowLevelAPI::swapFramebuffer()
{
    glutSwapBuffers();
}


bool fsLowLevelAPI::isFullScreen()
{
    return s_is_fullscreen;
}


bool fsLowLevelAPI::toggleFullScreen(u16 width, u16 height)
{
    destroyFramebuffer();

    s_is_fullscreen = !s_is_fullscreen;

    return createFramebuffer(width, height);
}


void fsLowLevelAPI::setKeyEventHandler(KeyEventHandler handler)
{
    s_key_event_handler = handler;
}


void fsLowLevelAPI::setMouseEventHandler(MouseEventHandler handler)
{
    s_mouse_event_handler = handler;
}


void fsLowLevelAPI::setExtraEventHandler(ExtraEventHandler handler)
{
    s_extra_event_handler = handler;
}


void fsLowLevelAPI::setMousePos(s16 mouse_x, s16 mouse_y)
{
    // TODO
}


bool fsLowLevelAPI::isMouseVisible()
{
    return s_is_mouse_visible;
}


void fsLowLevelAPI::setMouseVisible(bool is_visible)
{
    if (is_visible != s_is_mouse_visible)
    {
        s_is_mouse_visible = is_visible;

        // TODO
    }
}


u64 fsLowLevelAPI::getUsecTime()
{
    static u64 s_start_time;
    static bool s_is_first = true;

    struct timeval time;
    struct timezone tz;

    if (s_is_first)
    {
        s_is_first = false;

        gettimeofday(&time, &tz);
        s_start_time = time.tv_sec * 1000000 + time.tv_usec;
    }

    gettimeofday(&time, &tz);
    u64 cur_time = time.tv_sec * 1000000 + time.tv_usec;

    return cur_time - s_start_time;
}


void fsLowLevelAPI::sleepUsec(u64 usec)
{
    usleep(usec);
}


void fsLowLevelAPI::exit(s32 status)
{
    ::exit(status);
}


void fsLowLevelAPI::error(const char* msg)
{
    printf(msg);
    printf("\n");

    exit(1);
}


void fsLowLevelAPI::readLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


void fsLowLevelAPI::writeLittleEndian(void* dest, const void* src, u32 size)
{
    memcpy(dest, src, size);
}


struct ThreadStartFuncAndUserParam
{
    void (*start_func)(void*);
    void* user_param;
};


static void* threadStartFunc(void* user_param)
{
    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(user_param);

    func_and_param->start_func(func_and_param->user_param);

    fsLowLevelAPI::free(user_param);

    return NULL;
}


void* fsLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
    static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&s_mutex);

    void* thread_handler = malloc(sizeof(pthread_t));

    ThreadStartFuncAndUserParam* func_and_param = static_cast<ThreadStartFuncAndUserParam*>(malloc(sizeof(ThreadStartFuncAndUserParam)));
    func_and_param->start_func = start_func;
    func_and_param->user_param = user_param;

    if (pthread_create(static_cast<pthread_t*>(thread_handler), NULL, threadStartFunc, func_and_param))
    {
        free(thread_handler);
        free(func_and_param);

        thread_handler = NULL;
    }

    pthread_mutex_unlock(&s_mutex);

    return thread_handler;
}


void fsLowLevelAPI::deleteThread(void* thread_handler)
{
    free(thread_handler);
}


void fsLowLevelAPI::joinThread(void* thread_handler)
{
    pthread_join(*static_cast<pthread_t*>(thread_handler), NULL);
}


void* fsLowLevelAPI::newMutex()
{
    void* mutex_handler = malloc(sizeof(pthread_mutex_t));

    if (pthread_mutex_init(static_cast<pthread_mutex_t*>(mutex_handler), NULL))
    {
        free(mutex_handler);

        return NULL;
    }
    else
    {
        return mutex_handler;
    }
}


void fsLowLevelAPI::deleteMutex(void* mutex_handler)
{
    pthread_mutex_destroy(static_cast<pthread_mutex_t*>(mutex_handler));

    free(mutex_handler);
}


void fsLowLevelAPI::lockMutex(void* mutex_handler)
{
    pthread_mutex_lock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void fsLowLevelAPI::unlockMutex(void* mutex_handler)
{
    pthread_mutex_unlock(static_cast<pthread_mutex_t*>(mutex_handler));
}


void fsLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
    s_argc = argc;
    s_argv = argv;

    if (argc > 0)
    {
        chdir(dirname(argv[0]));
    }
}


void fsLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size) {}


#endif // FS_MACOSX
