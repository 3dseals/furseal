/*
 * fs_low_level_windows.cpp
 *
 *  Created on: 2011-3-28
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
*/

#ifdef WINDOWS


#include <windows.h>
#include <zmouse.h>
#include <shlwapi.h>
#include <process.h>

#include "base/fs_low_level_api.h"

#include "fs_kernel_all.h" // for fsMgr::SysFlag
#include "fs_input_all.h" // for fsInputMgr::KeyType


#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "opengl32.lib")


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

static HINSTANCE s_hInstance;
static HWND s_hWnd;
static HDC s_hDC;
static HGLRC s_hRC;


static void callKeyEventHandler(u16 vk, bool is_down)
{
	static const fsInputMgr::KeyType s_key_table[] =
	{
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_LBUTTON, fsInputMgr::KEY_RBUTTON, fsInputMgr::KEY_NONE, fsInputMgr::KEY_MBUTTON, // 0-4
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_BACKSPACE, fsInputMgr::KEY_TAB, // 5-9
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_ENTER, fsInputMgr::KEY_NONE, // 10-14
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_SHIFT, fsInputMgr::KEY_CTRL, fsInputMgr::KEY_ALT, fsInputMgr::KEY_NONE, // 15-19
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 20-24
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_ESCAPE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 25-29
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_SPACE, fsInputMgr::KEY_PAGEUP, fsInputMgr::KEY_PAGEDOWN, // 30-34
		fsInputMgr::KEY_END, fsInputMgr::KEY_HOME, fsInputMgr::KEY_LEFT, fsInputMgr::KEY_UP, fsInputMgr::KEY_RIGHT, // 35-39
		fsInputMgr::KEY_DOWN, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 40-44
		fsInputMgr::KEY_INSERT, fsInputMgr::KEY_DELETE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_0, fsInputMgr::KEY_1, // 45-49
		fsInputMgr::KEY_2, fsInputMgr::KEY_3, fsInputMgr::KEY_4, fsInputMgr::KEY_5, fsInputMgr::KEY_6, // 50-54
		fsInputMgr::KEY_7, fsInputMgr::KEY_8, fsInputMgr::KEY_9, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 55-59
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 60-64
		fsInputMgr::KEY_A, fsInputMgr::KEY_B, fsInputMgr::KEY_C, fsInputMgr::KEY_D, fsInputMgr::KEY_E, // 65-69
		fsInputMgr::KEY_F, fsInputMgr::KEY_G, fsInputMgr::KEY_H, fsInputMgr::KEY_I, fsInputMgr::KEY_J, // 70-74
		fsInputMgr::KEY_K, fsInputMgr::KEY_L, fsInputMgr::KEY_M, fsInputMgr::KEY_N, fsInputMgr::KEY_O, // 75-79
		fsInputMgr::KEY_P, fsInputMgr::KEY_Q, fsInputMgr::KEY_R, fsInputMgr::KEY_S, fsInputMgr::KEY_T, // 80-84
		fsInputMgr::KEY_U, fsInputMgr::KEY_V, fsInputMgr::KEY_W, fsInputMgr::KEY_X, fsInputMgr::KEY_Y, // 85-89
		fsInputMgr::KEY_Z, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, fsInputMgr::KEY_NONE, // 90-94
		fsInputMgr::KEY_NONE, fsInputMgr::KEY_NUMPAD0, fsInputMgr::KEY_NUMPAD1, fsInputMgr::KEY_NUMPAD2, fsInputMgr::KEY_NUMPAD3, // 95-99
		fsInputMgr::KEY_NUMPAD4, fsInputMgr::KEY_NUMPAD5, fsInputMgr::KEY_NUMPAD6, fsInputMgr::KEY_NUMPAD7, fsInputMgr::KEY_NUMPAD8, // 100-104
		fsInputMgr::KEY_NUMPAD9, fsInputMgr::KEY_MULTIPLY, fsInputMgr::KEY_ADD, fsInputMgr::KEY_SEPARATOR, fsInputMgr::KEY_SUBTRACT, // 105-109
		fsInputMgr::KEY_DECIMAL, fsInputMgr::KEY_DIVIDE, fsInputMgr::KEY_F1, fsInputMgr::KEY_F2, fsInputMgr::KEY_F3, // 110-114
		fsInputMgr::KEY_F4, fsInputMgr::KEY_F5, fsInputMgr::KEY_F6, fsInputMgr::KEY_F7, fsInputMgr::KEY_F8, // 115-119
		fsInputMgr::KEY_F9, fsInputMgr::KEY_F10, fsInputMgr::KEY_F11, fsInputMgr::KEY_F12, fsInputMgr::KEY_NONE // 120-124
	};

	static const u32 s_key_table_size = sizeof(s_key_table) / sizeof(fsInputMgr::KeyType);

	(*s_key_event_handler)((vk < s_key_table_size) ? s_key_table[vk] : fsInputMgr::KEY_NONE, is_down);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ENTERSIZEMOVE:
	case WM_EXITSIZEMOVE:
		return 0;

	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
		case SC_KEYMENU:
			return 0;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		callKeyEventHandler(wParam, true);
		break;

	case WM_KEYUP:
		callKeyEventHandler(wParam, false);
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_MENU)
		{
			callKeyEventHandler(VK_MENU, true);
		}
		else if (GetKeyState(VK_F10) < 0)
		{
			callKeyEventHandler(VK_F10, true);
		}
		break;

	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
		{
			callKeyEventHandler(VK_MENU, false);
		}
		else if (GetKeyState(VK_F10) >= 0) // TODO
		{
			callKeyEventHandler(VK_F10, false);
		}
		break;

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		callKeyEventHandler(VK_LBUTTON, true);
		break;

	case WM_LBUTTONUP:
		ReleaseCapture();
		callKeyEventHandler(VK_LBUTTON, false);
		break;

	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		callKeyEventHandler(VK_MBUTTON, true);
		break;

	case WM_MBUTTONUP:
		ReleaseCapture();
		callKeyEventHandler(VK_MBUTTON, false);
		break;

	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		callKeyEventHandler(VK_RBUTTON, true);
		break;

	case WM_RBUTTONUP:
		ReleaseCapture();
		callKeyEventHandler(VK_RBUTTON, false);
		break;

	case WM_MOUSEWHEEL:
		if (static_cast<short>(HIWORD(wParam)) > 0)
		{
			(*s_key_event_handler)(fsInputMgr::KEY_WHEELUP, true);
		}
		else
		{
			(*s_key_event_handler)(fsInputMgr::KEY_WHEELDOWN, true);
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


static void destroyFramebuffer()
{
	if (s_is_fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	if (s_hDC)
	{
		ReleaseDC(s_hWnd, s_hDC);
		s_hDC = NULL;
	}

	if (s_hWnd)
	{
		DestroyWindow(s_hWnd);
		s_hWnd = NULL;
	}

	if (s_hInstance)
	{
		UnregisterClass("Furseal", s_hInstance);
		s_hInstance = NULL;
	}
}


static bool createFramebuffer(u16 new_width, u16 new_height)
{
	/*
	create window
	*/
	s_hInstance = GetModuleHandle(NULL);

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = s_hInstance;
	wc.hIcon = LoadIcon(s_hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Furseal";

	if (!RegisterClass(&wc))
	{
		return false;
	}

	if (s_is_fullscreen)
	{
		DEVMODE dmScreenSettings;
		fsLowLevelAPI::memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = new_width;
		dmScreenSettings.dmPelsHeight = new_height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			s_is_fullscreen = false;

			new_width = s_framebuffer_width;
			new_height = s_framebuffer_height;
		}
	}

	DWORD dwStyle, dwExStyle;

	if (s_is_fullscreen)
	{
		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW;
	}
	else
	{
		if (s_sys_flag & fsMgr::FLAG_VARIABLE_SIZE)
		{
			dwStyle = WS_OVERLAPPEDWINDOW;
		}
		else
		{
			dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
		}

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	RECT rect;

	rect.left = 0;
	rect.right = new_width;
	rect.top = 0;
	rect.bottom = new_height;

	AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);

	s_hWnd = CreateWindowEx( //
		dwExStyle, // dwExStyle
		"Furseal", // lpClassName
		s_app_name, // lpWindowName
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // dwStyle
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		rect.right - rect.left, // nWidth
		rect.bottom - rect.top, // nHeight
		NULL, // hWndParent
		NULL, // hMenu
		s_hInstance, // hInstance
		NULL); // lpParam

	if (!s_hWnd)
	{
		destroyFramebuffer();
		return false;
	}

	s_hDC = GetDC(s_hWnd);

	if (!s_hDC)
	{
		destroyFramebuffer();
		return false;
	}

	/*
	initialize OpenGL
	*/
	static const PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // nSize
		1, // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
		PFD_TYPE_RGBA, // iPixelType
		24, // cColorBits
		0, 0, 0, 0, 0, 0, // Not used
		8, // cAlphaBits
		0, // Not used
		0, // cAccumBits
		0, 0, 0, 0, // Not used
		16, // cDepthBits
		0, // cStencilBits
		0, // cAuxBuffers
		PFD_MAIN_PLANE, // iLayerType
		0, 0, 0, 0, // Not used
	};

	int PixelFormat = ChoosePixelFormat(s_hDC, &pfd);

	if (!PixelFormat)
	{
		destroyFramebuffer();
		return false;
	}

	if (!SetPixelFormat(s_hDC, PixelFormat, &pfd))
	{
		destroyFramebuffer();
		return false;
	}

	s_hRC = wglCreateContext(s_hDC);

	if (!s_hRC)
	{
		destroyFramebuffer();
		return false;
	}

	if (!wglMakeCurrent(s_hDC, s_hRC))
	{
		destroyFramebuffer();
		return false;
	}

	/*
	wait vsync
	*/
	typedef bool (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);

	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));

	if (wglSwapIntervalEXT)
	{
		wglSwapIntervalEXT(1);
	}

	/*
	show window
	*/
	ShowWindow(s_hWnd, SW_SHOW);
	SetForegroundWindow(s_hWnd);
	SetFocus(s_hWnd);

	GetClientRect(s_hWnd, &rect);

	s_framebuffer_width = static_cast<u16>(rect.right);
	s_framebuffer_height = static_cast<u16>(rect.bottom);

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


