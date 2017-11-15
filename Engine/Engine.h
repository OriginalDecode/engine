#pragma once


#define _WINSOCKAPI_
#include <network/network_api.h>

#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>

#include <Timer/TimeManager/TimeManager.h>

#include <EntitySystem/EntityManager.h>

#include <Engine/VirtualFileSystem.h>
#include <Engine/engine_shared.h>
#include <Engine/Window.h>
#include <Engine/ShaderFactory.h>
#include <Engine/SystemMonitor.h>
#include <Engine/MemorySegmentHandle.h>
#include <Engine/DebugHandle.h>
#include <Engine/IGraphicsAPI.h>
#include <string>
#include <queue>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif


struct SLocalTime
{
	u16 hour;
	u16 minute;
	u16 second;
};

//typedef struct ID3D10Blob IBlob;

class TerrainManager;
class AssetsContainer;
class TreeDweller;
class LevelFactory;
class Camera;
class CFont;
class CFontManager;
class Model;
class Renderer;
class Synchronizer;
class Texture;
class Effect;
class Terrain;
class Sprite;
class InputHandle;
class PhysicsManager;
class Material;

class Engine
{
#if !defined(_PROFILE) && !defined(_FINAL)
	friend debug::DebugHandle;
#endif
public:
	static void Create();
	static void Destroy();
	static Engine* GetInstance();
	static Engine& GetRef() { return *myInstance; }

	bool Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc);
	bool CleanUp();

	void Update();
	void UpdateInput();

	int RegisterLight();

	static graphics::IGraphicsAPI* GetAPI() { return m_API; }
	//_________________________________________
	// Settings
	void ToggleVsync();

	Camera* GetCamera();

	TreeDweller* CreateEntity(const std::string& filepath, CU::Vector3f& position);

	static void Present();
	static void Clear();
	static void EnableZ();
	static void DisableZ();

	const WindowSize& GetWindowSize() const;
	const WindowSize& GetInnerSize() const;

	CFont* LoadFont(const s8* filepath, u16 aFontWidth, u16 aBorderWidth);
	float GetDeltaTime();
	float GetTotalTime();
	float GetFPS();
	float GetFrameTime();

	VirtualFileSystem& GetVFS();

	//_________________________________________
	// Get Resources
	Texture* GetTexture(u64 key);
	Effect* GetEffect(u64 key);
	Model* GetModel(u64 key);
	Sprite* GetSprite(u64 key);
	Material* GetMaterial(u64 key);


	Texture* GetTexture(const char* key);
	Effect* GetEffect(const char* key);
	Model* GetModel(const char* key);
	Sprite* GetSprite(const char* key);
	Material* GetMaterial(const char* key);



	//std::string LoadModel(const std::string& filepath, std::string effect, bool thread);
	template<typename T>
	u64 LoadModel(const std::string& filepath, std::string effect, bool thread);

	u64 LoadTexture(const std::string& path);
	u64 LoadEffect(const std::string& path);
	u64 LoadSprite(const std::string& path);

	void AddTexture(Texture* pTexture, u64 key);

	void ResetRenderTargetAndDepth();

	//_________________________________________
	// Windows Message Handling
	void OnAltEnter();
	void OnPause();
	void OnResume();
	void OnExit();
	void OnInactive();
	void OnActive();
	void OnResize();

	bool IsWindowActive() { return m_Window.IsWindowActive(); }

	//_________________________________________
	// Get Systems
	Synchronizer* GetSynchronizer();
	EntityManager& GetEntityManager() { return m_EntityManager; }
	PhysicsManager* GetPhysicsManager() { return m_PhysicsManager; }
	Threadpool& GetThreadpool() { return m_Threadpool; }

	//_________________________________________
	// Gets
	const Window& GetWindow() { return m_Window; }
	const Window& GetWindow() const { return m_Window; }

	const SLocalTime& GetLocalTime();
	std::string GetLocalTimeAsString(); // should probably return a static buffered string instead that doesn't get newed all the time
	InputHandle* GetInputHandle() { return m_InputHandle; }
	// This can probably stay


	//_________________________________________
	// Level Creation, Loading, Saving
	Terrain* CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize);
	CU::GrowingArray<TreeDweller*> LoadLevel(const std::string& level_filepath);
	// Should be refactored out of the engine stuff.


	void* CreateShader(IShaderBlob* pShader, eShaderType type, const std::string& debug_name);


	bool UseMouse() { return m_CameraUseMouse; }
	void ToggleUseMouse() { m_CameraUseMouse = !m_CameraUseMouse; }

	HWND GetHWND() const { return m_Window.GetHWND(); }

	enum class eEngineStates
	{
		USE_VSYNC,
		INITIATED,
		LOADING,
		_COUNT
	};

	memory::MemorySegmentHandle& GetMemorySegmentHandle() { return m_SegmentHandle; }
	const graphics::eSamplerStates GetCurrentSampler() const { return m_CurrentSampler; }
	ISamplerState* GetActiveSampler() { return m_API->GetSamplerState(m_CurrentSampler); }
	void SetCurrentSampler(const graphics::eSamplerStates& sampler) { m_CurrentSampler = sampler; }

	bool VSync() const { return m_VSyncOn; }
	void ToggleVSync() { m_VSyncOn = !m_VSyncOn; }

	u64 LoadModelA(std::string path, std::string effect, bool threaded);

	void PickEntity();
private:
	s32 PickEntity(Texture* pTexture);
	Engine();
	static Engine* myInstance;
	static graphics::IGraphicsAPI* m_API;
	memory::MemorySegmentHandle m_SegmentHandle;

	graphics::eSamplerStates m_CurrentSampler;
	bool m_VSyncOn = false;


	bool HasInitiated();

	bool m_HasPickedEntity = false;

	SLocalTime myLocalTime;


	VirtualFileSystem m_VirtualFileSystem;
	Threadpool m_Threadpool;
	Window m_Window;
	EntityManager m_EntityManager;

	InputHandle* m_InputHandle       = nullptr;
	PhysicsManager* m_PhysicsManager = nullptr;
	CFontManager* myFontManager      = nullptr;
	CU::TimeManager myTimeManager;
	Synchronizer* m_Synchronizer     = nullptr;
	Renderer* m_Renderer             = nullptr;
	Camera* m_Camera                 = nullptr;



	AssetsContainer* myAssetsContainer = nullptr;
	TerrainManager* m_TerrainManager   = nullptr;
	LevelFactory* m_LevelFactory       = nullptr;
	CSystemMonitor m_SystemMonitor;

	std::bitset<(u16)eEngineStates::_COUNT> m_States;


	float m_DeltaTime = 0.f;

	bool m_PauseInput : 1;
	bool m_CameraUseMouse  : 1;
	bool m_RenderInstanced : 1;
};

template<typename T>
u64 Engine::LoadModel(const std::string& filepath, std::string effect, bool thread)
{
	return myAssetsContainer->LoadModel<T>(filepath, effect, thread);
}

