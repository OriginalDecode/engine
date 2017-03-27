#include "stdafx.h"


#include <DebugSystem.h>
#include <PhysicsSystem.h>
#include <RenderSystem.h>
#include <LightSystem.h>
#include <InputSystem.h>
#include <AISystem.h>
#include <NetworkSystem.h>
#include <CameraSystem.h>

#include "AssetsContainer.h"

#include "Renderer.h"
#include "Synchronizer.h"

#include "Terrain.h"
#include "TerrainManager.h"

//#include <EntityManager.h>
#include <PhysicsManager.h>


#include "IGraphicsAPI.h"
#include <d3dcompiler.h>

#include <Input/InputHandle.h>
#include <PostMaster.h>

#include "EditObject.h"
#include <DebugComponent.h>

#include "imgui_impl_dx11.h"
#include "LevelFactory.h"

static constexpr char* vertex_shader = "VS";
static constexpr char* pixel_shader = "PS";
static constexpr char* geometry_shader = "GS";
static constexpr char* hull_shader = "HS";
static constexpr char* domain_shader = "DS";
static constexpr char* compute_shader = "CS";


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
IGraphicsAPI* Engine::myAPI = nullptr;

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

#ifdef SNOWBLIND_DX11
DirectX11* Engine::GetAPI()
{
	return static_cast<DirectX11*>(myAPI);
}
#else
Vulkan* Engine::GetAPI()
{
	return static_cast<Vulkan*>(myAPI);
}
#endif

/*bool Engine::InitiateDebugSystem(InputHandle* input_handle)
{
	m_DebugSystem.Initiate(input_handle);
	return true;
}*/

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
	m_Window.ShowWindow();
	myHWND = m_Window.GetHWND();

	if (!m_Window.IsWindowActive())
		m_Window.OnActive();
	SetWindowText(myHWND, "Snowblind Engine");
#ifdef SNOWBLIND_DX11
	myAPI = new DirectX11;
	const char* api_name = "DirectX11";
#else
	myAPI = new Vulkan;
	const char* api_name = "Vulkan";
#endif

	CreateInfo create_info;
	create_info.m_HWND = myHWND;
	create_info.m_Instance = instance_handle;
	create_info.m_WindowWidth = window_width;
	create_info.m_WindowHeight = window_height;
	create_info.m_APIName = api_name;

	DL_ASSERT_EXP(myAPI->Initiate(create_info), "Engine : Failed to initiate graphicsAPI");

	myAssetsContainer = new AssetsContainer;
	myAssetsContainer->Initiate();

	m_TerrainManager = new TerrainManager;

	myFontManager = new CFontManager;
	myFontManager->Initiate();

	mySynchronizer = new Synchronizer;
	DL_ASSERT_EXP(mySynchronizer->Initiate(), "Engine : Failed to Initiate Synchronizer!");

	/*m_DebugSystem.Initiate(m_InputHandle);

	m_DebugSystem.AddDebugMenuItem("Toggle VSync", [&]()
	{
		ToggleVsync();
	});*/

	//myCamera = new Camera(myWindowSize.myWidth, myWindowSize.myHeight);

	m_Camera = new Camera;
	m_Camera->CreatePerspectiveProjection(m_Window.GetInnerSize().m_Width, m_Window.GetInnerSize().m_Height, 0.01f, 10000.f, 90.f);
	m_Camera->CreateOrthogonalProjection(m_Window.GetInnerSize().m_Width, m_Window.GetInnerSize().m_Height, 0.01f, 100.f);

	//my2DCamera = new Camera(myWindowSize.myWidth, myWindowSize.myHeight, CU::Vector3f(0, 0, 0.f));

	myRenderer = new Renderer;
	DL_ASSERT_EXP(myRenderer->Initiate(mySynchronizer, m_Camera), "Engine : Failed to initiate Renderer!");


	m_PhysicsManager = new PhysicsManager;

	m_Threadpool.Initiate();

	m_InputHandle = new InputHandle;
	m_InputHandle->Initiate(myHWND, instance_handle);
	m_InputHandle->AddController(0);

	m_EntityManager.Initiate();

	m_EntityManager.AddSystem<::DebugSystem>(); //Since the engine has it's own debug system, I had to do it like this

#ifndef _EDITOR
	m_EntityManager.AddSystem<PhysicsSystem>();
