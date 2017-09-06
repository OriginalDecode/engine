#include "stdafx.h"
#include "Window.h"



bool Window::Initiate(WindowCreateInfo window_create_info)
{
	m_CreateInfo = window_create_info;
	m_WindowSize.m_Height = window_create_info.window_height;
	m_WindowSize.m_Width = window_create_info.window_width;

#ifdef _WINDEF_

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = m_CreateInfo.window_process;
	wc.hInstance = m_CreateInfo.instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowsClass";
	RegisterClassEx(&wc);
	float width = (GetSystemMetrics(SM_CXSCREEN)* 0.5f) - (m_CreateInfo.window_width * 0.5f);
	float height = (GetSystemMetrics(SM_CYSCREEN)* 0.5f) - (m_CreateInfo.window_height * 0.5f);
	m_HWND = CreateWindow(
		"WindowsClass"
		, NULL
		, WS_OVERLAPPEDWINDOW   //WS_POPUP, Borderless windowed
		, s32(width) // start x
		, s32(height) // start y
		, s32(m_CreateInfo.window_width)
		, s32(m_CreateInfo.window_height)
		, NULL
		, NULL
		, GetModuleHandle(NULL), NULL);

	RECT inner_size;
	GetClientRect(m_HWND, &inner_size);
	m_InnerSize.m_Height = (float)inner_size.bottom;
	m_InnerSize.m_Width = ( float ) inner_size.right;


#endif

	return true;
}

void Window::ShowWindow()
{
	::ShowWindow(m_HWND, true);
}


void Window::SetWindowText(const char* text)
{
	::SetWindowText(m_HWND, text);
}

const WindowSize& Window::GetWindowSize() const
{
	return m_WindowSize;
}

const WindowSize& Window::GetInnerSize() const
{
	return m_InnerSize;
}

void Window::OnInactive()
{
	m_WindowIsActive = false;
}

void Window::OnActive()
{
	m_WindowIsActive = true;
}

