#pragma once
#define _WINSOCKAPI_
#include <network/network_api.h>
#include "Window.h"
#include "VirtualFileSystem.h"
#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>

#include <Timer/TimeManager/TimeManager.h>
//#include <TimeManager/TimeManager.h>
//#include <TimeManager.h>

#include "engine_shared.h"
#include <string>
#include "../EntitySystem/EntityManager.h"
#include "ShaderFactory.h"
#include <Engine/SystemMonitor.h>
#include <Engine/MemorySegmentHandle.h>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

class InputHandle;
class PhysicsManager;

struct SLocalTime
{
	u16 hour;
	u16 minute;
	u16 second;
};

class AssetsContainer;
class TerrainManager;

typedef struct ID3D10Blob IBlob;
class TreeDweller;
class LevelFactory;

enum eDeferredType;


class DirectX11;
class Vulkan;

class Camera;
class CConsole;
class CFont;
class CFontManager;
class Model;
class Renderer;
class Synchronizer;
class Texture;
class Effect;
class Terrain;
class IGraphicsAPI;
class Sprite;

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
	void Render();
	void UpdateInput();

	int RegisterLight();

	static IGraphicsAPI* GetGraphicsAPI();
	static DirectX11* GetAPI();
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
	const SLocalTime& GetLocalTime();
	Window& GetWindow() { return m_Window; }
	std::string GetLocalTimeAsString();
	InputHandle* GetInputHandle() { return m_InputHandle; }

	//_________________________________________
	// Level Creation, Loading, Saving
	Terrain* CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize);
	CU::GrowingArray<TreeDweller*> LoadLevel(const std::string& level_filepath);

	//_________________________________________
	// Shader Creation
	//This should probably be moved to the graphics API instead.
	HRESULT CompileShaderFromFile(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message);
	HRESULT CompileShaderFromMemory(const s8* pData, s32 size, const std::string& source_name, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_shader, IBlob* out_message);

	void* CreateShader(IBlob* compiled_shader_blob, eShaderType type, const std::string& debug_name);
	//CompiledShader CreateShader(IBlob* compiled_shader_blob, const std::string& shader_type, const std::string& debug_name, bool use);


	bool UseMouse() { return m_CameraUseMouse; }
	void ToggleUseMouse() { m_CameraUseMouse = !m_CameraUseMouse; }

	const HWND& GetHWND() const { return myHWND; }

	enum class eEngineStates
	{
		USE_VSYNC,
		INITIATED,
		LOADING,
		_COUNT
	};

	void SelectEntity(u32 e);
	void DeselectEntity();
	memory::MemorySegmentHandle& GetMemorySegmentHandle() { return m_SegmentHandle; }
	struct ID3D11ShaderResourceView;
#if !defined(_PROFILE) && !defined(_FINAL)
	bool SaveLevel();
	bool GetLineRendering();
	void EditEntity();
	void DebugTextures();
	void AddTexture(Texture* texture, const std::string& debug_name);
	void AddTexture(void* srv, const std::string& debug_name);

	void RegisterCheckBox(bool* pBool, const std::string& box_name)
	{
		CheckBox box;
		box.m_Name = box_name;
		box.m_Toggle = pBool;
		m_Checkboxes.Add(box);
	}

	void AddFunction(const std::string& label, std::function<void()> function);
	void AddCheckBox(bool* toggle, std::string label);
	void RegisterFloatSider(float* v, const char* label, float min, float max);
	CU::GrowingArray<ID3D11ShaderResourceView*>& GetDebugTextures() { return m_DebugTextures;}
private:
	struct CheckBox
	{
		std::string m_Name;
		bool* m_Toggle = false;
	};
	CU::GrowingArray<CheckBox> m_Checkboxes;
	struct slider
	{
		float* current_value;
		const char* label;
		float min = 0.f;
		float max = 1.f;
	};
	CU::GrowingArray<slider> m_Sliders;
	void UpdateDebugUI();
	CU::GrowingArray<ID3D11ShaderResourceView*> m_DebugTextures;
	std::vector<std::string> m_Labels;
	std::vector<std::string> m_Levels;
	typedef std::function<void()> callback;
	std::vector<std::pair<std::string, callback>> m_Functions;

#endif
private:

	bool m_RenderInstanced = true;
public:
	bool GetRenderInstanced() { return m_RenderInstanced; }

private:
	memory::MemorySegmentHandle m_SegmentHandle;

	bool m_EditLight = false;
	bool m_EditRender = false;


	Engine() = default;
	//void AddEntitySystems();
	u32 m_EntityToEdit = 0;
	bool m_IsEditingEntity;


	bool HasInitiated();
	bool m_CameraUseMouse = false;

	static Engine* myInstance;
	static IGraphicsAPI* myAPI;

	Threadpool m_Threadpool;
	VirtualFileSystem m_VirtualFileSystem;

	SLocalTime myLocalTime;
	HWND myHWND;
	Window m_Window;
	//DebugSystem m_DebugSystem;
	EntityManager m_EntityManager;

	InputHandle* m_InputHandle = nullptr;

	PhysicsManager* m_PhysicsManager = nullptr;

	CFontManager* myFontManager = nullptr;
	CU::TimeManager myTimeManager;
	Synchronizer* mySynchronizer = nullptr;
	Renderer* myRenderer = nullptr;

	Camera* m_Camera = nullptr;


	CConsole* myConsole = nullptr;
	AssetsContainer* myAssetsContainer = nullptr;
	TerrainManager* m_TerrainManager = nullptr;
	LevelFactory* m_LevelFactory = nullptr;

	std::bitset<(u16)eEngineStates::_COUNT> m_States;
	CSystemMonitor m_SystemMonitor;
	bool m_PauseInput = false;
	float m_DeltaTime = 0.f;




};

template<typename T>
std::string Engine::LoadModel(std::string filepath, std::string effect, T* pModel, bool thread)
{
	return myAssetsContainer->LoadModel(filepath, effect, pModel, thread);
}