#endif
	m_EntityManager.AddSystem<RenderSystem>();
	m_EntityManager.AddSystem<LightSystem>();
	m_EntityManager.AddSystem<InputSystem>();
	m_EntityManager.AddSystem<NetworkSystem>();
#ifndef _EDITOR
	m_EntityManager.AddSystem<AISystem>();
#endif 

	m_EntityManager.AddSystem<CameraSystem>();


	//ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	ImGui_ImplDX11_Init(myHWND, GetAPI()->GetDevice(), GetAPI()->GetContext());

	m_States[(u16)eEngineStates::INITIATED] = TRUE;
	m_LevelFactory = new LevelFactory;
	m_LevelFactory->Initiate();

	return true;
}

bool Engine::CleanUp()
{
	ImGui_ImplDX11_Shutdown();
	m_EntityManager.CleanUp();

	m_InputHandle->CleanUp();
	SAFE_DELETE(m_InputHandle);
	if (m_InputHandle)
		return false;

	m_Threadpool.CleanUp();
	SAFE_DELETE(myAssetsContainer);
	SAFE_DELETE(mySynchronizer);

	m_TerrainManager->CleanUp();
	SAFE_DELETE(m_TerrainManager);

	myRenderer->CleanUp();
	SAFE_DELETE(myRenderer);

	SAFE_DELETE(m_Camera);
	SAFE_DELETE(myFontManager);


	SAFE_DELETE(m_PhysicsManager);

	DL_ASSERT_EXP(myAPI->CleanUp(), "Failed to clean up graphics API. Something was not set to null.");
	SAFE_DELETE(myAPI);
	SAFE_DELETE(m_LevelFactory);
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


	if (!HasInitiated())
		return;

	UpdateDebugUI();

	m_DeltaTime = myTimeManager.GetDeltaTime();
	if (m_States[(u16)eEngineStates::LOADING] == FALSE)
	{
		myTimeManager.Update();
		myAssetsContainer->Update();
	}
	myRenderer->Render();

	m_Threadpool.Update();

}

void Engine::UpdateInput()
{
	if (!m_PauseInput)
		m_InputHandle->Update(m_DeltaTime);
}

IGraphicsAPI* Engine::GetGraphicsAPI()
{
	return myAPI;
}

HRESULT Engine::CompileShaderFromFile(const std::string& file_path, const std::string& shader_type, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message)
{
	std::wstring w_file_path(file_path.begin(), file_path.end());
	HRESULT hr = D3DCompileFromFile(
		w_file_path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shader_type.c_str(),
		feature_level.c_str(),
		shader_flags,
		0,
		&out_compiled_shader,
		&out_compile_message);
	return hr;
}

