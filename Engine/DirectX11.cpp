#include "stdafx.h"
#ifdef SNOWBLIND_DX11
#include "DirectX11.h"

#include <sstream>

#define BLACK_CLEAR(v) v[0] = 0.f; v[1] = 0.f; v[2] = 0.f; v[3] = 0.f;
#define PINK_CLEAR(v) v[0] = 1.f; v[1] = 0.07f; v[2] = 255.f; v[3] = 0.f;

constexpr float clear[4] = { 0.f, 0.f, 0.f, 0.f };

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

	return true;
}

bool DirectX11::CleanUp()
{
	for ( auto it = myAdapters.begin(); it != myAdapters.end(); ++it )
	{
		SAFE_RELEASE(it->second);
	}
	mySwapchain->SetFullscreenState(FALSE, nullptr);
	myEngineFlags[static_cast<u16>(eEngineFlags::FULLSCREEN)] = FALSE;

	float blend[4];
	BLACK_CLEAR(blend);
	myContext->OMSetBlendState(nullptr, blend, 0xFFFFFFFF);


	SAFE_DELETE(myViewport);

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
		SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::NONE)]);
	}

	{
		SAFE_RELEASE(myDepthView);
		SAFE_RELEASE(myDepthBuffer);
		SAFE_RELEASE(myRenderTarget);
		SAFE_RELEASE(mySwapchain);
	}

	myContext->ClearState();
	myContext->Flush();
	//Swap the full screen flags correctly if swapping between.
	SAFE_RELEASE(myContext);
	SAFE_RELEASE(myDevice);
	if ( myDebug != nullptr )
	{
		std::stringstream ss;
		ss << "\nDebug is released last. Will report as Live Object! 0x" << myDebug << "\nWatch out for false reports. \n====\n";
		OutputDebugString(ss.str().c_str());

		myDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL /*| D3D11_RLDO_IGNORE_INTERNAL*/);
		SAFE_RELEASE(myDebug);
	}

#if defined (_DEBUG)
	OutputDebugString("\nIntRef is something that D3D has internal. You cannot control these.\n\n");
#endif

	return true;
}

void DirectX11::Present(u8 anInterval, u8 flags)
{
	mySwapchain->Present(anInterval, flags);
}

void DirectX11::Clear()
{
	float color[4];
	BLACK_CLEAR(color);
	myContext->ClearRenderTargetView(myRenderTarget, color);
	myContext->ClearDepthStencilView(myDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

const std::string& DirectX11::GetAdapterName(u16 anIndex)
{
	return myAdaptersName[anIndex];
}

void DirectX11::SetDebugName(void * pResource, std::string debug_name)
{
	if ( pResource )
	{
		ID3D11DeviceChild* resource = static_cast<ID3D11DeviceChild*>(pResource);
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, (u32)debug_name.size(), debug_name.c_str());
	}
}

void DirectX11::SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value)
{
	myContext->OMSetDepthStencilState(myDepthStates[u16(depth_stencil_state)], depth_value);
}

void DirectX11::ResetRenderTargetAndDepth()
{
	myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
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
	if ( myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE )
		scDesc.Windowed = true;
	else
		scDesc.Windowed = false;

	scDesc.Flags = 0;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bool useVsync = false;

	u32 numerator = 0;
	u32 denominator = 1;

	if ( useVsync )
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
	if ( myAdapters[adapterString] == nullptr )
	{
		myActiveAdapter = "Unknown";
		type = D3D_DRIVER_TYPE_HARDWARE;
	}
	else if ( type )
	{
		type = D3D_DRIVER_TYPE_UNKNOWN;
	}

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		myAdapters[adapterString],
		type,
		nullptr,
		createDeviceFlags,
		requested_feature_levels,
		featureCount,
		D3D11_SDK_VERSION,
		&scDesc,
		&mySwapchain,
		&myDevice,
		nullptr,
		&myContext);

	if ( myDevice == nullptr )
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
			&mySwapchain,
			&myDevice,
			nullptr,
			&myContext);
	}

	DL_ASSERT_EXP(hr == S_OK, "Failed to Create (Device, Swapchain and Context)!");

