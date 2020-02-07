#pragma once

#include "win_app.hpp"

#include <WinUser.h>

Win_App::Win_App(const Win_Desc & desc)
{
	window_info.cbSize = sizeof(WNDCLASSEX);
	window_info.style = CS_CLASSDC;
	window_info.hInstance = desc.instance;
	window_info.cbClsExtra = 0L;
	window_info.cbWndExtra = 0L;
	window_info.hIcon = NULL;
	window_info.hCursor = NULL;
	window_info.hbrBackground = NULL;
	window_info.hIconSm = NULL;
	window_info.lpszMenuName = NULL;
	window_info.lpszClassName = desc.name.data();

	window_info.lpfnWndProc = WndProc;

	options = desc.is_child ? WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN : WS_OVERLAPPEDWINDOW | WS_SYSMENU;
	parent_hwnd = desc.parent_handle;
	window_mode = desc.window_mode;
	rect = desc.rect;

	if (rect.left == 0 && rect.top == 0.0f)
	{
		RECT rect = { 0, 0, size.x, size.y };
		AdjustWindowRect(&rect, options, FALSE);
		size.x = rect.right - rect.left;
		size.y = rect.bottom - rect.top;
		rect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - size.x / 2;
		rect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - size.y / 2;
	}
}

LRESULT __stdcall Win_App::WndProc(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY :
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Win_App::init()
{
	RegisterClassEx(&window_info);

	hwnd = CreateWindow(window_info.lpszClassName, window_info.lpszClassName
		, options, rect.left, rect.top, size.x, size.y
		, parent_hwnd, NULL, window_info.hInstance, NULL);

	if (options !=(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN))
		ShowWindow(hwnd, SW_SHOWDEFAULT);
}

void Win_App::destroy()
{
	UnregisterClass(window_info.lpszClassName, window_info.hInstance);

	if (options != (WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN))
		DestroyWindow(hwnd);
}

HWND Win_App::get_hwnd() const
{
	return hwnd;
}
