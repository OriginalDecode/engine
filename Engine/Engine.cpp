#include "stdafx.h"
#include <Engine/Vulkan.h>
#include "AssetsContainer.h"

#include "Renderer.h"
#include "Synchronizer.h"

#include "Terrain.h"
#include "TerrainManager.h"

#include "IGraphicsAPI.h"
#include <d3dcompiler.h>

#include "EditObject.h"

#include "LevelFactory.h"

#include <Input/InputHandle.h>
#include <PostMaster.h>
#include <PhysicsManager.h>

#include <DebugComponent.h>

#include <DebugSystem.h>
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <LightSystem.h>
#include <InputSystem.h>
#include <AISystem.h>
#include <NetworkSystem.h>
#include <CameraSystem.h>

#include <Engine/LightModel.h>

#ifdef _PROFILE
#include <easy/profiler.h>
#include <easy/reader.h>
#endif

#include <Engine/IGraphicsDevice.h>


#define REGISTERCOMPONENT(x) x,
enum RegisteredComponents
{
#include "Components.h"
};
#undef REGISTERCOMPONENT

#define REGISTERCOMPONENT(x) #x,
const char* RegisteredComponentsStr[] = {
#include "Components.h"
};
#undef REGISTERCOMPONENT

bool Engine::HasInitiated()
{
	return (this && m_States[(u16)eEngineStates::INITIATED] == TRUE);
}

Engine* Engine::myInstance = nullptr;
graphics::IGraphicsAPI* Engine::m_API = nullptr;

void Engine::Create()
{
	DL_ASSERT_EXP(myInstance == nullptr, "Instance already created!");
	myInstance = new Engine;
}

void Engine::Destroy()
{
	DL_ASSERT_EXP(myInstance != nullptr, "Can't destroy the instance before it's created. Did you call Destroy twice?");
	SAFE_DELETE(myInstance);
}

Engine* Engine::GetInstance()
{
	DL_ASSERT_EXP(myInstance != nullptr, "Can't Get the instance before it's created. Did you call Destroy twice?");
	return myInstance;
}

bool Engine::Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc)
{
	Randomizer::Create();
	PostMaster::Create();
	//myWindowSize.m_Height = window_height;
	//myWindowSize.m_Width = window_width;

	WindowCreateInfo window_create_info;
	window_create_info.window_height = window_height;
	window_create_info.window_width = window_width;
	window_create_info.window_process = window_proc;
	window_create_info.instance = instance_handle;
	m_Window.Initiate(window_create_info);
	m_Window.SetWindowText("engine");
	m_Window.ShowWindow();


	if (!m_Window.IsWindowActive())
		m_Window.OnActive();


	graphics::CreateInfo create_info;
	create_info.m_HWND = m_Window.GetHWND();
	create_info.m_Instance = instance_handle;
	create_info.m_WindowWidth = m_Window.GetInnerSize().m_Width;
	create_info.m_WindowHeight = m_Window.GetInnerSize().m_Height;
	create_info.m_APIName = "DirectX11";
	m_API = new graphics::DirectX11(create_info);
	m_API->Initiate();


	m_InputHandle = new InputHandle;
	m_InputHandle->Initiate(m_Window.GetHWND(), instance_handle);

	myAssetsContainer = new AssetsContainer;
	myAssetsContainer->Initiate();

	m_TerrainManager = new TerrainManager;

	myFontManager = new CFontManager;
	myFontManager->Initiate();

	m_Synchronizer = new Synchronizer;
	m_Synchronizer->Initiate();

	m_SegmentHandle.Initiate();

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_Window.GetInnerSize().m_Width, m_Window.GetInnerSize().m_Height, 0.01f, 10000.f, 90.f);
	m_Camera->CreateOrthogonalProjection(m_Window.GetInnerSize().m_Width, m_Window.GetInnerSize().m_Height, 0.01f, 100.f);

	m_Renderer = new Renderer(m_Synchronizer);

	m_PhysicsManager = new PhysicsManager;

	m_Threadpool.Initiate("Engine - Worker");

	m_EntityManager.Initiate();

	m_EntityManager.AddSystem<::DebugSystem>(); //Since the engine has it's own debug system, I had to do it like this

#ifndef _EDITOR
	m_EntityManager.AddSystem<PhysicsSystem>();