#ifdef _DEBUG
	hr = myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&myDebug);
	if ( hr != S_OK )
	{
		OutputDebugStringA("Failed to Query Debug Interface. myDebug is NULL.");
	}
#endif
	SetDebugName(myContext, "DirectX11 Context Object");
	const std::string deviceName = "DirectX11 Device Object";
	const std::string swapchainName = "DirectX11 Swapchain Object";
	mySwapchain->SetPrivateData(WKPDID_D3DDebugObjectName, u32(swapchainName.size()), swapchainName.c_str());
	myDevice->SetPrivateData(WKPDID_D3DDebugObjectName, u32(deviceName.size()), deviceName.c_str());

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
	hr = myDevice->CreateTexture2D(&depthDesc, NULL, &myDepthBuffer);
	assert(!FAILED(hr) && "Failed to create texture for depthbuffer");

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));
	stencilDesc.Format = depthDesc.Format;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateDepthStencilView(myDepthBuffer, &stencilDesc, &myDepthView);
	DL_ASSERT_EXP(hr == S_OK, "Failed to create depth stenci");


	SetDebugName(myDepthBuffer, "DirectX11 DepthBuffer Object");
	SetDebugName(myDepthView, "DirectX11 DepthView Object");
}

void DirectX11::CreateBackBuffer()
{
	HRESULT hr;
	ID3D11Texture2D* backbuffer;
	hr = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	HandleErrors(hr, "Failed to get Buffer!");

	hr = mySwapchain->SetFullscreenState(FALSE, nullptr);
	HandleErrors(hr, "Failed to set Fullscreen/Borderless");

	hr = myDevice->CreateRenderTargetView(backbuffer, NULL, &myRenderTarget);
	HandleErrors(hr, "Failed to create RenderTarget.");

	SAFE_RELEASE(backbuffer);
	myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
	SetDebugName(myRenderTarget, "DirectX11 RenderTarget(Back Buffer) object");
}

void DirectX11::CreateViewport()
{
	myViewport = CreateViewport(m_CreateInfo.m_WindowWidth, m_CreateInfo.m_WindowHeight, 0.f, 1.f, 0, 0);
}

D3D11_VIEWPORT* DirectX11::CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y)
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

IBuffer* DirectX11::CreateConstantBuffer(s32 size)
{
	return CreateBuffer(size, nullptr, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
}

IBuffer* DirectX11::CreateVertexBuffer(s32 size, void* pData)
{
	return CreateBuffer(size, pData, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0);
}

IBuffer* DirectX11::CreateIndexBuffer(s32 size, void* pData)
{
	return CreateBuffer(size, pData, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
}

IBuffer* DirectX11::CreateBuffer(s32 size, void* pData, D3D11_USAGE usage_flag /*= D3D11_USAGE_IMMUTABLE*/, u32 bind_flag /*= D3D11_BIND_VERTEX_BUFFER*/, u32 cpu_access_flag /*= 0*/, u32 misc_flag /*= 0*/, u32 structured_byte_width /*= 0*/)
{

	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));
	buffer_desc.Usage = usage_flag;
	buffer_desc.ByteWidth = size;
	buffer_desc.BindFlags = bind_flag;
	buffer_desc.CPUAccessFlags = cpu_access_flag;
	buffer_desc.MiscFlags = misc_flag;
	buffer_desc.StructureByteStride = structured_byte_width; //investigate future use

	IBuffer* return_value = nullptr;
	HRESULT hr = S_OK;

	D3D11_SUBRESOURCE_DATA srd;
	if ( pData )
	{
		srd.pSysMem = pData;
		hr = myDevice->CreateBuffer(&buffer_desc, &srd, &return_value);
	}
	else
	{
		hr = myDevice->CreateBuffer(&buffer_desc, nullptr, &return_value);
	}
	HandleErrors(hr, "Failed to create buffer!");

	return return_value;
}

IInputLayout* DirectX11::CreateInputLayout(const void* pShader, s32 shader_byte_size, const D3D11_INPUT_ELEMENT_DESC* pLayout, s32 num_layout_elements)
{
	IInputLayout* return_value = nullptr;
	HRESULT hr = myDevice->CreateInputLayout(pLayout, num_layout_elements, pShader, shader_byte_size, &return_value);

#ifdef _DEBUG
	HandleErrors(hr, "CreateInputLayout : Failed to create input layout.");
#endif

	return return_value;
}

