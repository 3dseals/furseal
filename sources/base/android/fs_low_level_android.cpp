/*
 * fs_low_level_android.cpp
 *
 *  Created on: 2011-3-17
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
 */


#ifdef FS_ANDROID


#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <pthread.h>
#include <jni.h>

#include "base/fs_low_level_api.h"

#include "fs_kernel_all.h" // for fsMgr::SysFlag
#include "fs_input_all.h" // for fsInputMgr::KeyType
#include "fs_draw_all.h" // for fsDrawMgr::deleteAllVramObjForSystem


static fsLowLevelAPI::KeyEventHandler s_key_event_handler = NULL;
static fsLowLevelAPI::MouseEventHandler s_mouse_event_handler = NULL;
static fsLowLevelAPI::GravitySensorHandler s_gravity_sensor_handler = NULL;
static fsLowLevelAPI::ExtraEventHandler s_extra_event_handler = NULL;

static const char* s_app_name;
static u16 s_framebuffer_width;
static u16 s_framebuffer_height;
static u16 s_sys_flag;
static bool s_is_framebuffer_size_changed;
static bool s_is_fullscreen;
static bool s_is_mouse_visible;
static bool (*s_update_func)();


static void callKeyEventHandler(u16 vk, bool is_down)
{
	static const fsInputMgr::KeyType s_key_table[] =
	{
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_HOME, fsInputMgr::KEY_ESCAPE, // 0-4
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_0, fsInputMgr::KEY_1, fsInputMgr::KEY_2, // 5-9
		fsInputMgr::KEY_3, fsInputMgr::KEY_4, fsInputMgr::KEY_5, fsInputMgr::KEY_6, fsInputMgr::KEY_7, // 10-14
		fsInputMgr::KEY_8, fsInputMgr::KEY_9, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_UP, // 15-19
		fsInputMgr::KEY_DOWN, fsInputMgr::KEY_LEFT, fsInputMgr::KEY_RIGHT, fsInputMgr::KEY_ENTER, fsInputMgr::KEY_NONE, // 20-24
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_A, // 25-29
		fsInputMgr::KEY_B, fsInputMgr::KEY_C, fsInputMgr::KEY_D, fsInputMgr::KEY_E, fsInputMgr::KEY_F, // 30-34
		fsInputMgr::KEY_G, fsInputMgr::KEY_H, fsInputMgr::KEY_I, fsInputMgr::KEY_J, fsInputMgr::KEY_K, // 35-39
		fsInputMgr::KEY_L, fsInputMgr::KEY_M, fsInputMgr::KEY_N, fsInputMgr::KEY_O, fsInputMgr::KEY_P, // 40-44
		fsInputMgr::KEY_Q, fsInputMgr::KEY_R, fsInputMgr::KEY_S, fsInputMgr::KEY_T, fsInputMgr::KEY_U, // 45-49
		fsInputMgr::KEY_V, fsInputMgr::KEY_W, fsInputMgr::KEY_X, fsInputMgr::KEY_Y, fsInputMgr::KEY_Z, // 50-54
		fsInputMgr::KEY_SEPARATOR, fsInputMgr::KEY_DECIMAL, fsInputMgr::KEY_ALT, fsInputMgr::KEY_ALT, fsInputMgr::KEY_SHIFT, // 55-59
		fsInputMgr::KEY_SHIFT, fsInputMgr::KEY_TAB, fsInputMgr::KEY_SPACE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 60-64
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_ENTER, fsInputMgr::KEY_DELETE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_SUBTRACT, // 65-69
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 70-74
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_DIVIDE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 75-79
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_ADD, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 80-84
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 85-89
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_MULTIPLY, fsInputMgr::KEY_PAGEUP, fsInputMgr::KEY_PAGEDOWN, fsInputMgr::KEY_NONE, // 90-94
	};

	static const u32 s_key_table_size = sizeof(s_key_table) / sizeof(fsInputMgr::KeyType);

	(*s_key_event_handler)((vk < s_key_table_size) ? s_key_table[vk] : fsInputMgr::KEY_NONE, is_down);
}


static void destroyFramebuffer() {}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
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

    s_update_func = NULL; // added for Android

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


void fsLowLevelAPI::startApplication(bool (*update_func)(void))
{
    s_update_func = update_func;
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
    /*
    XGetGeometry(s_dpy, s_win, &root, &win_x, &win_y, &win_width, &win_height, &boarder_width, &depth);

    if (win_width != s_framebuffer_width || win_height != s_framebuffer_height)
    {
        s_framebuffer_width = static_cast<u16>(win_width);
        s_framebuffer_height = static_cast<u16>(win_height);
        s_is_framebuffer_size_changed = true;
    }
    else
    {
        s_is_framebuffer_size_changed = false;
    }
    */
}


bool fsLowLevelAPI::isFramebufferSizeChanged()
{
    return s_is_framebuffer_size_changed;
}


void fsLowLevelAPI::swapFramebuffer() {}


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

void fsLowLevelAPI::setGravitySensorHandler(GravitySensorHandler handler)
{
    s_gravity_sensor_handler = handler;
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

        if (s_is_mouse_visible)
        {
            // TODO
        }
        else
        {
            // TODO
        }
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
    if (argc > 0)
    {
        if (chdir(dirname(argv[0])) != 0)
        {
            return; // to avoid gcc warning
        }
    }
}


void fsLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size) {}


extern void fsMain_();


extern "C"
{
    static bool s_is_resumed = false;

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeInitialize(JNIEnv*, jobject)
    {
        fsMain_();
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeUpdate(JNIEnv*, jobject)
    {
        if (s_update_func)
        {
            if (s_is_resumed)
            {
                s_is_resumed = false;

            }

            while (!(*s_update_func)());
        }
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeFinalize(JNIEnv*, jobject)
    {
        fsLowLevelAPI::exit(0);
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativePause(JNIEnv*, jobject)
    {
        // do nothing for now
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeResume(JNIEnv*, jobject)
    {
        s_is_resumed = true;
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeTouch(JNIEnv*, jobject, jint action, jint x, jint y)
    {
        switch (action)
        {
        case 0: // ACTION_DOWN
            (*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
            (*s_key_event_handler)(fsInputMgr::KEY_LBUTTON, true);
            break;

        case 1: // ACTION_UP:
            (*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
            (*s_key_event_handler)(fsInputMgr::KEY_LBUTTON, false);
            break;

        case 2: // ACTION_MOVE:
            (*s_mouse_event_handler)(static_cast<s16>(x), static_cast<s16>(y));
            break;

        default:
            break;
        }
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeOnKeyDown(JNIEnv*, jobject, jint key_code)
    {
        callKeyEventHandler(static_cast<s16>(key_code), true);
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeOnKeyUp(JNIEnv*, jobject, jint key_code)
    {
        callKeyEventHandler(static_cast<s16>(key_code), false);
    }

    JNIEXPORT void JNICALL Java_com_furseal_FursealView_nativeOnSensorChanged(JNIEnv*, jobject, jfloat x, jfloat y, jfloat z)
    {
	   (*s_gravity_sensor_handler)(static_cast<r32>(x), static_cast<r32>(y), static_cast<r32>(z));
    }
}


#endif // FS_ANDROID