#endif
	m_EntityManager.AddSystem<RenderSystem>();
	m_EntityManager.AddSystem<LightSystem>();
	//m_EntityManager.AddSystem<InputSystem>();
	//m_EntityManager.AddSystem<NetworkSystem>();
#ifndef _EDITOR
	//m_EntityManager.AddSystem<AISystem>();
#endif 
	//m_EntityManager.AddSystem<CameraSystem>();

	m_LevelFactory = new LevelFactory;
	m_LevelFactory->Initiate();
	return true;
}

bool Engine::CleanUp()
{
	m_EntityManager.CleanUp();

	m_InputHandle->CleanUp();
	SAFE_DELETE(m_InputHandle);
	if (m_InputHandle)
		return false;

	m_Threadpool.CleanUp();

	m_TerrainManager->CleanUp();
	SAFE_DELETE(m_TerrainManager);

	SAFE_DELETE(m_Renderer);

	SAFE_DELETE(m_Camera);
	SAFE_DELETE(myFontManager);


	SAFE_DELETE(m_PhysicsManager);

	SAFE_DELETE(m_LevelFactory);
	SAFE_DELETE(myAssetsContainer);
	SAFE_DELETE(m_Synchronizer);
	//DL_ASSERT_EXP(myAPI->CleanUp(), "Failed to clean up graphics API. Something was not set to null.");
	//SAFE_DELETE(myAPI);
	SAFE_DELETE(m_API);
	PostMaster::Destroy();
	Randomizer::Destroy();
	return true;
}

Camera* Engine::GetCamera()
{
	return m_Camera;
}

TreeDweller* Engine::CreateEntity(const std::string& filepath, CU::Vector3f& position)
{
	return m_LevelFactory->CreateEntitiy(filepath, position);
}

void Engine::Update()
{
	if (!HasInitiated() && m_Synchronizer->HasQuit()) 
		return;

#if !defined(_PROFILE) && !defined(_FINAL)
	//UpdateDebugUI();
	m_DebugHandle.Update();
#endif
	m_DeltaTime = myTimeManager.GetDeltaTime();
	if (m_States[(u16)eEngineStates::LOADING] == FALSE)
	{
		myTimeManager.Update();
		myAssetsContainer->Update();
	}
	m_Renderer->Render();
	m_PhysicsManager->Update();
	m_Threadpool.Update();

}

void Engine::UpdateInput()
{
	if (!m_PauseInput || m_Window.IsWindowActive())
		m_InputHandle->Update(m_DeltaTime);
}

int Engine::RegisterLight()
{
	return m_Renderer->RegisterLight();
}
// 
// HRESULT Engine::CompileShaderFromFile(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message)
// {
// 	std::wstring w_file_path(file_path.begin(), file_path.end());
// 	HRESULT hr = D3DCompileFromFile(
// 		w_file_path.c_str(),
// 		nullptr,
// 		D3D_COMPILE_STANDARD_FILE_INCLUDE,
// 		entrypoint.c_str(),
// 		feature_level.c_str(),
// 		shader_flags,
// 		0,
// 		&out_compiled_shader,
// 		&out_compile_message);
// 	return hr;
// }
// 
// HRESULT Engine::CompileShaderFromMemory(const s8* pData, s32 size, const std::string& source_name, const std::string& entrypoint, const std::string& feature_level, s32 shader_flags, IBlob*& out_shader, IBlob* out_message)
// {
// 	HRESULT hr = D3DCompile(
// 		pData,
// 		size,
// 		source_name.c_str(),
// 		nullptr,
// 		D3D_COMPILE_STANDARD_FILE_INCLUDE,
// 		entrypoint.c_str(),
// 		feature_level.c_str(),
// 		0,
// 		0,
// 		&out_shader,
// 		&out_message);
// 	return hr;
// }

void* Engine::CreateShader(IShaderBlob* pShader, eShaderType type, const cl::HashString& debug_name)
{
	graphics::IGraphicsDevice& device = m_API->GetDevice();
	switch (type)
	{
		case eShaderType::VERTEX:
			return device.CreateVertexShader(pShader, debug_name);
		case eShaderType::PIXEL:
			return device.CreatePixelShader(pShader, debug_name);
		case eShaderType::GEOMETRY:
			return device.CreateGeometryShader(pShader, debug_name);
		case eShaderType::HULL:
			return device.CreateHullShader(pShader, debug_name);
		case eShaderType::DOMAINS:
			return device.CreateDomainShader(pShader, debug_name);
		case eShaderType::COMPUTE:
			return device.CreateComputeShader(pShader, debug_name);
		default:
			DL_ASSERT("Invalid shader type");
	}
	return nullptr;
}

