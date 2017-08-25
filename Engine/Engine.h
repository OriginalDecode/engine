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

#include <string>

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
class IGraphicsInterface;

class Engine
{
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

	static IGraphicsInterface* GetGraphicsInterface() { return m_GraphicsInterface; }
	//_________________________________________
	// Settings
	void ToggleVsync();

	//Camera* GetCamera();

	//TreeDweller* CreateEntity(const std::string& filepath, CU::Vector3f& position);

	static void Present();
	static void Clear();
	static void EnableZ();
	static void DisableZ();

	const WindowSize& GetWindowSize() const;
	const WindowSize& GetInnerSize() const;

	CFont* LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth);
	float GetDeltaTime();
	float GetTotalTime();
	float GetFPS();
	float GetFrameTime();
	std::string GetAPIName();

	VirtualFileSystem& GetVFS();

	//_________________________________________
	// Get Resources
	Texture* GetTexture(const std::string& aFilePath);
	Effect* GetEffect(const std::string& aFilePath);
	Model* GetModel(const std::string& aFilePath);
	Sprite* GetSprite(const cl::CHashString<128>& path);

	std::string LoadModel(std::string aFilePath, std::string effect, bool thread);
	template<typename T>
	std::string LoadModel(std::string filepath, std::string effect, T* pModel, bool thread);

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


	//_________________________________________
	// Shader Creation
	//This should probably be moved to the graphics API instead.
	/* These 3 functions are very API specific */
	HRESULT CompileShaderFromFile(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message);
	HRESULT CompileShaderFromMemory(const s8* pData, s32 size, const std::string& source_name, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_shader, IBlob* out_message);
	void* CreateShader(void* compiled_shader_blob, eShaderType type, const std::string& debug_name);
	/* END */


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

// 	void SelectEntity(u32 e);
// 	void DeselectEntity();
	memory::MemorySegmentHandle& GetMemorySegmentHandle() { return m_SegmentHandle; }

private:
	Engine() = default;
	static Engine* myInstance;
	static IGraphicsInterface* m_GraphicsInterface;
	memory::MemorySegmentHandle m_SegmentHandle;


	bool HasInitiated();



	SLocalTime myLocalTime;


	VirtualFileSystem m_VirtualFileSystem;
	Threadpool m_Threadpool;
	Window m_Window;
	EntityManager m_EntityManager;

	InputHandle* m_InputHandle = nullptr;
	PhysicsManager* m_PhysicsManager = nullptr;
	CFontManager* myFontManager = nullptr;
	CU::TimeManager myTimeManager;
	Synchronizer* mySynchronizer = nullptr;
	Renderer* myRenderer = nullptr;
	Camera* m_Camera = nullptr;


	debug::DebugHandle m_DebugHandle;

	AssetsContainer* myAssetsContainer = nullptr;
	TerrainManager* m_TerrainManager = nullptr;
	LevelFactory* m_LevelFactory = nullptr;
	CSystemMonitor m_SystemMonitor;

	std::bitset<(u16)eEngineStates::_COUNT> m_States;



	float m_DeltaTime = 0.f;

	bool m_PauseInput = false;
	bool m_CameraUseMouse = false;
	bool m_IsEditingEntity;
	bool m_EditLight = false;
	bool m_RenderInstanced = true;
	bool m_EditRender = false;
};

template<typename T>
std::string Engine::LoadModel(std::string filepath, std::string effect, T* pModel, bool thread)
{
	return myAssetsContainer->LoadModel(filepath, effect, pModel, thread);
}

