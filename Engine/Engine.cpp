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

#include "LevelFactory.h"

#include "IGraphicsAPI.h"
#include <d3dcompiler.h>

#include <Input/InputHandle.h>
#include <PostMaster.h>

#include "EditObject.h"
#include <DebugComponent.h>

#include "imgui_impl_dx11.h"

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

namespace Hex
{
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

	bool Engine::InitiateDebugSystem(InputHandle* input_handle)
	{
		m_DebugSystem.Initiate(input_handle);
		return true;
	}

	bool Engine::Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc)
	{
		Randomizer::Create();
		PostMaster::Create();
		myWindowSize.myHeight = window_height;
		myWindowSize.myWidth = window_width;

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

		myAssetsContainer = new CAssetsContainer;
		myAssetsContainer->Initiate();

		m_TerrainManager = new TerrainManager;

		myFontManager = new CFontManager;
		myFontManager->Initiate();

		mySynchronizer = new Synchronizer;
		DL_ASSERT_EXP(mySynchronizer->Initiate(), "Engine : Failed to Initiate Synchronizer!");

		m_DebugSystem.Initiate(m_InputHandle);

		m_DebugSystem.AddDebugMenuItem("Toggle VSync", [&]()
		{
			ToggleVsync();
		});

		myCamera = new Hex::Camera(myWindowSize.myWidth, myWindowSize.myHeight);
		my2DCamera = new Hex::Camera(myWindowSize.myWidth, myWindowSize.myHeight, CU::Vector3f(0, 0, 0.f));
		myRenderer = new Renderer;
		DL_ASSERT_EXP(myRenderer->Initiate(mySynchronizer, myCamera, my2DCamera), "Engine : Failed to initiate Renderer!");


		m_PhysicsManager = new PhysicsManager;





		m_Threadpool.Initiate();

		m_InputHandle = new InputHandle;
		m_InputHandle->Initiate(myHWND, instance_handle);
		m_InputHandle->AddController(0);

		m_EntityManager.Initiate();

		m_EntityManager.AddSystem<::DebugSystem>(); //Since the engine has it's own debug system, I had to do it like this
		m_EntityManager.AddSystem<CPhysicsSystem>();
		m_EntityManager.AddSystem<CRenderSystem>();
		m_EntityManager.AddSystem<CLightSystem>();
		m_EntityManager.AddSystem<InputSystem>();
		m_EntityManager.AddSystem<NetworkSystem>();
		m_EntityManager.AddSystem<AISystem>();
		m_EntityManager.AddSystem<CameraSystem>();


		//ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);

		ImGui_ImplDX11_Init(myHWND, GetAPI()->GetDevice(), GetAPI()->GetContext());

		m_States[(u16)eEngineStates::INITIATED] = TRUE;
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

		SAFE_DELETE(myCamera);
		SAFE_DELETE(myFontManager);


		SAFE_DELETE(m_PhysicsManager);

		DL_ASSERT_EXP(myAPI->CleanUp(), "Failed to clean up graphics API. Something was not set to null.");
		SAFE_DELETE(myAPI);

		PostMaster::Destroy();
		Randomizer::Destroy();

		return true;
	}

	Camera* Engine::GetCamera()
	{
		return myCamera;
	}

	Hex::Camera* Engine::Get2DCamera()
	{
		return my2DCamera;
	}

	void Engine::Update()
	{


		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(300, GetWindowSize().myHeight));

			ImGuiWindowFlags flags = 0;

			flags |= ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_NoResize;
			//flags |= ImGuiWindowFlags_NoCollapse;
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			static bool pOpen = false;
			static bool new_window = false;
			if (ImGui::Begin("Information", &pOpen, flags))
			{
				ImGui::Text("Delta Time : %.3f", GetDeltaTime());


				ImGui::Separator();

				if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
				{
					new_window = !new_window;
				}

				ImGui::Checkbox("Use mouse movement for Camera", &m_CameraUseMouse);

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


					/*if (ImGui::Button("New Entity", ImVec2(100.f, 25.f)))
					{
						int apa;
						apa = 5;
					}
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text("Create a new Entity using a Blueprint.");
						ImGui::EndTooltip();
					}*/
					//ImGui::SameLine();
					static bool new_bp = false;
					if (ImGui::Button("New Blueprint", ImVec2(100.f, 25.f)))
					{
						new_bp = true;
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

						if (edit_t)
						{
							ImGui::Begin("Hello World");

							static float x_value = 0.f;
							static float y_value = 0.f;
							static float z_value = 0.f;

							ImGui::SliderFloat("X", &x_value, 0, 1);
							ImGui::SliderFloat("Y", &y_value, 0, 1);
							ImGui::SliderFloat("Z", &z_value, 0, 1);



							ImGui::End();
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
				}
				ImGui::End();
				ImGui::PopStyleVar();

			}
			EditEntity();
		}

		if (!HasInitiated())
			return;

		m_DeltaTime = myTimeManager.GetDeltaTime();
		if (m_States[(u16)eEngineStates::LOADING] == FALSE)
		{
			myTimeManager.Update();
			myAssetsContainer->Update();
		}
		myRenderer->Render();

		m_Threadpool.Update();
		m_DebugSystem.Update();

	}

	void Engine::Render()
	{
		m_DebugSystem.Render();
	}

	void Engine::UpdateInput()
	{
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
			//float x = 150.f;// ImGui::GetWindowSize().x;
			//float y = 20.f;
			//ImGui::Text(ss.str().c_str()); ImGui::Separator();

			//if (ImGui::Button("Edit Light", ImVec2(x, y)))
			//{
			//	to_edit.SetComponentToEdit(EditObject::LIGHT);
			//}

			//if (ImGui::Button("Edit Graphics", ImVec2(x, y)))
			//{
			//	m_EditRender = !m_EditRender;
			//}

			//if (ImGui::Button("Edit AI", ImVec2(x, y)))
			//{
			//	m_EditLight = !m_EditLight;
			//}

			//if (ImGui::Button("Edit Network", ImVec2(x, y)))
			//{
			//	m_EditLight = !m_EditLight;
			//}

			//if (ImGui::Button("Edit Physics", ImVec2(x, y)))
			//{
			//	m_EditLight = !m_EditLight;
			//}

			//EditGraphicsComponent();


			ImGui::End();
		}
		ImGui::PopStyleVar();
	}

	void Engine::EditGraphicsComponent()
	{
		//if (!m_EditRender)
		//	return;

		//std::stringstream ss;
		//ss << "GraphicsComponent" << m_EntityToEdit;
		//ImGui::SetNextWindowPos(ImVec2(300.f, ImGui::GetWindowSize().y));
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		//if (ImGui::BeginChildFrame(0, ImVec2(300.f, ImGui::GetWindowSize().y / 2)))
		//{
		//	RenderComponent& l = m_EntityManager->GetComponent<RenderComponent>(m_EntityToEdit);

		//	std::string model_path = l.myModelID.c_str();
		//	char buff[512] = { 0 };
		//	if (ImGui::InputText("Model", buff, 512))
		//	{
		//		ImGui::Text(buff);
		//	}
		//	else
		//	{
		//		ImGui::Text(model_path.c_str());
		//	}



		//	//char shader_path[512] = { 0 };
		//	//ImGui::InputText("Shader", shader_path, 512);

		//	ImGui::Separator();

		//	if (ImGui::Button("Done"))
		//	{
		//		m_EditRender = !m_EditRender;
		//	}

		//	ImGui::EndChildFrame();
		//}
		//ImGui::PopStyleVar();
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

	SWindowSize Engine::GetWindowSize() const
	{
		return myWindowSize;
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

	Hex::Texture* Engine::GetTexture(const std::string& aFilePath)
	{
		return myAssetsContainer->GetTexture(aFilePath);
	}

	Hex::Effect* Engine::GetEffect(const std::string& aFilePath)
	{
		return myAssetsContainer->GetEffect(aFilePath);
	}

	Hex::CModel* Engine::GetModel(const std::string& aFilePath)
	{
		return myAssetsContainer->GetModel(aFilePath);
	}

	const std::string& Engine::LoadModel(const std::string& aFilePath, const std::string& effect)
	{
		//m_Threadpool.AddWork(Work([&]() {
		return myAssetsContainer->LoadModel(aFilePath, effect);
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
		if (m_States[(u16)eEngineStates::USE_VSYNC] == TRUE)
			m_States[(u16)eEngineStates::USE_VSYNC] = FALSE;
		else
			m_States[(u16)eEngineStates::USE_VSYNC] = TRUE;
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

	Hex::CTerrain* Engine::CreateTerrain(std::string aFile, CU::Vector3f position, CU::Vector2f aSize)
	{
		CTerrain* newTerrain = m_TerrainManager->GetTerrain(aFile);
		newTerrain->Initiate(aFile, position, aSize);
		myRenderer->AddTerrain(newTerrain);
		return newTerrain;
	}

	void Engine::LoadLevel(const std::string& level_filepath)
	{
		m_States[(u16)eEngineStates::LOADING] = TRUE;
		//m_IsLoadingLevel = true;
		LevelFactory level_factory;
		level_factory.Initiate();
		level_factory.CreateLevel(level_filepath);

		m_States[(u16)eEngineStates::LOADING] = FALSE;
		//m_IsLoadingLevel = false;
	}

	Threadpool& Engine::GetThreadpool()
	{
		return m_Threadpool;
	}

	void Engine::ToggleDebugMenu()
	{
		m_DebugSystem.GetDebugMenuIsActive() ? m_DebugSystem.DeactivateDebugMenu() : m_DebugSystem.ActivateDebugMenu();
	}

	void Engine::AddError(const std::string& error_message)
	{
		m_DebugSystem.AddToErrorList(error_message);
	}

	void Engine::AddDebugText(const std::string& debug_text)
	{
		m_DebugSystem.AddToDebugText(debug_text);
	}

};