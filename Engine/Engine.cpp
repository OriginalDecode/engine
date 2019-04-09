#include "stdafx.h"
#include <Engine/Vulkan.h>
#include "AssetsContainer.h"

#include "Renderer.h"
#include "Synchronizer.h"

#include "Terrain.h"
#include "TerrainManager.h"

#include "IGraphicsAPI.h"

#include "EditObject.h"

#include "LevelFactory.h"

#include <Input/InputHandle.h>
#include <EventManager.h>
#include <PhysicsManager.h>


#include <Engine/LightModel.h>

#include <Engine/IGraphicsDevice.h>

#if !defined(_PROFILE) && !defined(_FINAL)
#include <Engine/DebugHandle.h>
#include <imgui/ImGuizmo.h>
#include <imgui/imgui.h>
#endif

#include <Engine/AssetFactory.h>
#include <network/NetworkManager.h>
#include <Engine/RenderCommand.h>

#include <Engine/DebugRenderer.h>

#include <Input/InputManager.h>

#include <Engine/AssetsContainer.h>


bool Engine::HasInitiated()
{
	return true;
}

Engine::Engine()
	: m_PauseInput(false)
	, m_CameraUseMouse(false)
	, m_RenderInstanced(true)
	, m_VSyncOn(false)
{
	DebugRenderer::Create();
}

Engine::~Engine()
{
	DebugRenderer::Destroy();
}

Engine* Engine::myInstance = nullptr;
graphics::IGraphicsAPI* Engine::m_API = nullptr;

void Engine::Create()
{
	if(!myInstance)
		myInstance = new Engine;
}

void Engine::Destroy()
{
	SAFE_DELETE(myInstance);
}

Engine* Engine::GetInstance()
{
	return myInstance;
}

const CU::Vector3f& Engine::GetLightDir()
{
	return m_Renderer->GetLightDirection();
}

bool Engine::Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc)
{
	wndproc = window_proc; //WNDPROC
	hinstance = instance_handle; // HINSTANCE


	WindowCreateInfo window_create_info;
	window_create_info.window_height = window_height;
	window_create_info.window_width = window_width;
	window_create_info.window_process = window_proc;
	window_create_info.instance = instance_handle;
	m_Window.Initiate(window_create_info, WS_OVERLAPPEDWINDOW);
	m_Window.SetWindowText("engine");
	m_Window.ShowWindow();

	if (!m_Window.IsWindowActive())
		m_Window.OnActive();

	graphics::CreateInfo create_info;
	create_info.m_HWND = m_Window.GetHWND();
	create_info.m_Instance = instance_handle;
	create_info.m_WindowWidth = m_Window.GetInnerSize().m_Width;
	create_info.m_WindowHeight = m_Window.GetInnerSize().m_Height;

	m_InputManager = new Input::InputManager(m_Window.GetHWND(), instance_handle);

#ifdef _DEBUG
	ImGui::CreateContext();
#endif
	m_API = new graphics::DirectX11(create_info);
	m_API->Initiate();


	EventManager::Create();
#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::Create();
#endif

	AssetFactory::Create();
	//myAssetsContainer = new AssetsContainer;
	//myAssetsContainer->Initiate();

	AssetsContainer* ac = AssetsContainer::GetInstance();
	ac->Initiate();


	m_InputHandle = new InputHandle;
	m_InputHandle->Initiate(m_Window.GetHWND(), instance_handle);

	m_TerrainManager = new TerrainManager;

	myFontManager = new CFontManager;
	myFontManager->Initiate();

	m_Synchronizer = new Synchronizer;

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_Window.GetWindowSize().m_Width, m_Window.GetWindowSize().m_Height, 0.1f, 1000.f, 90.f); //these variables should probably be exposed to a settings file
	m_Camera->CreateOrthogonalProjection(m_Window.GetWindowSize().m_Width, m_Window.GetWindowSize().m_Height, 0.01f, 100.f);

	m_Renderer = new Renderer(m_Synchronizer);

	m_PhysicsManager = new PhysicsManager;

	m_Threadpool.Initiate("Engine - Worker");



	m_EntityManager.Initiate();

	int32 flags = 0;
	flags |= EntityManager::RENDER;
	flags |= EntityManager::PHYSICS;
	flags |= EntityManager::LIGHT;
	flags |= EntityManager::DEBUG;
	flags |= EntityManager::NETWORK;

	m_EntityManager.AddSystem(flags);

	m_NetManager = new network::NetworkManager;

	m_LevelFactory = new LevelFactory;
	m_LevelFactory->Initiate();

	return true;
}

bool Engine::CleanUp()
{
#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::Destroy();
#endif
	SAFE_DELETE(m_InputManager);
	AssetFactory::Destroy();
	m_NetManager->CleanUp();
	SAFE_DELETE(m_NetManager);

	SAFE_DELETE(m_InputHandle);

	m_Threadpool.CleanUp();

	m_TerrainManager->CleanUp();
	SAFE_DELETE(m_TerrainManager);

	SAFE_DELETE(m_Renderer);

	SAFE_DELETE(m_Camera);
	SAFE_DELETE(myFontManager);


	SAFE_DELETE(m_PhysicsManager);

	SAFE_DELETE(m_LevelFactory);
	SAFE_DELETE(m_Synchronizer);
	SAFE_DELETE(m_API);
	EventManager::Destroy();
	return true;
}

Camera* Engine::GetCamera()
{
	return m_Camera;
}

