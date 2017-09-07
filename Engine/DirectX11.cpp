#include "stdafx.h"

#include "DirectX11.h"
#include <DDSTextureLoader.h>
#include <ScreenGrab.h>

#if !defined(_PROFILE) && !defined(_FINAL)
#include "imgui_impl_dx11.h"
#endif

#include <sstream>

#define BLACK_CLEAR(v) v[0] = 0.f; v[1] = 0.f; v[2] = 0.f; v[3] = 0.f;

constexpr float clear[4] = { 0.f, 0.f, 0.f, 0.f };

namespace graphics
{
	DirectX11::DirectX11(CreateInfo info)
		: IGraphicsAPI(info)
	{
	}

	bool DirectX11::Initiate(CreateInfo create_info)
	{
		m_CreateInfo = create_info;
		CreateAdapterList();
		CreateDeviceAndSwapchain();
		CreateDepthBuffer();
		CreateDepthStencilStates();
		CreateBackBuffer();
		CreateViewport();
		CreateRazterizers();
		CreateBlendStates();
		CreateSamplerStates();

#if !defined(_PROFILE) && !defined(_FINAL)
		ImGui_ImplDX11_Init(m_CreateInfo.m_HWND, static_cast<ID3D11Device*>(**m_Device), static_cast<ID3D11DeviceContext*>(m_Context));
#endif
		return true;
	}

	bool DirectX11::CleanUp()
	{
#if !defined(_PROFILE) && !defined(_FINAL)
		ImGui_ImplDX11_Shutdown();
#endif
		for (auto it = myAdapters.begin(); it != myAdapters.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
		m_Swapchain->SetFullscreenState(FALSE, nullptr);
		//myEngineFlags[static_cast<u16>(eEngineFlags::FULLSCREEN)] = FALSE;

		float blend[4];
		BLACK_CLEAR(blend);
		m_Context->OMSetBlendState(nullptr, blend, 0xFFFFFFFF);


		SAFE_DELETE(m_Viewport);

		{
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::DEPTH_TEST)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::LIGHT_MASK)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::MASK_TEST)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::READ_NO_WRITE_PARTICLE)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::READ_NO_WRITE)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::Z_DISABLED)]);
			SAFE_RELEASE(myDepthStates[u16(eDepthStencilState::Z_ENABLED)]);
		}

		{
			SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::MSAA)]);
			SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::CULL_FRONT)]);
			SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::CULL_NONE)]);
			SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::CULL_BACK)]);
			SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::WIREFRAME)]);
		}

		{
			SAFE_RELEASE(myBlendStates[u16(eBlendStates::BLEND_FALSE)]);
			SAFE_RELEASE(myBlendStates[u16(eBlendStates::ALPHA_BLEND)]);
			SAFE_RELEASE(myBlendStates[u16(eBlendStates::LIGHT_BLEND)]);
			SAFE_RELEASE(myBlendStates[u16(eBlendStates::NO_BLEND)]);
		}

		{
			SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)]);
			SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)]);
			SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::POINT_CLAMP)]);
			SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::POINT_WRAP)]);
		}

		{
			SAFE_RELEASE(m_DepthView);
			SAFE_RELEASE(m_DepthBuffer);
			SAFE_RELEASE(m_RenderTarget);
			SAFE_RELEASE(m_Swapchain);
		}





		m_Context->ClearState();
		m_Context->Flush();
		//Swap the full screen flags correctly if swapping between.
		SAFE_RELEASE(m_Context);
		ID3D11Device* pDevice = static_cast<ID3D11Device*>(**m_Device);
		SAFE_RELEASE(pDevice);
		if (m_Debug != nullptr)
		{
			std::stringstream ss;
			ss << "\nDebug is released last. Will report as Live Object! 0x" << m_Debug << "\nWatch out for false reports. \n====\n";
			OutputDebugString(ss.str().c_str());

			m_Debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL /*| D3D11_RLDO_IGNORE_INTERNAL*/);
			SAFE_RELEASE(m_Debug);
		}

#if defined (_DEBUG)
		OutputDebugString("\nIntRef is something that D3D has internal. You cannot control these.\n\n");
