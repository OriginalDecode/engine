#pragma once
#include <string>
#include <standard_datatype.hpp>
#include "snowblind_shared.h"
#include "Window.h"
#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>
#include "DebugSystem.h"
#ifndef _WINDEF_

struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif
class InputHandle;
class CEntityManager;
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

		bool InitiateDebugSystem(CSynchronizer* synchronizer, InputHandle* input_handle);

		bool Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc);
		bool CleanUp();

		CCamera* GetCamera();
		CCamera* Get2DCamera();
		static void Present();
		static void Clear();
		static void EnableZ();
		static void DisableZ();

		void Update();
		
		SWindowSize GetWindowSize() const;
		CFont* LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth);
		float GetDeltaTime();
		float GetFPS();
		float GetFrameTime();
		std::string GetAPIName();

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
		void OnInactive();
		void OnActive();

		bool IsWindowActive() { return m_Window.IsWindowActive(); }
		
		CSynchronizer* GetSynchronizer();

		const SLocalTime& GetLocalTime();
		std::string GetLocalTimeAsString();
		CTerrain* CreateTerrain(const std::string& aFile, const CU::Vector3f& position, const CU::Vector2f& aSize);
		Window& GetWindow() { return m_Window; }
		Threadpool& GetThreadpool();
		void ToggleDebugMenu();
		bool IsDebugMenuActive() { return m_DebugSystem.GetDebugMenuIsActive(); }
	private:
		CEngine() = default;
		//void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);
		bool HasInitiated();
		bool m_IsInitiated = false;

		static CEngine* myInstance;
		static IGraphicsAPI* myAPI;
		Threadpool m_Threadpool;

		SWindowSize myWindowSize;
		SLocalTime myLocalTime;
		HWND myHWND;
		Window m_Window;
		DebugSystem m_DebugSystem;

		CFontManager* myFontManager			= nullptr;
		CU::TimeManager* myTimeManager		= nullptr;
		CSynchronizer* mySynchronizer		= nullptr;
		CRenderer* myRenderer				= nullptr;
		CCamera*  myCamera					= nullptr;
		CCamera*  my2DCamera				= nullptr;
		CConsole* myConsole					= nullptr;
		CAssetsContainer* myAssetsContainer	= nullptr;

		bool myUsingVSync = false;
		float m_DeltaTime = 0.f;
	};
};