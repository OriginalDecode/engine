#pragma once
#include "engine_shared.h"
#include <Windows.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

struct WindowSize
{
	WindowSize() = default;
	WindowSize(int32 width, int32 height) 
		: m_Width(width)
		, m_Height(height) 
	{
	};
	int32 m_Width = 0;
	int32 m_Height = 0;
};

struct WindowCreateInfo
{
	uint16 window_width = 0;
	uint16 window_height = 0;
#ifdef _WINDEF_
	HINSTANCE instance;
	WNDPROC window_process;
#endif
};

class Window
{
public:
	Window() = default;
	bool Initiate(WindowCreateInfo window_create_info, uint64 window_style);
	void ShowWindow();
	HWND GetHWND() const { return m_HWND; }
	HINSTANCE GetWindowInstance() { return m_CreateInfo.instance; }
	bool IsWindowActive() { return m_WindowIsActive; }

	void SetWindowText(const char* text);

	const WindowSize& GetWindowSize() const;

	/*
		returns the size of the window inside the borders
	*/
	const WindowSize& GetInnerSize() const;

	void OnAltEnter();
	void OnPause();
	void OnResume();
	void OnExit();
	void OnInactive();
	void OnActive();


	bool IsFullscreen() const { return m_IsFullscreen; }

private:
	WindowCreateInfo m_CreateInfo;
	HWND m_HWND;
	bool m_WindowIsActive = false;
	bool m_IsFullscreen = false;

	WindowSize m_WindowSize;
	WindowSize m_InnerSize;

};