#endif

		return true;
	}

	void DirectX11::Present(u8 anInterval, u8 flags)
	{
		m_Swapchain->Present(anInterval, flags);
	}

	void DirectX11::Clear()
	{
		float color[4];
		BLACK_CLEAR(color);
		m_Context->ClearRenderTargetView(m_RenderTarget, color);
		m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	const std::string& DirectX11::GetAdapterName(u16 anIndex)
	{
		return myAdaptersName[anIndex];
	}

	void DirectX11::SetDebugName(void * pResource, cl::CHashString<128> debug_name)
	{
		if (!pResource)
			return;

		ID3D11DeviceChild* resource = static_cast<ID3D11DeviceChild*>(pResource);
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, debug_name.length(), debug_name.c_str());
	}

	void DirectX11::SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value)
	{
		m_Context->OMSetDepthStencilState(myDepthStates[u16(depth_stencil_state)], depth_value);
	}

	void DirectX11::ResetRenderTargetAndDepth()
	{
		m_Context->OMSetRenderTargets(1, &m_RenderTarget, m_DepthView);
	}

	void DirectX11::CreateDeviceAndSwapchain()
	{
		DXGI_SWAP_CHAIN_DESC scDesc;
		ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		scDesc.BufferCount = 1;
		scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scDesc.BufferDesc.Width = UINT(m_CreateInfo.m_WindowWidth);
		scDesc.BufferDesc.Height = UINT(m_CreateInfo.m_WindowHeight);
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.OutputWindow = m_CreateInfo.m_HWND;
		scDesc.SampleDesc.Count = 1;
		scDesc.SampleDesc.Quality = 0;
		scDesc.Windowed = true;
		//if (myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE)
		//scDesc.Windowed = false;

		scDesc.Flags = 0;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		bool useVsync = false;

		u32 numerator = 0;
		u32 denominator = 1;

		if (useVsync)
		{
			GetRefreshRate(numerator, denominator);
		}

		scDesc.BufferDesc.RefreshRate.Numerator = numerator;
		scDesc.BufferDesc.RefreshRate.Denominator = denominator;


		const D3D_FEATURE_LEVEL requested_feature_levels[] = {
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG | D3D11_RLDO_DETAIL;
#endif
		UINT featureCount = ARRAYSIZE(requested_feature_levels);

		//Make it work later.

		JSONReader reader("Data/Config/game.json");
		std::string adapterString;
		reader.ReadElement("GraphicsAdapter", adapterString);

		myActiveAdapter = adapterString;

		D3D_DRIVER_TYPE type = D3D_DRIVER_TYPE_NULL;
		if (myAdapters[adapterString] == nullptr)
		{
			myActiveAdapter = "Unknown";
			type = D3D_DRIVER_TYPE_HARDWARE;
		}
		else if (type)
		{
			type = D3D_DRIVER_TYPE_UNKNOWN;
		}

		ID3D11Device* pDevice = nullptr; // static_cast<ID3D11Device*>(**m_Device);

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			myAdapters[adapterString],
			type,
			nullptr,
			createDeviceFlags,
			requested_feature_levels,
			featureCount,
			D3D11_SDK_VERSION,
			&scDesc,
			&m_Swapchain,
			&pDevice,
			nullptr,
			&m_Context);

		if (pDevice == nullptr)
		{
			hr = D3D11CreateDeviceAndSwapChain(
				myAdapters[adapterString],
				type,
				nullptr,
				0,
				requested_feature_levels,
				featureCount,
				D3D11_SDK_VERSION,
				&scDesc,
				&m_Swapchain,
				&pDevice,
				nullptr,
				&m_Context);
		}

		DL_ASSERT_EXP(hr == S_OK, "Failed to Create (Device, Swapchain and Context)!");




#ifdef _DEBUG
		hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_Debug);
		if (hr != S_OK)
		{
			OutputDebugStringA("Failed to Query Debug Interface. myDebug is NULL.");
		}
#endif
		SetDebugName(m_Context, "DirectX11 Context Object");
		const std::string deviceName = "DirectX11 Device Object";
		const std::string swapchainName = "DirectX11 Swapchain Object";
		m_Swapchain->SetPrivateData(WKPDID_D3DDebugObjectName, u32(swapchainName.size()), swapchainName.c_str());
		pDevice->SetPrivateData(WKPDID_D3DDebugObjectName, u32(deviceName.size()), deviceName.c_str());
		m_Device = reinterpret_cast<graphics::IDevice*>(pDevice);

		m_Device = static_cast<graphics::IDevice*>(static_cast<void*>(pDevice));


	}

	void DirectX11::CreateDepthBuffer()
	{
		HRESULT hr;

		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(depthDesc));

		depthDesc.Width = UINT(m_CreateInfo.m_WindowWidth);
		depthDesc.Height = UINT(m_CreateInfo.m_WindowHeight);
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D32_FLOAT;//DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthDesc.SampleDesc.Count = 1; //sample
		depthDesc.SampleDesc.Quality = 0; //quality pattern
		depthDesc.Usage = D3D11_USAGE_DEFAULT;
		depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = static_cast<ID3D11Device*>(**m_Device)->CreateTexture2D(&depthDesc, NULL, &m_DepthBuffer);
		assert(!FAILED(hr) && "Failed to create texture for depthbuffer");

		D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.Format = depthDesc.Format;
		stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		stencilDesc.Texture2D.MipSlice = 0;

		hr = static_cast<ID3D11Device*>(**m_Device)->CreateDepthStencilView(m_DepthBuffer, &stencilDesc, &m_DepthView);
		DL_ASSERT_EXP(hr == S_OK, "Failed to create depth stenci");

