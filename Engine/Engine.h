#pragma once
#include "../Include/hash/EngineEvents.h"

#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>

#include <CommonLib/Timer/TimeManager.h>

#include <EntitySystem/EntityManager.h>

#include <Engine/VirtualFileSystem.h>
#include <Engine/engine_shared.h>
#include <Engine/Window.h>
#include <Engine/ShaderFactory.h>
#include <Engine/SystemMonitor.h>
#include <Engine/MemorySegmentHandle.h>
#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsContext.h>

#include <Engine/DebugHandle.h>

#include <CommonLib/RefPointer.h>
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
	uint16 hour;
	uint16 minute;
	uint16 second;
};

class TerrainManager;
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
class PhysicsManager;
class Material;
struct ModelCommand;

class InputHandle;

namespace Input
{
	class InputManager;
};

namespace network
{
	class NetworkManager;
};

class Engine
{
	friend debug::DebugHandle;
public:
	static void Create();
	static void Destroy();
	static Engine* GetInstance();
	static Engine& GetRef() { return *myInstance; }
	const CU::Vector3f& GetLightDir();
	bool Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc);
	bool CleanUp();

	void Update();
	void UpdateInput();

	int RegisterLight();

	static graphics::IGraphicsAPI* GetAPI() { return m_API; }
	
	Camera* GetCamera();


	const WindowSize& GetWindowSize() const;
	const WindowSize& GetInnerSize() const;

	CFont* LoadFont(const int8* filepath, uint16 aFontWidth, uint16 aBorderWidth);
	VirtualFileSystem& GetVFS();

	//_________________________________________
	// Get Resources
	Texture* GetTexture(uint64 key);
	Effect* GetEffect(uint64 key);

	CompiledShader* GetShader(uint64 key);
	CompiledShader* GetShader(const char* key);

	Sprite* GetSprite(uint64 key);
	Material* GetMaterial(uint64 key);


	Texture* GetTexture(const char* key);
	Effect* GetEffect(const char* key);

	Sprite* GetSprite(const char* key);
	Material* GetMaterial(const char* key);


	uint64 LoadTexture(const std::string& path, bool make_mips = false);
	uint64 LoadEffect(const std::string& path);
	uint64 LoadSprite(const std::string& path);
	uint64 LoadShader(const std::string& path, const std::string& entrypoint);


	void AddTexture(Texture* pTexture, uint64 key);

	// Windows Message Handling
	void OnAltEnter();
	void OnPause();
	void OnResume();
	void OnExit();
	void OnInactive();
	void OnActive();
	void OnResize();

	bool IsWindowActive() { return m_Window.IsWindowActive(); }

	Synchronizer* GetSynchronizer();
	EntityManager& GetEntityManager() { return m_EntityManager; }
	PhysicsManager* GetPhysicsManager() { return m_PhysicsManager; }
	Threadpool& GetThreadpool() { return m_Threadpool; }
	network::NetworkManager* GetNetworkManager() { return m_NetManager; }


	//_________________________________________
	// Gets
	const Window& GetWindow() { return m_Window; }
	const Window& GetWindow() const { return m_Window; }

	const SLocalTime& GetLocalTime();
	InputHandle* GetInputHandle() { return m_InputHandle; }


	void* CreateShader(IShaderBlob* pShader, eShaderType type, const std::string& debug_name);


	bool UseMouse() { return m_CameraUseMouse; }
	void ToggleUseMouse() { m_CameraUseMouse = !m_CameraUseMouse; }

	HWND GetHWND() const { return m_Window.GetHWND(); }


	bool VSync() const { return m_VSyncOn; }
	void ToggleVSync() { m_VSyncOn = !m_VSyncOn; }

	Renderer* GetRenderer() { return m_Renderer; }
	TerrainManager* GetTerrainManager() { return m_TerrainManager; }

	LevelFactory* GetLevelFactory() { return m_LevelFactory; }


	Model* GetModelDirect(uint64 key);

	Input::InputManager* GetInputManager() { return m_InputManager; }

private:
	bool HasInitiated();
	
	Engine();
	~Engine();
	static Engine* myInstance;
	static graphics::IGraphicsAPI* m_API;


	CU::TimeManager myTimeManager;

	Window m_Window;



	SLocalTime myLocalTime;

	VirtualFileSystem m_VirtualFileSystem;
	Threadpool m_Threadpool;
	EntityManager m_EntityManager;

	InputHandle* m_InputHandle       = nullptr;
	Input::InputManager* m_InputManager = nullptr;
	
	PhysicsManager* m_PhysicsManager = nullptr;
	CFontManager* myFontManager      = nullptr;

	Synchronizer* m_Synchronizer     = nullptr;
	Renderer* m_Renderer             = nullptr;
	Camera* m_Camera                 = nullptr;
	network::NetworkManager* m_NetManager = nullptr;


	TerrainManager* m_TerrainManager   = nullptr;
	LevelFactory* m_LevelFactory       = nullptr;
	CSystemMonitor m_SystemMonitor;

	float m_DeltaTime = 0.f;

	bool m_PauseInput : 2;
	bool m_CameraUseMouse  : 2;
	bool m_RenderInstanced : 2;
	bool m_VSyncOn : 2;



	void* hinstance = nullptr;
	void* wndproc = nullptr;

};

//template<typename T>
//HashType Engine::LoadModel(const std::string& filepath, std::string effect, bool thread, int option)
//{
//	return myAssetsContainer->LoadModel<T>(filepath, effect, thread, option);
//}
//
//template<typename T>
//RefPointer<T> Engine::GetModel(uint64 key)
//{
//	return myAssetsContainer->GetModel<T>(key);
//}

//template<typename T>
//RefPointer<T> Engine::GetModel(const char* key)
//{
//	return GetModel<T>(cl::Hash(key));
//}