#pragma once
#include <string>
#include <Windows.h>
#include <standard_datatype.hpp>
#ifndef _WINDEF_

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class CEntityManager;

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
	enum eDeferredType;
	struct SWindowSize
	{
		SWindowSize() {};
		SWindowSize(float aWidth, float aHeight) : myWidth(aWidth), myHeight(aHeight) {};
		float myWidth;
		float myHeight;
	};

	class CAssetsContainer;
	class CCamera;
	class CConsole;
	class CDirectX11;
	class CFont;
	class CFontManager;
	class CModel;
	class CRenderer;
	class CSynchronizer;
	class CTexture;
	class CEffect;

	struct SFontData;

	class CEngine
	{
	public:
		static void Create(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		static void Destroy();
		static CEngine* GetInstance();

		static CDirectX11* GetDirectX();

		void Initiate();

		CCamera* GetCamera();
		CCamera* Get2DCamera();
		static void Update();
		static void Present();
		static void Clear();
		
		const SWindowSize& GetWindowSize() const;
		CFont* LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth);
		const float GetDeltaTime();
		const float GetFPS();
		const std::string& GetAPIName();

		CTexture* GetTexture(const std::string& aFilePath);
		CEffect* GetEffect(const std::string& aFilePath);
		CModel* GetModel(const std::string& aFilePath);
		const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);

		void ResetRenderTargetAndDepth();
		void ToggleVsync();
		void OnAltEnter();
		void OnPause();
		void OnResume();
		void OnExit();
		CSynchronizer* GetSynchronizer();
	
	private:
		CEngine(float aWindowWidth, float aWindowHeight, HINSTANCE anInstance, WNDPROC aWndProc);
		~CEngine();

		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;

		static CDirectX11* myAPI;
		SWindowSize myWindowSize;

		HWND myHWND;
		CFontManager* myFontManager;
		CU::TimeManager* myTimeManager;
		CSynchronizer* mySynchronizer;
		CModel* model;
		CRenderer* myRenderer;
		CCamera*  myCamera;
		CCamera*  my2DCamera;
		CConsole* myConsole;
		CAssetsContainer* myAssetsContainer;
		bool myUsingVSync = false;
	};
};