#ifdef _DEBUG
		SetDebugName(m_DepthBuffer, "DirectX11 DepthBuffer Object");
		SetDebugName(m_DepthView, "DirectX11 DepthView Object");
#endif
	}

	void DirectX11::CreateBackBuffer()
	{
		HRESULT hr;
		ID3D11Texture2D* backbuffer;
		hr = m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		HandleErrors(hr, "Failed to get Buffer!");

		hr = m_Swapchain->SetFullscreenState(FALSE, nullptr);
		HandleErrors(hr, "Failed to set Fullscreen/Borderless");

		hr = static_cast<ID3D11Device*>(**m_Device)->CreateRenderTargetView(backbuffer, NULL, &m_RenderTarget);
		HandleErrors(hr, "Failed to create RenderTarget.");

		SAFE_RELEASE(backbuffer);
		m_Context->OMSetRenderTargets(1, &m_RenderTarget, m_DepthView);
#ifdef _DEBUG
		SetDebugName(m_RenderTarget, "DirectX11 RenderTarget(Back Buffer) object");
#endif
	}

	void DirectX11::CreateViewport()
	{
		m_Viewport = static_cast<D3D11_VIEWPORT*>(CreateViewport(m_CreateInfo.m_WindowWidth, m_CreateInfo.m_WindowHeight, 0.f, 1.f, 0, 0));
	}

	void* DirectX11::CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y)
	{
		D3D11_VIEWPORT* new_viewport = new D3D11_VIEWPORT;

		new_viewport->TopLeftX = top_left_x;
		new_viewport->TopLeftY = top_left_y;
		new_viewport->Width = FLOAT(width);
		new_viewport->Height = FLOAT(height);
		new_viewport->MinDepth = min_depth;
		new_viewport->MaxDepth = max_depth;

		return new_viewport;
	}

	ID3D11Buffer* DirectX11::CreateConstantBuffer(s32 size)
	{
		return CreateBuffer(size, nullptr, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	}

	ID3D11Buffer* DirectX11::CreateVertexBuffer(s32 size, void* pData)
	{
		return CreateBuffer(size, pData, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
	}

	ID3D11Buffer* DirectX11::CreateIndexBuffer(s32 size, void* pData)
	{
		return CreateBuffer(size, pData, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
	}

	ID3D11Buffer* DirectX11::CreateBuffer(s32 size, void* pData, D3D11_USAGE usage_flag /*= D3D11_USAGE_IMMUTABLE*/, u32 bind_flag /*= D3D11_BIND_VERTEX_BUFFER*/, u32 cpu_access_flag /*= 0*/, u32 misc_flag /*= 0*/, u32 structured_byte_width /*= 0*/)
	{

		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = usage_flag;
		buffer_desc.ByteWidth = size;
		buffer_desc.BindFlags = bind_flag;
		buffer_desc.CPUAccessFlags = cpu_access_flag;
		buffer_desc.MiscFlags = misc_flag;
		buffer_desc.StructureByteStride = structured_byte_width; //investigate future use

		ID3D11Buffer* return_value = nullptr;
		HRESULT hr = S_OK;

		D3D11_SUBRESOURCE_DATA srd;
		if (pData)
		{
			srd.pSysMem = pData;
			hr = static_cast<ID3D11Device*>(**m_Device)->CreateBuffer(&buffer_desc, &srd, &return_value);
		}
		else
		{
			hr = static_cast<ID3D11Device*>(**m_Device)->CreateBuffer(&buffer_desc, nullptr, &return_value);
		}
		HandleErrors(hr, "Failed to create buffer!");

		return return_value;
	}

	ID3D11Buffer* DirectX11::CreateBuffer(D3D11_BUFFER_DESC buffer_desc)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* buffer = nullptr;
		hr = static_cast<ID3D11Device*>(**m_Device)->CreateBuffer(&buffer_desc, nullptr, &buffer);
		HandleErrors(hr, "Failed to create buffer!");
		return buffer;

	}

	void* DirectX11::CreateInputLayout(const void* pShader, s32 shader_byte_size, const D3D11_INPUT_ELEMENT_DESC* pLayout, s32 num_layout_elements)
	{
		ID3D11InputLayout* return_value = nullptr;
		HRESULT hr = static_cast<ID3D11Device*>(**m_Device)->CreateInputLayout(pLayout, num_layout_elements, pShader, shader_byte_size, &return_value);

#ifdef _DEBUG
		HandleErrors(hr, "CreateInputLayout : Failed to create input layout.");
#endif

		return return_value;
	}


	ID3D11SamplerState* DirectX11::GetSampler(s32 index)
	{
		return mySamplerStates[index];
	}


	void DirectX11::CreateAdapterList()
	{
		std::vector<IDXGIAdapter*> enumAdapter;
		IDXGIFactory* factory = nullptr;
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

		IDXGIAdapter* adapter;

		for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			enumAdapter.push_back(adapter);
		}
		SAFE_RELEASE(factory);

		for (u32 i = 0; i < enumAdapter.size(); ++i)
		{
			DXGI_ADAPTER_DESC adapterDesc;
			enumAdapter[i]->GetDesc(&adapterDesc);
			WCHAR* temp = adapterDesc.Description;
			//adapterDesc.DedicatedVideoMemory;
			s8 dst[128];
			std::wcstombs(dst, temp, 128);
			std::string actualString(dst);
			myAdaptersName.push_back(actualString);
			myAdapters.insert(std::pair<std::string, IDXGIAdapter*>(actualString, enumAdapter[i]));
		}
	}

	void DirectX11::EnableZBuffer()
	{
		m_Context->OMSetDepthStencilState(myDepthStates[int(eDepthStencilState::Z_ENABLED)], 1);
	}

	void DirectX11::DisableZBuffer()
	{
		m_Context->OMSetDepthStencilState(myDepthStates[int(eDepthStencilState::Z_DISABLED)], 1);
	}

	void DirectX11::HandleErrors(const HRESULT& aResult, const std::string& anErrorString)
	{
		std::string toError;
		switch (aResult)
		{
			case D3D11_ERROR_FILE_NOT_FOUND:
			{
				toError = (anErrorString + " File not found!");
			}break;
			case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			{
				toError = (anErrorString + " Too many unique state objects!");
			}break;
			case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			{
				toError = (anErrorString + " Too many view objects!");
			}break;
			case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			{
				toError = (anErrorString + " Deferred Context Map Without Initial Discard!");
			}break;
			case DXGI_ERROR_INVALID_CALL:
			{
				toError = (anErrorString + " Invalid call!");
			}break;
			case DXGI_ERROR_WAS_STILL_DRAWING:
			{
				toError = (anErrorString + " Were still drawing!");
			}break;
			case E_FAIL:
			{
				toError = (anErrorString + " Failed!");
			}break;
			case E_INVALIDARG:
			{
				toError = (anErrorString + " One or more arguments were invalid!");
			}break;
			case E_OUTOFMEMORY:
			{
				toError = (anErrorString + " Out of Memory!");
			}break;
			case E_NOTIMPL:
			{
				toError = (anErrorString + " The method call isn't implemented with the passed parameter combination.");
			}break;
			case S_FALSE:
			{
				toError = (anErrorString + " Something went wrong. Returned false!");

			}break;
		}

		if (!toError.empty())
		{
			DL_MESSAGE("%s", toError.c_str());
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
		}
	}

	std::string DirectX11::GetAPIName()
	{
		return m_CreateInfo.m_APIName;
	}

	void DirectX11::ResetViewport()
	{
		SetViewport(m_Viewport);
	}

	void DirectX11::ResetRendertarget()
	{
		m_Context->OMSetRenderTargets(1, &m_RenderTarget, m_DepthView);
	}

	void DirectX11::ClearDepthStencilState()
	{
		m_Context->OMSetDepthStencilState(nullptr, 0);
	}

	void DirectX11::SetRasterizer(const eRasterizer& aRasterizer)
	{
		m_Context->RSSetState(myRasterizerStates[u16(aRasterizer)]);
	}

	void DirectX11::SetBlendState(const eBlendStates& blendState)
	{
		float blend[4] = { 0.f , 0.f , 0.f , 0.f };
		m_Context->OMSetBlendState(myBlendStates[u16(blendState)], blend, 0xFFFFFFFF);
	}

	void DirectX11::SetSamplerState(const eSamplerStates& samplerState)
	{
		m_Context->PSSetSamplers(0, 1, &mySamplerStates[samplerState]);
	}

	void DirectX11::CreateTexture2D(void* pTexDesc, void* pInitialData, void** ppTexture2D)
	{

	}

#ifdef _DEBUG
	void DirectX11::ReportLiveObjects()
	{
		m_Context->ClearState();
		m_Context->Flush();
		m_Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | (D3D11_RLDO_FLAGS)0x4);
	}