void fsLowLevelAPI::startApplication(bool (*update_func)(void))
{
	while (true)
	{
		MSG msg;
		msg.wParam = 0;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		POINT pos;

		GetCursorPos(&pos);
		ScreenToClient(s_hWnd, &pos);
		(*s_mouse_event_handler)(static_cast<s16>(pos.x), static_cast<s16>(pos.y));

		(*update_func)();
	}
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
	RECT rect;
	GetClientRect(s_hWnd, &rect);

	if (rect.right != s_framebuffer_width || rect.bottom != s_framebuffer_height)
	{
		s_framebuffer_width = static_cast<u16>(rect.right);
		s_framebuffer_height = static_cast<u16>(rect.bottom);
		s_is_framebuffer_size_changed = true;
	}
	else
	{
		s_is_framebuffer_size_changed = false;
	}
}


bool fsLowLevelAPI::isFramebufferSizeChanged()
{
	return s_is_framebuffer_size_changed;
}


void fsLowLevelAPI::swapFramebuffer()
{
	::SwapBuffers(s_hDC);
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
	POINT pos;

	pos.x = mouse_x;
	pos.y = mouse_y;

	ClientToScreen(s_hWnd, &pos);
	SetCursorPos(pos.x, pos.y);
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

		ShowCursor(s_is_mouse_visible);
	}
}


