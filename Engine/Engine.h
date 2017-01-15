#pragma once

#include "DebugSystem.h"
#include "Window.h"

#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>

#include "snowblind_shared.h"
#include <string>

#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class InputHandle;
class EntityManager;
class PhysicsManager;

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

class CAssetsContainer;
class TerrainManager;

typedef struct ID3D10Blob IBlob;

namespace Hex
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

	class Camera;
	class CConsole;
	class CFont;
	class CFontManager;
	class CModel;
	class Renderer;
	class Synchronizer;
	class Texture;
	class Effect;
	class CTerrain;
	class IGraphicsAPI;

	class Engine
	{
	public:
		static void Create();
		static void Destroy();
		static Engine* GetInstance();

		bool Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc);
		bool CleanUp();

		void Update();
		void Render();
		void UpdateInput();

		/*
			Both of the APIs should be available at all time.
			and you should not get the api but you should get 
			whatever is selected
		*/
#ifdef SNOWBLIND_DX11
		static DirectX11* GetAPI(); //Should not be a define.
#else
		static Vulkan* GetAPI(); //Should not be a define.
#endif
		//_________________________________________
		// Settings
		void ToggleVsync();
		
		Camera* GetCamera();
		Camera* Get2DCamera();


		static void Present();
		static void Clear();
		static void EnableZ();
		static void DisableZ();

		static void ToggleWireframe();
		
		SWindowSize GetWindowSize() const;
		CFont* LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth);
		float GetDeltaTime();
		float GetFPS();
		float GetFrameTime();
		std::string GetAPIName();

		//_________________________________________
		// Get Resources
		Texture* GetTexture(const std::string& aFilePath);
		Effect* GetEffect(const std::string& aFilePath);
		CModel* GetModel(const std::string& aFilePath);

		const std::string& LoadModel(const std::string& aFilePath, const std::string& effect);

		void ResetRenderTargetAndDepth();

		//_________________________________________
		// Windows Message Handling
		void OnAltEnter();
		void OnPause();
		void OnResume();
		void OnExit();
		void OnInactive();
		void OnActive();

		bool IsWindowActive() { return m_Window.IsWindowActive(); }
		
		//_________________________________________
		// Get Systems
		Synchronizer* GetSynchronizer() { return mySynchronizer; }
		EntityManager* GetEntityManager() { return m_EntityManager; }
		PhysicsManager* GetPhysicsManager() { return m_PhysicsManager; }
		Threadpool& GetThreadpool();
		

		//_________________________________________
		// Gets
		const SLocalTime& GetLocalTime();
		Window& GetWindow() { return m_Window; }
		std::string GetLocalTimeAsString();
		InputHandle* GetInputHandle() { return m_InputHandle; }

		//_________________________________________
		// Level Creation, Loading, Saving
		CTerrain* CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize);
		void LoadLevel(const std::string& level_filepath);
		void SaveLevel();



		//_________________________________________
		// Debug Handling
		bool InitiateDebugSystem(InputHandle* input_handle);
		void ToggleDebugMenu();
		bool IsDebugMenuActive() { return m_DebugSystem.GetDebugMenuIsActive(); }
		void AddError(const std::string& error_message);
		void AddDebugText(const std::string& debug_text);


		//_________________________________________
		// Shader Creation
		HRESULT CompileShaderFromFile(const std::string& file_path, const std::string& shader_type, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message);
		void* CreateShader(IBlob* compiled_shader_blob, const std::string& shader_type, const std::string& debug_name);


		const HWND& GetHWND() const { return myHWND; }

	private:
		Engine() = default;
		//void CreateAppWindow(HINSTANCE anInstance, WNDPROC aWndProc);
		
		void AddEntitySystems();

		bool HasInitiated();
		bool m_IsInitiated = false;
		bool m_IsLoadingLevel = false;
		static Engine* myInstance;
		static IGraphicsAPI* myAPI;
		Threadpool m_Threadpool;

		SWindowSize myWindowSize;
		SLocalTime myLocalTime;
		HWND myHWND;
		Window m_Window;
		DebugSystem m_DebugSystem;

		InputHandle* m_InputHandle					= nullptr;

		EntityManager* m_EntityManager				= nullptr;
		PhysicsManager* m_PhysicsManager			= nullptr;

		CFontManager* myFontManager					= nullptr;
		CU::TimeManager* myTimeManager				= nullptr;
		Synchronizer* mySynchronizer				= nullptr;
		Renderer* myRenderer						= nullptr;
		Camera*  myCamera							= nullptr;
		Camera*  my2DCamera							= nullptr;
		CConsole* myConsole							= nullptr;
		CAssetsContainer* myAssetsContainer	= nullptr;
		TerrainManager* m_TerrainManager		= nullptr;
		bool myUsingVSync = false;
		float m_DeltaTime = 0.f;

	};
};