#endif

	void DirectX11::SetViewport(void* viewport)
	{
		m_Context->RSSetViewports(1, static_cast<D3D11_VIEWPORT*>(viewport));
	}

	void DirectX11::OnAltEnter()
	{
		if (this && m_Swapchain)
		{
			m_Swapchain->SetFullscreenState(FALSE, nullptr);
			//if (myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE)
			{
				m_Swapchain->SetFullscreenState(TRUE, nullptr);
				//myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = TRUE;
				return;
			}
			//myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = FALSE;
		}
	}

	void DirectX11::OnResize()
	{
		SAFE_RELEASE(m_Swapchain);

		IDXGIFactory* factory;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		HandleErrors(hr, "(OnResize) Failed to create Factory!");

		DXGI_SWAP_CHAIN_DESC scDesc;
		ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		float sym_x = GetSystemMetrics(SM_CXSCREEN);
		float sym_y = GetSystemMetrics(SM_CYSCREEN);

		scDesc.BufferCount = 1;
		scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scDesc.BufferDesc.Width = UINT(sym_x);
		scDesc.BufferDesc.Height = UINT(sym_y);
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.OutputWindow = m_CreateInfo.m_HWND;
		scDesc.SampleDesc.Count = 1;
		scDesc.SampleDesc.Quality = 0;
		scDesc.Windowed = true;
		//scDesc.Windowed = false;



		scDesc.Flags = 0;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		bool useVsync = false;

		u32 numerator = 0;
		u32 denominator = 1;

		if (useVsync)
		{
			GetRefreshRate(numerator, denominator);
		}

		scDesc.BufferDesc.RefreshRate.Numerator = numerator;
		scDesc.BufferDesc.RefreshRate.Denominator = denominator;

		hr = factory->CreateSwapChain(static_cast<IUnknown*>(**m_Device), &scDesc, &m_Swapchain);

		SAFE_RELEASE(factory);
	}

	void DirectX11::CopyResource(void * pDestination, void * pSource)
	{
		m_Context->CopyResource(static_cast<ID3D11Resource*>(pDestination), static_cast<ID3D11Resource*>(pSource));
	}

	// Needs to be tweaked
	void DirectX11::CreateRazterizers()
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = false;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = false;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;


		ID3D11Device* pDevice = static_cast<ID3D11Device*>(**m_Device);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		pDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::WIREFRAME)]);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		pDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_BACK)]);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		pDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_FRONT)]);

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		pDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_NONE)]);


		desc.MultisampleEnable = true;
		desc.AntialiasedLineEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		pDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::MSAA)]);