u64 fsLowLevelAPI::getUsecTime()
{
	static bool is_first = true;
	static LARGE_INTEGER freq;
	static LARGE_INTEGER now;

	if (is_first)
	{
		QueryPerformanceFrequency(&freq);
		is_first = false;
	}

	QueryPerformanceCounter(&now);
	return static_cast<u64>(static_cast<r64>(now.QuadPart) * 1000.0 * 1000.0 / freq.QuadPart);
}


void fsLowLevelAPI::sleepUsec(u64 usec)
{
	Sleep(static_cast<u32>(usec / 1000));
}


void fsLowLevelAPI::exit(s32 status)
{
	::exit(status);
}


void fsLowLevelAPI::error(const char* msg)
{
	printf(msg);
	MessageBoxA(NULL, msg, "Furseal Error", MB_OK | MB_ICONERROR);

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


void* fsLowLevelAPI::newThread(void (*start_func)(void*), void* user_param)
{
	return reinterpret_cast<void*>(_beginthread(start_func, 0, user_param));
}


void fsLowLevelAPI::deleteThread(void* thread_handler) {}


void fsLowLevelAPI::joinThread(void* thread_handler)
{
	WaitForSingleObject(thread_handler, INFINITE);
}


void* fsLowLevelAPI::newMutex()
{
	return CreateMutex(NULL, false, NULL);
}


void fsLowLevelAPI::deleteMutex(void* mutex_handler)
{
	CloseHandle(mutex_handler);
}


void fsLowLevelAPI::lockMutex(void* mutex_handler)
{
	WaitForSingleObject(mutex_handler, INFINITE);
}


void fsLowLevelAPI::unlockMutex(void* mutex_handler)
{
	ReleaseMutex(mutex_handler);
}


void fsLowLevelAPI::setInitialDirectory(s32 argc, char** argv)
{
	if (argc > 0)
	{
		PathRemoveFileSpec(argv[0]);
		SetCurrentDirectory(argv[0]);
	}
}


void fsLowLevelAPI::getWindowsFontDirectory(char* buf, u32 buf_size)
{
	GetWindowsDirectory(buf, buf_size);
	strcat_s(buf, buf_size, "\\Fonts\\");
}


#endif // WINDOWS
