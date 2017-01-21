#include "stdafx.h"
#include "Window.h"

namespace Hex
{

	bool Window::Initiate(WindowCreateInfo window_create_info)
	{
		m_CreateInfo = window_create_info;
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
		float sym_x = GetSystemMetrics(SM_CXSCREEN);
		float sym_y = GetSystemMetrics(SM_CYSCREEN);
		RegisterClassEx(&wc);
		float width = (GetSystemMetrics(SM_CXSCREEN)* 0.5f) - (m_CreateInfo.window_width * 0.5f);
		float height = (GetSystemMetrics(SM_CYSCREEN)* 0.5f) - (m_CreateInfo.window_height * 0.5f);
		m_HWND = CreateWindow(
			"WindowsClass"
			, NULL
			, WS_POPUP //Windowed
			, s32(width) // start x
			, s32(height) // start y
			, s32(m_CreateInfo.window_width)
			, s32(m_CreateInfo.window_height)
			, NULL
			, NULL
			, GetModuleHandle(NULL), NULL);
#endif

		return true;
	}

	void Window::ShowWindow()
	{
		::ShowWindow(m_HWND, true);
	} 


	void Window::OnInactive()
	{
		m_WindowIsActive = false;
	}

	void Window::OnActive()
	{
		m_WindowIsActive = true;
	}

};