#ifdef _DEBUG
		SetDebugName(myRasterizerStates[u16(eRasterizer::WIREFRAME)], "Wireframe Rasterizer");
		SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_BACK)], "CULL_BACK Rasterizer");
		SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_FRONT)], "CULL_FRONT Rasterizer");
		SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_NONE)], "CULL_NONE Rasterizer");
		SetDebugName(myRasterizerStates[u16(eRasterizer::MSAA)], "MSAA Rasterizer");
#endif


	}

	void DirectX11::GetRefreshRate(u32& aNumerator, u32& aDenominator)
	{
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes;
		DXGI_MODE_DESC* displayModeList;

		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		result = factory->EnumAdapters(0, &adapter);
		result = adapter->EnumOutputs(0, &adapterOutput);
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		displayModeList = new DXGI_MODE_DESC[numModes];
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

		for (unsigned int i = 0; i < numModes; ++i)
		{
			if (displayModeList[i].Width == m_CreateInfo.m_WindowWidth && displayModeList[i].Height == m_CreateInfo.m_WindowHeight)
			{
				aNumerator = displayModeList[i].RefreshRate.Numerator;
				aDenominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		delete[] displayModeList;
		displayModeList = nullptr;
		SAFE_RELEASE(adapterOutput);
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(factory);
	}
};