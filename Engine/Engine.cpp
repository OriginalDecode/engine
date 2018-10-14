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

bool Engine::HasInitiated()
{
	return true;
}

Engine::Engine()
	: m_PauseInput(false)
	, m_CameraUseMouse(false)
	, m_RenderInstanced(true)
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
	ASSERT(myInstance == nullptr, "Instance already created!");
	myInstance = new Engine;
}

void Engine::Destroy()
{
	ASSERT(myInstance != nullptr, "Can't destroy the instance before it's created. Did you call Destroy twice?");
	SAFE_DELETE(myInstance);
}

Engine* Engine::GetInstance()
{
	ASSERT(myInstance != nullptr, "Can't Get the instance before it's created. Did you call Destroy twice?");
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
	myAssetsContainer = new AssetsContainer;
	myAssetsContainer->Initiate();
	Randomizer::Create();
	m_CurrentSampler = graphics::MSAA_x16;


	m_InputHandle = new InputHandle;
	m_InputHandle->Initiate(m_Window.GetHWND(), instance_handle);

	m_TerrainManager = new TerrainManager;

	myFontManager = new CFontManager;
	myFontManager->Initiate();

	m_Synchronizer = new Synchronizer;
	m_Synchronizer->Initiate();

	//m_SegmentHandle.Initiate();

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_Window.GetWindowSize().m_Width, m_Window.GetWindowSize().m_Height, 0.1f, 1000.f, 90.f); //these variables should probably be exposed to a settings file
	m_Camera->CreateOrthogonalProjection(m_Window.GetWindowSize().m_Width, m_Window.GetWindowSize().m_Height, 0.01f, 100.f);

	m_Renderer = new Renderer(m_Synchronizer);

	m_PhysicsManager = new PhysicsManager;

	m_Threadpool.Initiate("Engine - Worker");



	m_EntityManager.Initiate();

	s32 flags = 0;
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
	AssetFactory::Destroy();
	m_NetManager->CleanUp();
	SAFE_DELETE(m_NetManager);

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
	SAFE_DELETE(m_API);
	EventManager::Destroy();
	Randomizer::Destroy();
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
	myAssetsContainer->Update();

	m_NetManager->Update();
	m_Threadpool.Update();

#if !defined(_PROFILE) && !defined(_FINAL)
	static bool render_imgui = true;
	if (m_InputHandle->GetInputWrapper()->OnDown(KButton::U))
		render_imgui = !render_imgui;

	if (render_imgui)
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
	if (!m_PauseInput || m_Window.IsWindowActive())
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

HashType Engine::LoadModelA(std::string path, std::string effect, bool threaded, int option)
{
	return LoadModel<Model>(path, effect, threaded, option);
}

u32 Engine::PickEntity(Texture* pTexture)
{
	return m_API->PickColor(pTexture).color;
}

void Engine::PickEntity()
{
	m_HasPickedEntity = true;
}

void Engine::AddRenderCommand(const ModelCommand& command)
{
	//const u16 current_buffer = m_Synchronizer->GetCurrentBufferIndex();
	//memory::CommandAllocator& allocator = m_SegmentHandle.GetCommandAllocator(current_buffer ^ 1, 0);
	//void * current = allocator.Alloc(sizeof(ModelCommand));
	//memcpy(current, &command, sizeof(ModelCommand));
}

AssetsContainer* Engine::GetAssetsContainer()
{
	return myAssetsContainer;
}

Model* Engine::GetModelDirect(u64 key)
{
	return myAssetsContainer->GetModelDirect(key);
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

float& Engine::GetDeltaTimeRef()
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

Texture* Engine::GetTexture(u64 key)
{
	return myAssetsContainer->GetTexture(key);
}

Texture* Engine::GetTexture(const char* key)
{
	Texture* texture = myAssetsContainer->GetTexture(cl::Hash(key));

	if (texture)
		return texture;


	u64 hash = myAssetsContainer->LoadTexture(key);
	return myAssetsContainer->GetTexture(hash);
}

Material* Engine::GetMaterial(const char* key)
{
	u64 hash = myAssetsContainer->LoadMaterial(key);
	Material* material = myAssetsContainer->GetMaterial(hash);
	return material;
}

Effect* Engine::GetEffect(u64 key)
{
	return myAssetsContainer->GetEffect(key);
}

Effect* Engine::GetEffect(const char* key)
{
	std::string file = m_VirtualFileSystem.GetFile(key);
	Effect* effect = myAssetsContainer->GetEffect(cl::Hash(file.c_str()));

	if (effect)
		return effect;

	u64 hash = myAssetsContainer->LoadEffect(file);
	return myAssetsContainer->GetEffect(hash);
}


Sprite* Engine::GetSprite(u64 key)
{
	return myAssetsContainer->GetSprite(key);
}

Sprite* Engine::GetSprite(const char* key)
{
	u64 hash = cl::Hash(key);
	Sprite* sprite = myAssetsContainer->GetSprite(hash);

	if (sprite)
		return sprite;


	myAssetsContainer->LoadEffect(key);
	return myAssetsContainer->GetSprite(hash);
}

Material* Engine::GetMaterial(u64 key)
{
	return myAssetsContainer->GetMaterial(key);
}

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
	//m_States[(u16)eEngineStates::USE_VSYNC] = !m_States[(u16)eEngineStates::USE_VSYNC];
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

u64 Engine::LoadTexture(const std::string& path, bool make_mips)
{
	return myAssetsContainer->LoadTexture(m_VirtualFileSystem.GetFile(path), make_mips);
}

u64 Engine::LoadEffect(const std::string& path)
{
	return myAssetsContainer->LoadEffect(m_VirtualFileSystem.GetFile(path));
}

u64 Engine::LoadSprite(const std::string& path)
{
	return myAssetsContainer->LoadSprite(m_VirtualFileSystem.GetFile(path));
}

void Engine::AddTexture(Texture* pTexture, u64 key)
{
	myAssetsContainer->AddTexture(pTexture, key);
}

