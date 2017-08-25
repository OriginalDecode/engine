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
	WindowSize() {};
	WindowSize(float aWidth, float aHeight) : m_Width(aWidth), m_Height(aHeight) {};
	float m_Width;
	float m_Height;
};

struct WindowCreateInfo
{
	u16 window_width = 0;
	u16 window_height = 0;
	HINSTANCE instance;
	WNDPROC window_process;
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

	const WindowSize& GetWindowSize() const;
	const WindowSize& GetInnerSize() const;
public:
	//________________

	void OnAltEnter();
	void OnPause();
	void OnResume();
	void OnExit();
	void OnInactive();
	void OnActive();

	//________________

private:
	WindowCreateInfo m_CreateInfo;
	HWND m_HWND;
	bool m_WindowIsActive = false;

	WindowSize m_WindowSize;
	WindowSize m_InnerSize;

};
