#pragma once

#include "../common.hpp"

#include <Windows.h>

struct Win_Desc {
	RECT rect = { 0, 0, 1280, 720 };
	HINSTANCE instance = nullptr;
	std::string name = "Nothing World";
	bool window_mode = false;
	bool is_child = false;
	HWND parent_handle = nullptr;
};

class Win_App 
{
public:
	Win_App(const Win_Desc& desc);

	static LRESULT WINAPI WndProc(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam);

	void init();
	void destroy();

	HWND get_hwnd() const;

private:
	RECT rect;
	vec2 size;
	WNDCLASSEX window_info;
	HWND parent_hwnd;
	HWND hwnd;
	u32 options;
	bool window_mode;
};