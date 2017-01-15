#include "stdafx.h"


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

#include <EntityManager.h>
#include <PhysicsManager.h>

#include "LevelFactory.h"

#include "IGraphicsAPI.h"
#include <d3dcompiler.h>

#include <Input/InputHandle.h>

#define ID3D11DeviceContext_IASetInputLayout(context, input_layout)\
	context->IASetInputLayout(input_layout)

#define ID3D11DeviceContext_IASetVertexBuffers(context, start_slot, num_buffers, ppVertexBuffers, pStrides, pOffsets)\
	context->IASetVertexBuffers(start_slot, num_buffers, ppVertexBuffers, pStrides, pOffsets)

#define ID3D11DeviceContext_IASetIndexBuffer(context, pIndexBuffer, format, offset)\
	context->IASetIndexBuffer(pIndexBuffer,format,offset)

#define ID3D11DeviceContext_IASetPrimitiveTopology(context, format)\
	context->IASetPrimitiveTopology(format);

#define ID3D11DeviceContext_VSSetShader(context, pVSShader, pClassInstance, NumClassInstances)\
	context->VSSetShader(pVSShader, pClassInstance, NumClassInstances)

#define ID3D11DeviceContext_VSSetConstantBuffers(context, StartSlot, NumBuffers, ppConstantBuffer)\
	context->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffer)

#define ID3D11DeviceContext_PSSetShader(context, pPSShader, pClassInstance, NumClassInstances)\
	context->PSSetShader(pPSShader, pClassInstance, NumClassInstances)

#define ID3D11DeviceContext_PSSetSamplers(context, StartSlot, NumSamplers, ppSamplers)\
	context->PSSetSamplers(StartSlot, NumSamplers, ppSamplers)

#define ID3D11DeviceContext_OMSetBlendState(context, blend_state, blend_factor, mask)\
	context->OMSetBlendState(blend_state, blend_factor, mask);

#define ID3D11DeviceContext_RSSetState(context, pRasterizer)\
	context->RSSetState(pRasterizer)

#define ID3D11DeviceContext_RSSetViewports(context, numViewports, pViewports)\
	context->RSSetViewports(numViewports, pViewports)

#define ID3D11DeviceContext_Map(context, pResource, subresource, MapType, MapFlags, pMappedResource)\
	context->Map(pResource,subresource,MapType, MapFlags, pMappedResource)
#define ID3D11DeviceContext_Unmap(context, pResource, subresource)\
	context->Unmap(pResource, subresource)

#define ID3D11DeviceContext_PSSetShaderResources(context, StartSlot, NumViews, ppShaderResourceView)\
	context->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceView)

#define ID3D11DeviceContext_RSSetScissorRects(context, NumRects, pRects)\
	context->RSSetScissorRects(NumRects, pRects)

#define ID3D11DeviceContext_DrawIndexed(context, NumIndexes, StartIndexLocation, BaseVertexLocation)\
	context->DrawIndexed(NumIndexes, StartIndexLocation, BaseVertexLocation)

#define ID3D11Device_CreateRasterizerState(device, desc, pRasterizerState)\
	device->CreateRasterizerState(desc, pRasterizerState)

#define ID3D11Device_CreateVertexShader(device, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)\
	device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)

#define ID3D11Device_CreateInputLayout(device, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLenght, ppInputLayout)\
	device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLenght, ppInputLayout)

#define ID3D11Device_CreateBuffer(device, desc, pInitialData, ppBuffer)\
	device->CreateBuffer(desc, pInitialData, ppBuffer)

#define ID3D11Device_CreatePixelShader(device, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)\
	device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)

#define ID3D11Device_CreateBlendState(device, desc, pBlendState)\
	device->CreateBlendState(desc, pBlendState)

#define ID3D11Device_CreateSamplerState(device, desc, pSamplerState)\
	device->CreateSamplerState(desc, pSamplerState)

#define ID3D11Device_CreateTexture2D(device, pDesc, pInitialData, ppTexture2D)\
	device->CreateTexture2D(pDesc, pInitialData, ppTexture2D)

