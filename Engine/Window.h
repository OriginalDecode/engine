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
	WindowSize(float aWidth, float aHeight) 
		: m_Width(aWidth)
		, m_Height(aHeight) 
	{
	};
	float m_Width = 0.f;
	float m_Height = 0.f;
};

struct WindowCreateInfo
{
	u16 window_width = 0;
	u16 window_height = 0;
#ifdef _WINDEF_
	HINSTANCE instance;
	WNDPROC window_process;
#endif
};

class Window
{
public:
	Window() = default;
	bool Initiate(WindowCreateInfo window_create_info);
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