void Engine::Update()
{
	if (!HasInitiated() && m_Synchronizer->HasQuit())
		return;
	m_DeltaTime = myTimeManager.GetDeltaTime();
	m_PhysicsManager->Update();

	myTimeManager.Update();

	m_NetManager->Update();
	m_Threadpool.Update();

#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::GetInstance()->Update();
#endif

	m_Camera->Update();
	m_Renderer->Render();

#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::GetInstance()->SetHoveredModel(m_API->PickColor(GetTexture("entity_id")).color);
#endif
}

void Engine::UpdateInput()
{
	if (m_PauseInput || !m_Window.IsWindowActive())
		return;

	m_InputManager->Update();
	m_InputHandle->Update(m_DeltaTime);
}

int Engine::RegisterLight()
{
	return m_Renderer->RegisterLight();
}

void* Engine::CreateShader(IShaderBlob* pShader, eShaderType type, const std::string& debug_name)
{
	graphics::IGraphicsDevice& device = m_API->GetDevice();
	switch (type)
	{
	case EShaderType_VERTEX:
		return device.CreateVertexShader(pShader, debug_name);
	case EShaderType_PIXEL:
		return device.CreatePixelShader(pShader, debug_name);
	case EShaderType_GEOMETRY:
		return device.CreateGeometryShader(pShader, debug_name);
	case EShaderType_HULL:
		return device.CreateHullShader(pShader, debug_name);
	case EShaderType_DOMAIN:
		return device.CreateDomainShader(pShader, debug_name);
	case EShaderType_COMPUTE:
		return device.CreateComputeShader(pShader, debug_name);
	default:
		DL_ASSERT("Invalid shader type");
	}
	return nullptr;
}

Model* Engine::GetModelDirect(uint64 key)
{
	return AssetsContainer::GetInstance()->GetModelDirect(key);
}

const WindowSize& Engine::GetWindowSize() const
{
	return m_Window.GetWindowSize();
}

const WindowSize& Engine::GetInnerSize() const
{
	return m_Window.GetInnerSize();
}

CFont* Engine::LoadFont(const int8* filepath, uint16 aFontWidth, uint16 aBorderWidth)
{
	return myFontManager->LoadFont(filepath, aFontWidth, aBorderWidth);
}

VirtualFileSystem& Engine::GetVFS()
{
	return m_VirtualFileSystem;
}

Texture* Engine::GetTexture(uint64 key)
{
	return AssetsContainer::GetInstance()->GetTexture(key);
}

Texture* Engine::GetTexture(const char* key)
{
	Texture* texture = AssetsContainer::GetInstance()->GetTexture(cl::Hash(key));

	if (texture)
		return texture;


	uint64 hash = AssetsContainer::GetInstance()->LoadTexture(key);
	return AssetsContainer::GetInstance()->GetTexture(hash);
}

Material* Engine::GetMaterial(const char* key)
{
	uint64 hash = AssetsContainer::GetInstance()->LoadMaterial(key);
	Material* material = AssetsContainer::GetInstance()->GetMaterial(hash);
	return material;
}

Effect* Engine::GetEffect(uint64 key)
{
	return AssetsContainer::GetInstance()->GetEffect(key);
}

Effect* Engine::GetEffect(const char* key)
{
	std::string file = m_VirtualFileSystem.GetFile(key);
	Effect* effect = AssetsContainer::GetInstance()->GetEffect(cl::Hash(file.c_str()));

	if (effect)
		return effect;

	uint64 hash = AssetsContainer::GetInstance()->LoadEffect(file);
	return AssetsContainer::GetInstance()->GetEffect(hash);
}




CompiledShader* Engine::GetShader(const char* key)
{
	return GetShader(cl::Hash(key));
}

CompiledShader* Engine::GetShader(uint64 key)
{
	return AssetsContainer::GetInstance()->GetShader(key);
}

Sprite* Engine::GetSprite(uint64 key)
{
	return AssetsContainer::GetInstance()->GetSprite(key);
}

Sprite* Engine::GetSprite(const char* key)
{
	uint64 hash = cl::Hash(key);
	Sprite* sprite = AssetsContainer::GetInstance()->GetSprite(hash);

	if (sprite)
		return sprite;


	AssetsContainer::GetInstance()->LoadEffect(key);
	return AssetsContainer::GetInstance()->GetSprite(hash);
}

Material* Engine::GetMaterial(uint64 key)
{
	return AssetsContainer::GetInstance()->GetMaterial(key);
}

std::string string_together(uint16 time, uint16 to_compare)
{
	std::string to_return;
	to_return += (time < to_compare ? ("0" + std::to_string(time)) : std::to_string(time));
	return to_return;
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
	m_Synchronizer->Quit();
	CleanUp();
}

void Engine::OnInactive()
{
	m_Window.OnInactive();
}

void Engine::OnActive()
{
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

uint64 Engine::LoadTexture(const std::string& path, bool make_mips)
{
	return AssetsContainer::GetInstance()->LoadTexture(m_VirtualFileSystem.GetFile(path), make_mips);
}

uint64 Engine::LoadEffect(const std::string& path)
{
	return AssetsContainer::GetInstance()->LoadEffect(m_VirtualFileSystem.GetFile(path));
}

uint64 Engine::LoadSprite(const std::string& path)
{
	return AssetsContainer::GetInstance()->LoadSprite(m_VirtualFileSystem.GetFile(path));
}

uint64 Engine::LoadShader(const std::string& path, const std::string& entrypoint)
{
	return AssetsContainer::GetInstance()->LoadShader(path, entrypoint);
}

void Engine::AddTexture(Texture* pTexture, uint64 key)
{
	AssetsContainer::GetInstance()->AddTexture(pTexture, key);
}

