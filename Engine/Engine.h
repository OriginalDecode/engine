#pragma once
#include <string>
#include <standard_datatype.hpp>
#include "snowblind_shared.h"
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

struct SLocalTime
{
	u16 hour;
	u16 minute;
	u16 second;
};

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
	
	class DirectX11;
	class Vulkan;

	class CAssetsContainer;
	class CCamera;
	class CConsole;
	class CFont;
	class CFontManager;
	class CModel;
	class CRenderer;
	class CSynchronizer;
	class CTexture;
	class CEffect;
	class CTerrain;
	class IGraphicsAPI;
	struct SFontData;

	class CEngine
	{
	public:
		static void Create();
		static void Destroy();
		static CEngine* GetInstance();

#ifdef SNOWBLIND_DX11
		static DirectX11* GetAPI();
#else
		static Vulkan* GetAPI();
#endif

		bool Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc);
		bool CleanUp();

		CCamera* GetCamera();
		CCamera* Get2DCamera();
		static void Update();
		static void Present();
		static void Clear();
		static void EnableZ();
		static void DisableZ();

		
		const SWindowSize& GetWindowSize() const;
		CFont* LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth);
		void GetDeltaTime(float& delta_time_out);
		const float GetFPS();
		const float GetFrameTime();
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
		const SLocalTime& GetLocalTime();

		CTerrain* CreateTerrain(const std::string& aFile, const CU::Vector3f& position, const CU::Vector2f& aSize);

	private:
		CEngine() = default;
		void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);

		static CEngine* myInstance;
		static IGraphicsAPI* myAPI;

		SWindowSize myWindowSize;
		SLocalTime myLocalTime;
		HWND myHWND;
		
		CFontManager* myFontManager			= nullptr;
		CU::TimeManager* myTimeManager		= nullptr;
		CSynchronizer* mySynchronizer		= nullptr;
		CRenderer* myRenderer				= nullptr;
		CCamera*  myCamera					= nullptr;
		CCamera*  my2DCamera				= nullptr;
		CConsole* myConsole					= nullptr;
		CAssetsContainer* myAssetsContainer	= nullptr;

		bool myUsingVSync = false;
	};
};