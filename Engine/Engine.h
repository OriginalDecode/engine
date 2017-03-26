#pragma once

#include "Window.h"

#include <CommonLib/Math/Vector/Vector.h>
#include <CommonLib/Threadpool.h>
#include <TimeManager.h>
#include "snowblind_shared.h"
#include <string>

#include "../EntitySystem/EntityManager.h"
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

	static IGraphicsAPI* GetGraphicsAPI();
#ifdef SNOWBLIND_DX11
	static DirectX11* GetAPI();
#else
	static Vulkan* GetAPI();
#endif
	//_________________________________________
	// Settings
	void ToggleVsync();

	Camera* GetCamera();

	TreeDweller* CreateEntity(const std::string& filepath, CU::Vector3f& position);

	static void Present();
	static void Clear();
	static void EnableZ();
	static void DisableZ();

	static void ToggleWireframe();

	const WindowSize& GetWindowSize() const;
	const WindowSize& GetInnerSize() const;

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

	std::string LoadModel(std::string aFilePath, std::string effect);

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
	Synchronizer* GetSynchronizer() { return mySynchronizer; }
	EntityManager& GetEntityManager() { return m_EntityManager; }
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
	CU::GrowingArray<TreeDweller*> LoadLevel(const std::string& level_filepath);


	bool SaveLevel();

	//_________________________________________
	// Shader Creation
	//This should probably be moved to the graphics API instead.
	HRESULT CompileShaderFromFile(const std::string& file_path, const std::string& shader_type, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message);
	void* CreateShader(IBlob* compiled_shader_blob, const std::string& shader_type, const std::string& debug_name);


	bool UseMouse() { return m_CameraUseMouse; }
	void ToggleUseMouse() { m_CameraUseMouse = !m_CameraUseMouse; }

	const HWND& GetHWND() const { return myHWND; }

	void SelectEntity(u32 e);
	void DeselectEntity();
	void EditEntity();
	void ToggleFrame() { m_Frame ^= 1; }
	s32 GetFrame() { return m_Frame; }

	enum class eEngineStates
	{
		USE_VSYNC,
		INITIATED,
		LOADING,
		_COUNT
	};

#ifdef _DEBUG
	void OutputDebugString(std::string debug_str);
#endif
private:
#ifdef _DEBUG

	struct debug_string
	{
		std::string m_String;
		CU::Vector3f m_Color;
	};

	CU::StaticArray<std::string, 8> m_DebugStrings;
#endif
	void UpdateDebugUI();

	bool m_EditLight = false;
	//void EditLightComponent();
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

	//Shouldn't need 2 cameras?
	Camera* m_Camera = nullptr;


	CConsole* myConsole = nullptr;
	AssetsContainer* myAssetsContainer = nullptr;
	TerrainManager* m_TerrainManager = nullptr;
	LevelFactory* m_LevelFactory = nullptr;

	std::bitset<(u16)eEngineStates::_COUNT> m_States;

	bool m_PauseInput = false;
	float m_DeltaTime = 0.f;
	s32	m_Frame = 0;
};