void DirectX11::CreateAdapterList()
{
	std::vector<IDXGIAdapter*> enumAdapter;
	IDXGIFactory* factory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

	IDXGIAdapter* adapter;

	for ( UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		enumAdapter.push_back(adapter);
	}
	SAFE_RELEASE(factory);

	for ( u32 i = 0; i < enumAdapter.size(); ++i )
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
	myContext->OMSetDepthStencilState(myDepthStates[int(eDepthStencilState::Z_ENABLED)], 1);
}

void DirectX11::DisableZBuffer()
{
	myContext->OMSetDepthStencilState(myDepthStates[int(eDepthStencilState::Z_DISABLED)], 1);
}

void DirectX11::HandleErrors(const HRESULT& aResult, const std::string& anErrorString)
{
	std::string toError;
	switch ( aResult )
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

	if ( !toError.empty() )
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
	SetViewport(myViewport);
}

void DirectX11::ResetRendertarget()
{
	myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
}

void DirectX11::ClearDepthStencilState()
{
	myContext->OMSetDepthStencilState(nullptr, 0);
}

void DirectX11::SetRasterizer(const eRasterizer& aRasterizer)
{
	myContext->RSSetState(myRasterizerStates[u16(aRasterizer)]);
}

void DirectX11::SetBlendState(const eBlendStates& blendState)
{
	float blend[4] = { 0.f , 0.f , 0.f , 0.f };
	myContext->OMSetBlendState(myBlendStates[u16(blendState)], blend, 0xFFFFFFFF);
}

void DirectX11::SetSamplerState(const eSamplerStates& samplerState)
{
	if ( samplerState == eSamplerStates::NONE )
	{
		myContext->PSSetSamplers(0, 0, nullptr);
		return;
	}
	myContext->PSSetSamplers(0, 1, &mySamplerStates[u16(samplerState)]);
}

void DirectX11::SetVertexShader(void* vertex_shader)
{
	myContext->VSSetShader(vertex_shader ? static_cast<IVertexShader*>(vertex_shader) : nullptr, nullptr, 0);
}

void DirectX11::SetPixelShader(void* pixel_shader)
{
	myContext->PSSetShader(pixel_shader ? static_cast<IPixelShader*>(pixel_shader) : nullptr, nullptr, 0);
}

void DirectX11::SetGeometryShader(void* geomtery_shader)
{
	myContext->GSSetShader(geomtery_shader ? static_cast<IGeometryShader*>(geomtery_shader) : nullptr, nullptr, 0);
}

void DirectX11::SetHullShader(void* hull_shader)
{
	myContext->HSSetShader(hull_shader ? static_cast<IHullShader*>(hull_shader) : nullptr, nullptr, 0);
}

void DirectX11::SetDomainShader(void* domain_shader)
{
	myContext->DSSetShader(domain_shader ? static_cast<IDomainShader*>(domain_shader) : nullptr, nullptr, 0);
}

void DirectX11::SetComputeShader(void* compute_shader)
{
	myContext->CSSetShader(compute_shader ? static_cast<IComputeShader*>(compute_shader) : nullptr, nullptr, 0);
}

