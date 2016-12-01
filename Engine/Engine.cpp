#include "stdafx.h"

#include <d3dcompiler.h>

#include "AssetsContainer.h"
#include "IGraphicsAPI.h"
#include "Renderer.h"
#include "Synchronizer.h"
#include "Terrain.h"
#include "TerrainManager.h"


namespace Snowblind
{

	bool Engine::HasInitiated()
	{
		return (this && m_IsInitiated);
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

	bool Engine::InitiateDebugSystem(Synchronizer* synchronizer, InputHandle* input_handle)
	{
		m_DebugSystem.Initiate(synchronizer, input_handle);
		return true;
	}

	bool Engine::Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc)
	{
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

		if(!m_Window.IsWindowActive())
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
	
		myAssetsContainer = new Cache::CAssetsContainer;
		myAssetsContainer->Initiate();
	
		m_TerrainManager = new Cache::TerrainManager;
		
		myFontManager = new CFontManager;
		myFontManager->Initiate();
	
		mySynchronizer = new Synchronizer;
		DL_ASSERT_EXP(mySynchronizer->Initiate(), "Engine : Failed to Initiate Synchronizer!");
	
		m_DebugSystem.AddDebugMenuItem("Toggle VSync", [&]() 
		{
			ToggleVsync();
		});

		myCamera = new Snowblind::Camera(myWindowSize.myWidth, myWindowSize.myHeight);
		my2DCamera = new Snowblind::Camera(myWindowSize.myWidth, myWindowSize.myHeight, CU::Vector3f(0, 0, 0.f));
		myRenderer = new Renderer;
		DL_ASSERT_EXP(myRenderer->Initiate(mySynchronizer, myCamera, my2DCamera), "Engine : Failed to initiate Renderer!");
	
		myTimeManager = new CU::TimeManager;
		Randomizer::Create();
	
	
		m_Threadpool.Initiate();



		m_IsInitiated = true;
		return true;
	}

	bool Engine::CleanUp()
	{
		m_Threadpool.CleanUp();
		SAFE_DELETE(myAssetsContainer);
		SAFE_DELETE(mySynchronizer);

		m_TerrainManager->CleanUp();
		SAFE_DELETE(m_TerrainManager);

		myRenderer->CleanUp();
		SAFE_DELETE(myRenderer);

		SAFE_DELETE(myCamera);
		SAFE_DELETE(myFontManager);
		SAFE_DELETE(myTimeManager);

		Randomizer::Destroy();

		DL_ASSERT_EXP(myAPI->CleanUp(), "Failed to clean up graphics API. Something was not set to null.");
		SAFE_DELETE(myAPI);


		return true;
	}

	Camera* Engine::GetCamera()
	{
		return myCamera;
	}

	Snowblind::Camera* Engine::Get2DCamera()
	{
		return my2DCamera;
	}

	void Engine::Update()
	{
		m_DeltaTime = myTimeManager->GetDeltaTime();
		myAssetsContainer->Update();
		myTimeManager->Update();
		myRenderer->Render();
		m_Threadpool.Update();
		m_DebugSystem.Update();
	}

	void Engine::Render()
	{
		m_DebugSystem.Render();
	}

	void Engine::CompileShaderFromFile(const std::string& file_path, const std::string& shader_type, const std::string& feature_level, s32 shader_flags, IBlob*& out_compiled_shader, IBlob*& out_compile_message)
	{
		std::wstring w_file_path(file_path.begin(), file_path.end());
		HRESULT hr = D3DCompileFromFile(
			w_file_path.c_str(),
			NULL,
			NULL,
			shader_type.c_str(),
			feature_level.c_str(), 
			shader_flags, 
			NULL, 
			&out_compiled_shader, 
			&out_compile_message);

		GetAPI()->HandleErrors(hr, "Failed to compile shader!");
	}


	static constexpr vertex_shader = "VS";
	static constexpr pixel_shader = "PS";
	static constexpr geometry_shader = "GS";
	static constexpr hull_shader = "HS";
	static constexpr domain_shader = "DS";
	static constexpr compute_shader = "CS";


	void* Engine::CreateShader(IBlob* compiled_shader_blob, const std::string& shader_type, const std::string& debug_name)
	{

		IDevice* device = myAPI->GetDevice();
		if (shader_type.find(vertex_shader))
		{
			IVertexShader* shader = nullptr;
			HRESULT hr = device->CreateVertexShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
			GetAPI()->HandleErrors(hr, "Failed to create Vertex Shader!");
			GetAPI()->SetDebugName(shader, debug_name);
			void* to_return = shader;
			return to_return;
		}
		else if (shader_type.find(vertex_shader))
		{
			IPixelShader* shader = nullptr;
			HRESULT hr = device->CreatePixelShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
			GetAPI()->HandleErrors(hr, "Failed to create Pixel Shader!");
			GetAPI()->SetDebugName(shader, debug_name);
			void* to_return = shader;
			return to_return;
		}
		else if (shader_type.find(vertex_shader))
		{
			IGeometryShader* shader = nullptr;
			HRESULT hr = device->CreateGeometryShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
			GetAPI()->HandleErrors(hr, "Failed to create Geometry Shader!");
			GetAPI()->SetDebugName(shader, debug_name);
			void* to_return = shader;
			return to_return;
		}
		else if (shader_type.find(vertex_shader))
		{
			IHullShader* shader = nullptr;
			HRESULT hr = device->CreateHullShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
			GetAPI()->HandleErrors(hr, "Failed to create Hull Shader!");
			GetAPI()->SetDebugName(shader, debug_name);
			void* to_return = shader;
			return to_return;
		}
		else if (shader_type.find(vertex_shader))
		{
			IDomainShader* shader = nullptr;
			HRESULT hr = device->CreateDomainShader(compiled_shader_blob->GetBufferPointer(), compiled_shader_blob->GetBufferSize(), nullptr, &shader);
			GetAPI()->HandleErrors(hr, "Failed to create Domain Shader!");
			GetAPI()->SetDebugName(shader, debug_name);
			void* to_return = shader;
			return to_return;
		}
		else if (shader_type.find(vertex_shader))
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

	void Engine::Present()
	{
		if (myInstance->myUsingVSync)
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
		return myTimeManager->GetFPS();
	}

	float Engine::GetFrameTime()
	{
		return myTimeManager->GetFrameTime();
	}

	std::string Engine::GetAPIName()
	{
		return myAPI->GetAPIName();
	}

	Snowblind::Texture* Engine::GetTexture(const std::string& aFilePath)
	{
		return myAssetsContainer->GetTexture(aFilePath);
	}

	Snowblind::Effect* Engine::GetEffect(const std::string& aFilePath)
	{
		return myAssetsContainer->GetEffect(aFilePath);
	}

	Snowblind::CModel* Engine::GetModel(const std::string& aFilePath)
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
		myUsingVSync = !myUsingVSync;
	}

	void Engine::OnAltEnter()
	{
		if (myAPI)
			myAPI->OnAltEnter();
	}

	void Engine::OnPause()
	{
		myTimeManager->Pause();
	}

	void Engine::OnResume()
	{
		myTimeManager->Start();
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

	Synchronizer* Engine::GetSynchronizer()
	{
		return mySynchronizer;
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

	Snowblind::CTerrain* Engine::CreateTerrain(const std::string& aFile, const CU::Vector3f& position, const CU::Vector2f& aSize)
	{
		CTerrain* newTerrain = m_TerrainManager->GetTerrain(aFile);
		newTerrain->Initiate(aFile, position, aSize);
		myRenderer->AddTerrain(newTerrain);
		return newTerrain;
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