#pragma once

#include <Windows.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif


namespace std
{
	class thread;
}

namespace CommonUtilities
{
	class TimeManager;
}

namespace Snowblind
{

	struct SWindowSize
	{
		SWindowSize() {};
		SWindowSize(float aWidth, float aHeight) : myWidth(aWidth), myHeight(aHeight) {};
		float myWidth;
		float myHeight;
	};

#ifndef DX12
	class CDirectX11;
#endif
	class CFontManager;
	class CSynchronizer;
	class CFont;
	class CCamera;
	class CRenderer;
	struct SFontData;
	class CEngine
	{
	public:
		static void Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		static void Destroy();
		static CEngine* GetInstance();
#ifndef DX12
		static CDirectX11* GetDirectX();
#endif
		void Initiate();
		CCamera* GetCamera();
		CCamera* Get2DCamera();
		static void Update();
		static void Present();
		static void Clear();
		
		const SWindowSize& GetWindowSize() const;
		CFont* LoadFont(const char* aFilepath, short aFontWidth);
		const float GetDeltaTime();
		const float GetFPS();
		const char* GetAPIName();

		void OnPause();
		void OnResume();
		void OnExit();
		CSynchronizer* GetSynchronizer();



	private:
		CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		~CEngine();

		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;
#ifndef DX12
		static CDirectX11* myAPI;
#endif
		SWindowSize myWindowSize;

		HWND myHWND;
		CFontManager* myFontManager;
		CU::TimeManager* myTimeManager;
		CSynchronizer* mySynchronizer;

		CRenderer* myRenderer;
		CCamera*  myCamera;
		CCamera*  my2DCamera;

	};
};