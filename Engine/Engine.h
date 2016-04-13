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
	struct SWindowSize
	{
		SWindowSize(float aWidth, float aHeight) : myWidth(aWidth), myHeight(aHeight) {};
		float myWidth;
		float myHeight;
	};

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
		const SWindowSize& GetWindowSize() const;

	private:
		CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		~CEngine();

		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;
		CDirectX11* myAPI;
		Font* myFont;
		SWindowSize myWindowSize;

		HWND myHWND;

	};

	__forceinline CDirectX11* CEngine::GetAPI()
	{
		return myAPI;
	}

};