void* DirectX11::CreateVertexShader(void* pBuffer, float buffer_size)
{
	ID3D11VertexShader* vShader = nullptr;// static_cast< ID3D11VertexShader* >( shader );
	HRESULT hr = myDevice->CreateVertexShader(pBuffer, buffer_size, nullptr, &vShader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return vShader;
}

void* DirectX11::CreatePixelShader(void* pBuffer, float buffer_size)
{
	ID3D11PixelShader* shader = nullptr;// static_cast< ID3D11VertexShader* >( shader );
	HRESULT hr = myDevice->CreatePixelShader(pBuffer, buffer_size, nullptr, &shader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return shader;
}

void* DirectX11::CreateGeometryShader(void* pBuffer, float buffer_size)
{
	ID3D11GeometryShader* shader = nullptr;// static_cast< ID3D11VertexShader* >( shader );
	HRESULT hr = myDevice->CreateGeometryShader(pBuffer, buffer_size, nullptr, &shader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return shader;
}

void* DirectX11::CreateHullShader(void* pBuffer, float buffer_size)
{
	ID3D11HullShader* shader = nullptr;// static_cast< ID3D11VertexShader* >( shader );
	HRESULT hr = myDevice->CreateHullShader(pBuffer, buffer_size, nullptr, &shader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return shader;
}

void* DirectX11::CreateDomainShader(void* pBuffer, float buffer_size)
{
	ID3D11DomainShader* shader = nullptr;
	HRESULT hr = myDevice->CreateDomainShader(pBuffer, buffer_size, nullptr, &shader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return shader;
}

void* DirectX11::CreateComputeShader(void* pBuffer, float buffer_size)
{
	ID3D11ComputeShader* shader = nullptr;
	HRESULT hr = myDevice->CreateComputeShader(pBuffer, buffer_size, nullptr, &shader);
	if ( hr != S_OK )
	{
		DL_ASSERT("Failed to create vertex shader!");
	}

	return shader;
}

void* DirectX11::CreateBlendState(s32 render_target_write_mask
	, s32 enable_blend_flags
	, BlendState::BlendOp blend_op, BlendState::BlendFlag src_blend, BlendState::BlendFlag dest_blend
	, BlendState::BlendOp alpha_blend_op, BlendState::BlendFlag src_blend_alpha, BlendState::BlendFlag dest_blend_alpha)
{
	D3D11_BLEND_DESC blend_desc;

	blend_desc.AlphaToCoverageEnable = (enable_blend_flags & BlendState::ALPHA_TO_COVERAGE);
	blend_desc.IndependentBlendEnable = (enable_blend_flags & BlendState::INDEPENDANT_BLEND_ENABLED);;
	blend_desc.RenderTarget[0].BlendEnable = (enable_blend_flags & BlendState::BLEND_ENABLED);

	blend_desc.RenderTarget[0].SrcBlend = static_cast<D3D11_BLEND>(src_blend);
	blend_desc.RenderTarget[0].DestBlend = static_cast<D3D11_BLEND>(dest_blend);
	blend_desc.RenderTarget[0].BlendOp = static_cast<D3D11_BLEND_OP>(blend_op);

	blend_desc.RenderTarget[0].SrcBlendAlpha = static_cast<D3D11_BLEND>(src_blend_alpha);
	blend_desc.RenderTarget[0].DestBlendAlpha = static_cast<D3D11_BLEND>(dest_blend_alpha);
	blend_desc.RenderTarget[0].BlendOpAlpha = static_cast<D3D11_BLEND_OP>(alpha_blend_op);

	blend_desc.RenderTarget[0].RenderTargetWriteMask = render_target_write_mask;

	ID3D11BlendState* blend_state = nullptr;
	myDevice->CreateBlendState(&blend_desc, &blend_state);

	return blend_state;
}

void* DirectX11::CreateSamplerState(SamplerState::FilterMode filter_mode, SamplerState::UVAddressMode address_mode, u32 max_anisotropy, float mip_lod_bias, float min_lod, float max_lod, float border_color[4], SamplerState::ComparisonFunc comparison_function)
{
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(address_mode);
	sampler_desc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(address_mode);
	sampler_desc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(address_mode);
	sampler_desc.Filter = static_cast<D3D11_FILTER>(filter_mode);

	sampler_desc.MaxAnisotropy = max_anisotropy;
	sampler_desc.MipLODBias = mip_lod_bias;
	sampler_desc.MinLOD = min_lod;
	sampler_desc.MaxLOD = max_lod;
	sampler_desc.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(comparison_function);
	//sampler_desc.BorderColor = border_color;


	ID3D11SamplerState* sampler_state;
	myDevice->CreateSamplerState(&sampler_desc, &sampler_state);
	return sampler_state;
}

void DirectX11::SetShaderState(ShaderState& shader_state)
{
	ID3D11BlendState* blend_state = static_cast<ID3D11BlendState*>(shader_state.GetBlendState().GetState());
	myContext->OMSetBlendState(blend_state, clear, 0xFFFFFFFF);

	ID3D11RasterizerState* raster_state = static_cast<ID3D11RasterizerState*>(shader_state.GetRasterizerState().GetState());
	myContext->RSSetState(raster_state);

	ID3D11DepthStencilState* depth_state = static_cast<ID3D11DepthStencilState*>(shader_state.GetDepthstencilState().GetState());
	myContext->OMSetDepthStencilState(depth_state, 0);

	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::VSSetSamplers; }, shader_state, SamplerState::VERTEX_SHADER);
	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::PSSetSamplers; }, shader_state, SamplerState::PIXEL_SHADER);
	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::GSSetSamplers; }, shader_state, SamplerState::GEOMETRY_SHADER);
	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::HSSetSamplers; }, shader_state, SamplerState::HULL_SHADER);
	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::DSSetSamplers; }, shader_state, SamplerState::DOMAIN_SHADER);
	SetSamplerState([&](UINT, UINT, ID3D11SamplerState*const*) { &ID3D11DeviceContext::CSSetSamplers; }, shader_state, SamplerState::COMPUTE_SHADER);

}

