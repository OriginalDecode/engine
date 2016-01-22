#pragma once

#include "EngineEnums.h"
#include <Windows.h>

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

namespace Snowblind
{
	class CAPI;
	class CEngine
	{
	public:
		static void Create(eAPIFlag anAPIFlag, float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		static void Destroy();
		static CEngine* GetInstance();

		void Present();
		void Clear();
		CAPI* GetAPI();
	private:
		CEngine(eAPIFlag anAPIFlag, float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		~CEngine();

		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;
		CAPI* myAPI;

		float myWindowWidth;
		float myWindowHeight;

		HWND myHWND;

	};

	__forceinline CAPI* CEngine::GetAPI()
	{
		return myAPI;
	}

};