#define ID3D11Device_CreateShaderResourceView(device, pResource, pDesc, ppSRView)\
	device->CreateShaderResourceView(pResource, pDesc, ppSRView)

#define ID3D11Device_AddRef(device)\
	device->AddRef()

#define ID3D11Texture2D_Release(This)\
	This->Release()
#define ID3D11SamplerState_Release(This)\
	This->Release()
#define ID3D11ShaderResourceView_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11BlendState_Release(This)\
	This->Release()
#define ID3D11PixelShader_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11VertexShader_Release(This)\
	This->Release()
#define ID3D11InputLayout_Release(This)\
	This->Release()
#define ID3D11RasterizerState_Release(This)\
	This->Release()
#define ID3D11Device_Release(This)\
	This->Release()

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D11_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_d3d11.h"

static constexpr char* vertex_shader = "VS";
static constexpr char* pixel_shader = "PS";
static constexpr char* geometry_shader = "GS";
static constexpr char* hull_shader = "HS";
static constexpr char* domain_shader = "DS";
static constexpr char* compute_shader = "CS";

namespace Hex
{

	void Engine::AddEntitySystems()
	{
		m_EntityManager->AddSystem<CPhysicsSystem>();
		m_EntityManager->AddSystem<CRenderSystem>();
		m_EntityManager->AddSystem<CLightSystem>();
		m_EntityManager->AddSystem<InputSystem>();
		m_EntityManager->AddSystem<NetworkSystem>();
		m_EntityManager->AddSystem<AISystem>();
		m_EntityManager->AddSystem<CameraSystem>();
	}

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

	bool Engine::InitiateDebugSystem(InputHandle* input_handle)
	{
		m_DebugSystem.Initiate(input_handle);
		return true;
	}

	bool Engine::Initiate(float window_width, float window_height, HINSTANCE instance_handle, WNDPROC window_proc)
	{
		Randomizer::Create();

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

		myAssetsContainer = new Cache::CAssetsContainer;
		myAssetsContainer->Initiate();

		m_TerrainManager = new Cache::TerrainManager;

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

		myTimeManager = new CU::TimeManager;

		m_PhysicsManager = new PhysicsManager;

		m_EntityManager = new EntityManager;
		m_EntityManager->Initiate();
		AddEntitySystems();

		m_Threadpool.Initiate();

		m_InputHandle = new InputHandle;
		m_InputHandle->Initiate(myHWND, instance_handle);
		m_InputHandle->AddController(0);


		//_____
		// NUKLEAR
		
		static s32 MAX_VERTEX_BUFFER = (512 * 1024);
		static s32 MAX_INDEX_BUFFER = (128 * 1024);
		m_NKContext = nk_d3d11_init(myAPI->GetDevice(), window_width, window_height, MAX_VERTEX_BUFFER, MAX_INDEX_BUFFER);

		nk_d3d11_font_stash_begin(&m_NKAtlas);

		nk_d3d11_font_stash_end();
		m_NKColor = nk_rgb(28, 48, 62);
		// END 
		//_____

		m_IsInitiated = true;
		return true;
	}

	bool Engine::CleanUp()
	{

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
		SAFE_DELETE(myTimeManager);

		Randomizer::Destroy();

		SAFE_DELETE(m_PhysicsManager);
		m_EntityManager->CleanUp();
		SAFE_DELETE(m_EntityManager);

		DL_ASSERT_EXP(myAPI->CleanUp(), "Failed to clean up graphics API. Something was not set to null.");
		SAFE_DELETE(myAPI);


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
		//nk_input_begin(m_NKContext);
		//nk_input_end(m_NKContext);

		if (!HasInitiated())
			return;

		m_DeltaTime = myTimeManager->GetDeltaTime();
		if (!m_IsLoadingLevel)
		{
			myTimeManager->Update();
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
		m_IsLoadingLevel = true;
		LevelFactory level_factory;
		level_factory.Initiate();
		level_factory.CreateLevel(level_filepath);


		m_IsLoadingLevel = false;
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