void DirectX11::SetSamplerState(std::function<void(UINT, UINT, ID3D11SamplerState*const*)> function, ShaderState& shader_state, s32 shader_type)
{
	CU::GrowingArray<SamplerState>& samplers = shader_state.GetSamplerState(shader_type);
	for (SamplerState& sampler : samplers )
	{
		ID3D11SamplerState* sampler_state = static_cast<ID3D11SamplerState*>(sampler.GetState());
		function(sampler.GetSlot(), 1, &sampler_state);
	}
}

void DirectX11::ReportLiveObjects()
{
	myContext->ClearState();
	myContext->Flush();
	myDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | (D3D11_RLDO_FLAGS)0x4);
}

void DirectX11::SetViewport(Viewport* viewport)
{
	myContext->RSSetViewports(1, viewport);
}

void DirectX11::OnAltEnter()
{
	if ( this && mySwapchain )
	{
		mySwapchain->SetFullscreenState(FALSE, nullptr);
		if ( myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE )
		{
			mySwapchain->SetFullscreenState(TRUE, nullptr);
			myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = TRUE;
			return;
		}
		myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = FALSE;
	}
}

void DirectX11::OnResize()
{
	mySwapchain->Release();
	mySwapchain = nullptr;

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
	if ( myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE )
		scDesc.Windowed = true;
	else
		scDesc.Windowed = false;

	scDesc.Flags = 0;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bool useVsync = false;

	u32 numerator = 0;
	u32 denominator = 1;

	if ( useVsync )
	{
		GetRefreshRate(numerator, denominator);
	}

	scDesc.BufferDesc.RefreshRate.Numerator = numerator;
	scDesc.BufferDesc.RefreshRate.Denominator = denominator;

	hr = factory->CreateSwapChain(myDevice, &scDesc, &mySwapchain);

	SAFE_RELEASE(factory);
}

void DirectX11::CopyResource(void * pDestination, void * pSource)
{
	myContext->CopyResource(static_cast<ID3D11Resource*>(pDestination), static_cast<ID3D11Resource*>(pSource));
}

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


	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::WIREFRAME)]);
	SetDebugName(myRasterizerStates[u16(eRasterizer::WIREFRAME)], "Wireframe Rasterizer");


	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_BACK)]);
	SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_BACK)], "CULL_BACK Rasterizer");

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_FRONT)]);
	SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_FRONT)], "CULL_FRONT Rasterizer");

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_NONE)]);
	SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_NONE)], "CULL_NONE Rasterizer");


	desc.MultisampleEnable = true;
	desc.AntialiasedLineEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::MSAA)]);
	SetDebugName(myRasterizerStates[u16(eRasterizer::MSAA)], "MSAA Rasterizer");



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

	for ( unsigned int i = 0; i < numModes; ++i )
	{
		if ( displayModeList[i].Width == m_CreateInfo.m_WindowWidth && displayModeList[i].Height == m_CreateInfo.m_WindowHeight )
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

#endif