const WindowSize& Engine::GetWindowSize() const
{
	return m_Window.GetWindowSize();
}

const WindowSize& Engine::GetInnerSize() const
{
	return m_Window.GetInnerSize();
}

CFont* Engine::LoadFont(const s8* filepath, u16 aFontWidth, u16 aBorderWidth)
{
	return myFontManager->LoadFont(filepath, aFontWidth, aBorderWidth);
}

float Engine::GetDeltaTime()
{
	return m_DeltaTime;
}

float Engine::GetTotalTime()
{
	return myTimeManager.GetMasterTimer().GetTotalTime().GetSeconds();
}

float Engine::GetFPS()
{
	return myTimeManager.GetFPS();
}

float Engine::GetFrameTime()
{
	return myTimeManager.GetFrameTime();
}

VirtualFileSystem& Engine::GetVFS()
{
	return m_VirtualFileSystem;
}

Texture* Engine::GetTexture(const cl::HashString& filepath)
{
	return myAssetsContainer->GetTexture(filepath);
}

Effect* Engine::GetEffect(const cl::HashString& filepath)
{
	return myAssetsContainer->GetEffect(m_VirtualFileSystem.GetFile(filepath.c_str()).c_str());
}

Model* Engine::GetModel(const cl::HashString& filepath)
{
	return myAssetsContainer->GetModel(filepath);
}

Sprite* Engine::GetSprite(const cl::HashString& filepath)
{
	return myAssetsContainer->GetSprite(filepath);
}

//cl::HashString Engine::LoadModel(const cl::HashString& filepath, std::string effect, bool thread)
//{
//	return myAssetsContainer->LoadModel(filepath, effect, thread);
//}

std::string string_together(u16 time, u16 to_compare)
{
	std::string to_return;
	to_return += (time < to_compare ? ("0" + std::to_string(time)) : std::to_string(time));
	return to_return;
}

std::string Engine::GetLocalTimeAsString()
{
	GetLocalTime();
	std::string return_value = "Local Time : ";
	return_value += string_together(myLocalTime.hour, 10) + ":";
	return_value += string_together(myLocalTime.minute, 10) + ":";
	return_value += string_together(myLocalTime.second, 10);
	return return_value;
}

void Engine::ResetRenderTargetAndDepth()
{
	//GetAPI()->ResetRenderTargetAndDepth();
}

void Engine::ToggleVsync()
{
	m_States[(u16)eEngineStates::USE_VSYNC] = !m_States[(u16)eEngineStates::USE_VSYNC];
}

void Engine::OnAltEnter()
{
	m_Window.OnAltEnter();
	m_API->OnAltEnter();
}

void Engine::OnPause()
{
	myTimeManager.Pause();
}

void Engine::OnResume()
{
	myTimeManager.Start();
}

void Engine::OnExit()
{
	if (HasInitiated())
	{
		m_Synchronizer->Quit();
		CleanUp();
	}
}

void Engine::OnInactive()
{
	if (HasInitiated())
		m_Window.OnInactive();
}

void Engine::OnActive()
{
	if (HasInitiated())
		m_Window.OnActive();
}

void Engine::OnResize()
{
	m_API->OnResize();
}

Synchronizer* Engine::GetSynchronizer()
{
	return m_Synchronizer;
}

const SLocalTime& Engine::GetLocalTime()
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	myLocalTime.hour = time.wHour;
	myLocalTime.minute = time.wMinute;
	myLocalTime.second = time.wSecond;

	return myLocalTime;
}

Terrain* Engine::CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize)
{
	Terrain* newTerrain = m_TerrainManager->GetTerrain(aFile);
	newTerrain->Initiate(aFile, position, aSize);
	m_Renderer->AddTerrain(newTerrain);
	return newTerrain;
}

CU::GrowingArray<TreeDweller*> Engine::LoadLevel(const std::string& level_filepath)
{

	m_States[(u16)eEngineStates::LOADING] = TRUE;

	m_LevelFactory->CreatePBLLevel(8);
	//m_LevelFactory->CreateLevel(level_filepath);

	m_States[(u16)eEngineStates::LOADING] = FALSE;

	return m_LevelFactory->GetDwellers();
}
