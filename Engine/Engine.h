#pragma once

#include <Windows.h>

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

namespace Snowblind
{
	class CDirectX11;
	class Font;

	class CEngine
	{
	public:
		static void Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		static void Destroy();
		static CEngine* GetInstance();

		static void Present();
		static void Clear();
		CDirectX11* GetAPI();


	private:
		CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		~CEngine();

		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;
		CDirectX11* myAPI;
		Font* myFont;
		float myWindowWidth;
		float myWindowHeight;

		HWND myHWND;

	};

	__forceinline CDirectX11* CEngine::GetAPI()
	{
		return myAPI;
	}

};