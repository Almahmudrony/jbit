/*
 * Copyright (C) 2012-2017  Emanuele Fornara
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// io2win32.cc

#include <stdarg.h>
#include <stdio.h>

#define WINVER 0x500
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>

// TODO: better
#ifndef VK_OEM_COMMA
#define VK_OEM_COMMA 0xBC
#endif
#ifndef VK_OEM_PERIOD
#define VK_OEM_PERIOD 0xBE
#endif

#include "core.h"
#include "resource.h"

#include "libretro.h"

extern bool io2_opengl;

static const int width = 128;
static const int height = 128;

static const char *szClassName = "jbit";

#include "io2gl12.h"
typedef GL12Renderer GLRenderer;
static GLRenderer gl;

// TODO: MessageBox
static void error(const char *format, ...) {
	va_list ap;

	va_start(ap, format);
	fprintf(stderr, "jbit: ");
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

static Buffer jb;
static bool valid_program = false;

static void load_program(const char *file_name) {
	FILE *f = 0;

	if (valid_program) {
		retro_unload_game();
		valid_program = false;
	}
	if (!file_name) {
		valid_program = retro_load_game(0);
		return;
	}
	if (!(f = fopen(file_name, "rb"))) {
		error("Failed opening file '%s'.", file_name);
		return;
	}
	jb.reset();
	fseek(f, 0, SEEK_END);
	int n = ftell(f);
	if (n > 1024 * 1024) {
		error("Invalid file '%s' (size >1M).", file_name);
		fclose(f);
		return;
	}
	rewind(f);
	char *buf = jb.append_raw(n);
	int i = 0, j;
	while (i < n) {
		if ((j = fread(&buf[i], 1, n - 1, f)) == 0)
			break;
		i += j;
	}
	fclose(f);
	if (i != n) {
		error("Size check failed for '%s'.", file_name);
		return;
	}
	retro_game_info info;
	info.path = 0;
	info.data = jb.get_data();
	info.size = jb.get_length();
	info.meta = "";
	valid_program = retro_load_game(&info);
}

#define JOYPAD_NULL 1000

static struct {
	int code;
	unsigned keyboard_id;
	unsigned joypad_id;
	bool pressed;
} key_state[] = {
	{ '0', '0', JOYPAD_NULL, false },
	{ '1', '1', JOYPAD_NULL, false },
	{ '2', '2', JOYPAD_NULL, false },
	{ '3', '3', JOYPAD_NULL, false },
	{ '4', '4', JOYPAD_NULL, false },
	{ '5', '5', JOYPAD_NULL, false },
	{ '6', '6', JOYPAD_NULL, false },
	{ '7', '7', JOYPAD_NULL, false },
	{ '8', '8', JOYPAD_NULL, false },
	{ '9', '9', JOYPAD_NULL, false },
	{ VK_OEM_COMMA, ',', JOYPAD_NULL, false },
	{ VK_OEM_PERIOD, '.', JOYPAD_NULL, false },
	{ 'A', 0, RETRO_DEVICE_ID_JOYPAD_Y, false },
	{ VK_UP, 0, RETRO_DEVICE_ID_JOYPAD_UP, false },
	{ 'S', 0, RETRO_DEVICE_ID_JOYPAD_X, false },
	{ VK_LEFT, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, false },
	{ 'X', 0, RETRO_DEVICE_ID_JOYPAD_A, false },
	{ VK_RIGHT, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, false },
	{ 'Q', 0, RETRO_DEVICE_ID_JOYPAD_L, false },
	{ VK_DOWN, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, false },
	{ 'W', 0, RETRO_DEVICE_ID_JOYPAD_R, false },
	{ VK_SHIFT, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, false },
	{ 'Z', 0, RETRO_DEVICE_ID_JOYPAD_B, false },
	{ VK_RETURN, 0, RETRO_DEVICE_ID_JOYPAD_START, false },
	{ 0, false }
};

static void key(int code, bool pressed) {
	for (int i = 0; key_state[i].code; i++)
		if (code == key_state[i].code)
			key_state[i].pressed = pressed;
}

static void video_refresh(const void *data, unsigned width_, unsigned height_,
  size_t pitch) {
	gl.draw((const unsigned char *)data);
}

static bool env(unsigned cmd, void *data) {
	return cmd != RETRO_ENVIRONMENT_GET_LOG_INTERFACE;
}

static void input_poll() {
}

static int16_t input_state(unsigned port, unsigned device, unsigned index,
  unsigned id) {
	for (int i = 0; key_state[i].code; i++) {
		if (device == RETRO_DEVICE_KEYBOARD && id == key_state[i].keyboard_id)
			return key_state[i].pressed;
		if (device == RETRO_DEVICE_JOYPAD && id == key_state[i].joypad_id)
			return key_state[i].pressed;
	}
	return 0;
}

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
static PFNWGLSWAPINTERVALFARPROC wglSwapInterval;

static HDC hDC;

static void load_file_dialog(HWND hWnd) {
	OPENFILENAME ofn;
	char szFileName[1024] = "";
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = "JBit Files (*.jb)\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = "jb";
	if (GetOpenFileName(&ofn))
		load_program(szFileName);
}

static WPARAM main_loop() {
	MSG msg;
	while (1) {
		while (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&msg, 0, 0, 0))
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (valid_program)
			retro_run();
		if (!wglSwapInterval)
			Sleep(10); // TODO: better
		SwapBuffers(hDC);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT: {
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		SwapBuffers(hDC);
		} break;
	case WM_KEYDOWN:
		key(wParam, true);
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_KEYUP:
		key(wParam, false);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_LOAD:
			load_file_dialog(hWnd);
			break;
		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_HELP_ABOUT:
			MessageBox(hWnd, get_jbit_version(), "JBit",
			  MB_OK | MB_ICONINFORMATION);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow) {
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (char *)IDI_APPLICATION);
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_APPMENU);
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (char *)IDI_APPLICATION);
	RegisterClassEx(&wcex);
	const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
	  | WS_MINIMIZEBOX;
	RECT rect = { 0, 0, width * 3, height * 3 };
	AdjustWindowRect(&rect, style, TRUE);
	HWND hWnd = CreateWindow(szClassName, "JBit IO2 Simulator",
	  style, CW_USEDEFAULT, CW_USEDEFAULT,
	  rect.right - rect.left, rect.bottom - rect.top,
	  0, 0, hInstance, 0);
	hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0)
		return 0;
	if (!SetPixelFormat(hDC, pf, &pfd))
		return 0;
	HGLRC hRC;
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	io2_opengl = true;
	retro_set_environment(env);
	retro_init();
	retro_set_video_refresh(video_refresh);
	retro_set_input_poll(input_poll);
	retro_set_input_state(input_state);
	load_program(0);
	gl.init();
	wglSwapInterval = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress(
	  "wglSwapIntervalEXT");
	if (wglSwapInterval)
		wglSwapInterval(1);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	WPARAM ret = main_loop();
	wglMakeCurrent(0, 0);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);
	retro_deinit();
	return ret;
}