void* Engine::CreateShader(IBlob* compiled_shader_blob, const std::string& shader_type, const std::string& debug_name)
{

	IDevice* device = myAPI->GetDevice();
	if (shader_type.find(vertex_shader) != shader_type.npos)
	{
		IVertexShader* shader = nullptr;
		HRESULT hr = device->CreateVertexShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Vertex Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	else if (shader_type.find(pixel_shader) != shader_type.npos)
	{
		IPixelShader* shader = nullptr;
		HRESULT hr = device->CreatePixelShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Pixel Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	else if (shader_type.find(geometry_shader) != shader_type.npos)
	{
		IGeometryShader* shader = nullptr;
		HRESULT hr = device->CreateGeometryShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Geometry Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	else if (shader_type.find(hull_shader) != shader_type.npos)
	{
		IHullShader* shader = nullptr;
		HRESULT hr = device->CreateHullShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Hull Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	else if (shader_type.find(domain_shader) != shader_type.npos)
	{
		IDomainShader* shader = nullptr;
		HRESULT hr = device->CreateDomainShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Domain Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	else if (shader_type.find(compute_shader) != shader_type.npos)
	{
		IComputeShader* shader = nullptr;
		HRESULT hr = device->CreateComputeShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
		GetAPI()->HandleErrors(hr, "Failed to create Compute Shader!");
		GetAPI()->SetDebugName(shader, debug_name);
		void* to_return = shader;
		return to_return;
	}
	TRACE_LOG("FAILED TO CREATE ANY SHADER! TYPE NOT FOUND!");
	DL_ASSERT("FAILED TO CREATE ANY SHADER! TYPE NOT FOUND!");
	return nullptr;
}

void Engine::EditEntity()
{
	if (!m_IsEditingEntity)
		return;

	DebugComponent& debug = m_EntityManager.GetComponent<DebugComponent>(m_EntityToEdit);
	EditObject& to_edit = debug.m_EditObject;

	std::stringstream ss;
	ss << "Entity : " << m_EntityToEdit;
	ImGui::SetNextWindowPos(ImVec2(300.f, 0));
	ImGui::SetNextWindowSize(ImVec2(300.f, 600.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	if (ImGui::Begin(ss.str().c_str(), &m_IsEditingEntity, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		to_edit.Update();
		ImGui::End();
	}
	ImGui::PopStyleVar();
}

void Engine::SelectEntity(u32 e)
{
	m_EntityToEdit = e;
	m_IsEditingEntity = true;
}

void Engine::DeselectEntity()
{
	m_EntityToEdit = 0;
	m_IsEditingEntity = false;
}

void Engine::Present()
{
	if (myInstance->m_States[(u16)eEngineStates::USE_VSYNC] == TRUE)
		myAPI->Present(1, 0);
	else
		myAPI->Present(0, 0);
}

void Engine::Clear()
{
	myAPI->Clear();
}

void Engine::EnableZ()
{
	myAPI->EnableZBuffer();
}

void Engine::DisableZ()
{
	myAPI->DisableZBuffer();
}

void Engine::ToggleWireframe()
{
	myInstance->myRenderer->ToggleWireframe();
}

const WindowSize& Engine::GetWindowSize() const
{
	return m_Window.GetWindowSize();
}

const WindowSize& Engine::GetInnerSize() const
{
	return m_Window.GetInnerSize();
}

CFont* Engine::LoadFont(const s8* aFilepath, u16 aFontWidth, u16 aBorderWidth)
{
	return myFontManager->LoadFont(aFilepath, aFontWidth, aBorderWidth);
}

float Engine::GetDeltaTime()
{
	return m_DeltaTime;
}

float Engine::GetFPS()
{
	return myTimeManager.GetFPS();
}

float Engine::GetFrameTime()
{
	return myTimeManager.GetFrameTime();
}

std::string Engine::GetAPIName()
{
	return myAPI->GetAPIName();
}

Texture* Engine::GetTexture(const std::string& aFilePath)
{
	return myAssetsContainer->GetTexture(aFilePath);
}

Effect* Engine::GetEffect(const std::string& aFilePath)
{
	return myAssetsContainer->GetEffect(aFilePath);
}

CModel* Engine::GetModel(const std::string& aFilePath)
{
	return myAssetsContainer->GetModel(aFilePath);
}

std::string Engine::LoadModel(std::string aFilePath, std::string effect)
{
	//m_Threadpool.AddWork(Work([&]() {
	myAssetsContainer->LoadModel(aFilePath, effect);
	return aFilePath;
	/*	}));
		return aFilePath;*/
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
#ifdef SNOWBLIND_DX11
	GetAPI()->ResetRenderTargetAndDepth();
#endif
}

void Engine::ToggleVsync()
{
	m_States[(u16)eEngineStates::USE_VSYNC] = !m_States[(u16)eEngineStates::USE_VSYNC];
}

void Engine::OnAltEnter()
{
	if (myAPI)
		myAPI->OnAltEnter();
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
		mySynchronizer->Quit();
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
	if (myAPI)
		myAPI->OnResize();
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

CTerrain* Engine::CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize)
{
	CTerrain* newTerrain = m_TerrainManager->GetTerrain(aFile);
	newTerrain->Initiate(aFile, position, aSize);
	myRenderer->AddTerrain(newTerrain);
	return newTerrain;
}

CU::GrowingArray<TreeDweller*> Engine::LoadLevel(const std::string& level_filepath)
{
	m_States[(u16)eEngineStates::LOADING] = TRUE;
	//m_IsLoadingLevel = true;
	m_LevelFactory->CreateLevel(level_filepath);

	m_States[(u16)eEngineStates::LOADING] = FALSE;
	return m_LevelFactory->GetDwellers();
}

bool Engine::SaveLevel()
{
	//Should write the entire file
	//Prompt dialogue to give it a file name
	static char file_name[512];
	if (ImGui::Begin("Filename"))
	{

		ImGui::InputText("filename", file_name, 512);
		if (ImGui::Button("Save", ImVec2(100, 30)))
		{
			if (!CL::substr(file_name, ".level"))
			{
				DL_WARNINGBOX("Incorrect filetype, no file created!");
			}
			else
			{
				std::ofstream f(file_name);
				f << "Hello World!";
				f.flush();
				f.close();
				m_PauseInput = false;
				ImGui::End();
				return true;
			}
		}

	}
	ImGui::End();

	return false;
}

Threadpool& Engine::GetThreadpool()
{
	return m_Threadpool;
}

#ifdef _DEBUG
void Engine::OutputDebugString(std::string debug_str)
{
}
#endif

void Engine::UpdateDebugUI()
{
#ifdef _DEBUG
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(300, GetWindowSize().m_Height));

	ImGuiWindowFlags flags = 0;

	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	//flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	static bool pOpen = false;
	static bool new_window = false;
	static bool save = false;
	if (ImGui::Begin("Information", &pOpen, flags))
	{
		ImGui::Text("Delta Time : %.3f", GetDeltaTime());

		ImGui::Separator();

		if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
		{
			new_window = !new_window;
		}

		if (ImGui::Button("Save Level", ImVec2(100.f, 25.f)))
		{
			save = !save;
			m_PauseInput = true;
		}

		if (save)
			if (SaveLevel())
				save = !save;

		
		static bool tonemapping_hdr = true;
		ImGui::Checkbox("Tonemapping/HDR", &tonemapping_hdr);
		if ( tonemapping_hdr )
			myRenderer->GetPostprocessManager().SetPassesToProcess(PostProcessManager::HDR);
		else
			myRenderer->GetPostprocessManager().RemovePassToProcess(PostProcessManager::HDR);
		ImGui::SameLine();
		ImGui::Checkbox("Debug Textures", &myRenderer->GetPostprocessManager().GetHDRPass().toggle_debug);

		float fov_value = m_Camera->GetFOV();
		ImGui::SliderFloat("FOV", &fov_value, 60.f, 120.f,"%.f");
		m_Camera->SetFOV(fov_value);



		ImGui::End();
	}
	ImGui::PopStyleVar();


	if (new_window)
	{
		ImGui::SetNextWindowPos(ImVec2(300.f, 0.f));
		//ImGui::SetNextWindowSize(ImVec2(300.f, 300.f));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		if (ImGui::Begin("New Window", &new_window, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::Text("Entity Creation");
			ImGui::Separator();
			static bool new_bp = false;
			static bool new_entity = false;
			if (ImGui::Button("New Blueprint", ImVec2(100.f, 25.f)))
			{
				new_bp = !new_bp;
			}
			ImGui::SameLine();
			if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
			{
				new_entity = !new_entity;
			}




			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Create and Edit a new Blueprint.");
				ImGui::EndTooltip();
			}
			ImGui::Separator();

			if (new_bp)
			{
				static bool new_component = false;
				static const int item_count = ARRAYSIZE(RegisteredComponentsStr) - 1;
				static int components = 0;

				static int selected[item_count] = { 0 };
				static char* str = { "Translation\0Graphics\0Physics\0AI\0Network\0Input" };

				ImGui::BeginChildFrame(0, ImVec2(ImGui::GetWindowSize().x, 200.f));
				static bool edit_t = false;
				if (new_component)
				{
					for (int i = 0; i < components; i++)
					{
						std::stringstream ss;
						ss << "Box_" << i;
						std::stringstream ss2;
						ss2 << "X_" << i;
						std::stringstream ss3;
						ss3 << "Edit_" << i;
						if (ImGui::Button(ss2.str().c_str()))
						{
							for (int j = i; j < components - 1; j++)
							{
								selected[i] = selected[j + 1];
							}
							components--;
						}
						ImGui::SameLine();
						ImGui::Combo(ss.str().c_str(), &selected[i], str);
						ImGui::SameLine();
						if (ImGui::Button(ss3.str().c_str()))
						{
							switch (selected[i])
							{
								case RegisteredComponents::translation:
								{
									edit_t = !edit_t;
								}break;
							}
						}
					}

					ImGui::Separator();
				}

				if (components < item_count)
				{
					if (ImGui::Button("Add Component"))
					{
						if (components < item_count)
							components++;
						new_component = true;
					}
				}

				ImGui::EndChildFrame();

				if (ImGui::Button("Save"))
				{

				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					new_bp = false;
				}
			}

			if (new_entity)
			{
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

	}
	EditEntity();
#endif
}