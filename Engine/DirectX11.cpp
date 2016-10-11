#include "stdafx.h"
#ifndef SNOWBLIND_VULKAN
#include "DirectX11.h"

#include <sstream>

#define BLACK_CLEAR(v) v[0] = 0.f; v[1] = 0.f; v[2] = 0.f; v[3] = 0.f;

namespace Snowblind
{
	bool DirectX11::Initiate(HWND window_handle, float window_width, float window_height)
	{
		myAPI = "DirectX11";
		myWindowHandle = window_handle;
		myWindowWidth = window_width;
		myWindowHeight = window_height;

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
		for (auto it = myAdapters.begin(); it != myAdapters.end(); ++it)
		{
			 SAFE_RELEASE(it->second);
		}


		mySwapchain->SetFullscreenState(FALSE, nullptr);
		myEngineFlags[static_cast<u16>(eEngineFlags::FULLSCREEN)] = FALSE;

		SAFE_DELETE(myViewport);

		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::LIGHT_MASK)]);

		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::LIGHT_MASK)]);
		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::MASK_TEST)]);
		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::READ_NO_WRITE)]);
		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::Z_DISABLED)]);
		SAFE_RELEASE(myDepthStates[u16(eDepthStencil::Z_ENABLED)]);

		SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::WIREFRAME)]);
		SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::CULL_BACK)]);
		SAFE_RELEASE(myRasterizerStates[u16(eRasterizer::CULL_NONE)]);

		SAFE_RELEASE(myBlendStates[u16(eBlendStates::LIGHT_BLEND)]);
		SAFE_RELEASE(myBlendStates[u16(eBlendStates::BLEND_FALSE)]);
		SAFE_RELEASE(myBlendStates[u16(eBlendStates::PARTICLE_BLEND)]);
		SAFE_RELEASE(myBlendStates[u16(eBlendStates::ALPHA_BLEND)]);
		SAFE_RELEASE(myBlendStates[u16(eBlendStates::NO_BLEND)]);

		SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::POINT_WRAP)]);
		SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::POINT_CLAMP)]);
		SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)]);
		SAFE_RELEASE(mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)]);

		SAFE_RELEASE(myDepthView);
		SAFE_RELEASE(myDepthBuffer);
		SAFE_RELEASE(myRenderTarget);
		SAFE_RELEASE(mySwapchain);


		myContext->ClearState();
		myContext->Flush();
		//Swap the full screen flags correctly if swapping between.
		SAFE_RELEASE(myContext);
		SAFE_RELEASE(myDevice);
		if (myDebug != nullptr)
		{
			std::stringstream ss;
			ss << "\nDebug is released last. Will report as Live Object! 0x" << myDebug << "\nWatch out for false reports. \n====\n";
			OutputDebugString(ss.str().c_str());

			myDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
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

	void DirectX11::SetDebugName(ID3D11DeviceChild* aChild, const std::string& aDebugName)
	{
		if (aChild != nullptr)
		{
			aChild->SetPrivateData(WKPDID_D3DDebugObjectName, u32(aDebugName.size()), aDebugName.c_str());
		}
	}

	void DirectX11::ResetRenderTargetAndDepth()
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
	}

	void DirectX11::SetDepthBufferState(const eDepthStencil& aDepthState)
	{
		switch (aDepthState)
		{
			case eDepthStencil::READ_NO_WRITE:
			{
				myContext->OMSetDepthStencilState(myDepthStates[u16(eDepthStencil::READ_NO_WRITE)], 0);
			}break;
			case eDepthStencil::Z_DISABLED:
			{
				myContext->OMSetDepthStencilState(myDepthStates[u16(eDepthStencil::Z_DISABLED)], 1);
			}break;
			case eDepthStencil::Z_ENABLED:
			{
				myContext->OMSetDepthStencilState(myDepthStates[u16(eDepthStencil::Z_ENABLED)], 1);
			}break;
			case eDepthStencil::MASK_TEST:
			{
				myContext->OMSetDepthStencilState(myDepthStates[u16(eDepthStencil::MASK_TEST)], 0);
			}break;
			case eDepthStencil::LIGHT_MASK:
			{
				myContext->OMSetDepthStencilState(myDepthStates[u16(eDepthStencil::MASK_TEST)], 0);
			}break;
		}
	}

	void DirectX11::CreateDeviceAndSwapchain()
	{
		DXGI_SWAP_CHAIN_DESC scDesc;
		ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		scDesc.BufferCount = 1;
		scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scDesc.BufferDesc.Width = UINT(myWindowWidth);
		scDesc.BufferDesc.Height = UINT(myWindowHeight);
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.OutputWindow = myWindowHandle;
		scDesc.SampleDesc.Count = 1;
		scDesc.SampleDesc.Quality = 0;
		if (myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE)
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
		reader.ForceReadElement("GraphicsAdapter", adapterString);

		IDXGIAdapter* adapterToUse;
		adapterToUse = myAdapters[adapterString];
		myActiveAdapter = adapterString;

		D3D_DRIVER_TYPE type = D3D_DRIVER_TYPE_NULL;
		if (adapterToUse == nullptr)
		{
			myActiveAdapter = "Unknown";
			type = D3D_DRIVER_TYPE_HARDWARE;
		}
		else if (type)
		{
			type = D3D_DRIVER_TYPE_UNKNOWN;
		}

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapterToUse,
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

		if (myDevice == nullptr)
		{
			hr = D3D11CreateDeviceAndSwapChain(
				adapterToUse,
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
		if (hr != S_OK)
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

		depthDesc.Width = UINT(myWindowWidth);
		depthDesc.Height = UINT(myWindowHeight);
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
		myViewport = new D3D11_VIEWPORT();
		//ZeroMemory(&myViewport, sizeof(D3D11_VIEWPORT));
		myViewport->TopLeftX = 0;
		myViewport->TopLeftY = 0;
		myViewport->Width = FLOAT(myWindowWidth);
		myViewport->Height = FLOAT(myWindowHeight);
		myViewport->MinDepth = 0.f;
		myViewport->MaxDepth = 1.f;
		myContext->RSSetViewports(1, myViewport);
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
			s8 dst[128];
			std::wcstombs(dst, temp, 128);
			std::string actualString(dst);
			myAdaptersName.push_back(actualString);
			myAdapters.insert(std::pair<std::string, IDXGIAdapter*>(actualString, enumAdapter[i]));
		}
	}

	void DirectX11::EnableZBuffer()
	{
		myContext->OMSetDepthStencilState(myDepthStates[int(eDepthStencil::Z_ENABLED)], 1);
	}

	void DirectX11::DisableZBuffer()
	{
		myContext->OMSetDepthStencilState(myDepthStates[int(eDepthStencil::Z_DISABLED)], 1);
	}

	void DirectX11::HandleErrors(const HRESULT& aResult, const std::string& anErrorString)
	{
		std::string toError;
		switch (aResult)
		{
			case D3D11_ERROR_FILE_NOT_FOUND:
			{
				toError = (anErrorString + " File not found!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			{
				toError = (anErrorString + " Too many unique state objects!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			{
				toError = (anErrorString + " Too many view objects!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			{
				toError = (anErrorString + " Deferred Context Map Without Initial Discard!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case DXGI_ERROR_INVALID_CALL:
			{
				toError = (anErrorString + " Invalid call!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case DXGI_ERROR_WAS_STILL_DRAWING:
			{
				toError = (anErrorString + " Were still drawing!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case E_FAIL:
			{
				toError = (anErrorString + " Failed!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case E_INVALIDARG:
			{
				toError = (anErrorString + " One or more arguments were invalid!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case E_OUTOFMEMORY:
			{
				toError = (anErrorString + " Out of Memory!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case E_NOTIMPL:
			{
				toError = (anErrorString + " The method call isn't implemented with the passed parameter combination.");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
			case S_FALSE:
			{
				toError = (anErrorString + " Something went wrong. Returned false!");
				DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			}break;
		}
	}

	const std::string& DirectX11::GetAPIName()
	{
		return myAPI;
	}

	void DirectX11::SetViewport(u16 aWidth, u16 aHeight, u8 aDepth)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(aWidth);
		viewport.Height = float(aHeight);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = float(aDepth);
		myContext->RSSetViewports(1, &viewport);
	}

	void DirectX11::ResetViewport()
	{
		myContext->RSSetViewports(1, myViewport);
	}

	void DirectX11::ResetRendertarget()
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
	}

	void DirectX11::CreateEnabledDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::Z_ENABLED)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::Z_ENABLED)], "eDepthStencil::Z_ENABLED");
		HandleErrors(hr, "Failed to setup Enabled Depth!");
	}

	void DirectX11::CreateDisabledDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::Z_DISABLED)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::Z_DISABLED)], "eDepthStencil::Z_DISABLED");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void DirectX11::CreateReadDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = u8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::READ_NO_WRITE)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::READ_NO_WRITE)], "eDepthStencil::READ_NO_WRITE");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void DirectX11::SetRasterizer(const eRasterizer& aRasterizer)
	{
		myContext->RSSetState(myRasterizerStates[u16(aRasterizer)]);
	}

	void DirectX11::SetBlendState(const eBlendStates& blendState)
	{
		float blend[4];
		blend[0] = 0.f;
		blend[1] = 0.f;
		blend[2] = 0.f;
		blend[3] = 0.f;
		myContext->OMSetBlendState(myBlendStates[u16(blendState)], blend, 0xFFFFFFFF);
	}

	void DirectX11::SetSamplerState(const eSamplerStates& samplerState)
	{
		if (samplerState == eSamplerStates::NONE)
		{
			myContext->PSSetSamplers(0, 0, nullptr);
			return;
		}
		myContext->PSSetSamplers(0, 1, &mySamplerStates[u16(samplerState)]);
	}

	void DirectX11::SetVertexShader(ID3D11VertexShader* aVertexShader)
	{
		DL_ASSERT_EXP(aVertexShader != nullptr, "pixelshader was null!");
		myContext->VSSetShader(aVertexShader, nullptr, 0);
	}

	void DirectX11::SetPixelShader(ID3D11PixelShader* aPixelShader)
	{
		if (aPixelShader == nullptr)
			return;
		//DL_ASSERT_EXP(aPixelShader != nullptr, "pixelshader was null.");
		//Check current Pixel Shader, if current == aPixelShader return
		myContext->PSSetShader(aPixelShader, nullptr, 0);
	}

	void DirectX11::SetGeometryShader(ID3D11GeometryShader* aGeometryShader)
	{
		DL_ASSERT_EXP(aGeometryShader != nullptr, "geomteryshader was null.");
		myContext->GSSetShader(aGeometryShader, nullptr, 0);
	}

	void DirectX11::SetHullShader(ID3D11HullShader* aHullShader)
	{
		DL_ASSERT_EXP(aHullShader != nullptr, "hullshader was null.");
		myContext->HSSetShader(aHullShader, nullptr, 0);
	}

	void DirectX11::SetDomainShader(ID3D11DomainShader* aDomainShader)
	{
		DL_ASSERT_EXP(aDomainShader != nullptr, "domainshader was null.");
		myContext->DSSetShader(aDomainShader, nullptr, 0);
	}

	void DirectX11::SetComputeShader(ID3D11ComputeShader* aComputeShader)
	{
		DL_ASSERT_EXP(aComputeShader != nullptr, "computeshader was null.");
		myContext->CSSetShader(aComputeShader, nullptr, 0);
	}

	void DirectX11::OnAltEnter()
	{
		if (this && mySwapchain)
		{
			mySwapchain->SetFullscreenState(FALSE, nullptr);
			if (myEngineFlags[u16(eEngineFlags::FULLSCREEN)] == FALSE)
			{
				mySwapchain->SetFullscreenState(TRUE, nullptr);
				myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = TRUE;
				return;
			}
			myEngineFlags[u16(eEngineFlags::FULLSCREEN)] = FALSE;
		}
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
		desc.CullMode = D3D11_CULL_NONE;
		myDevice->CreateRasterizerState(&desc, &myRasterizerStates[u16(eRasterizer::CULL_NONE)]);
		SetDebugName(myRasterizerStates[u16(eRasterizer::CULL_NONE)], "CULL_NONE Rasterizer");
	}

	void DirectX11::CreateBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::ALPHA_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::ALPHA_BLEND)], "ALPHA_BLEND BlendState");

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // dest blend makes it go bonkers?
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; //no difference between zero & one
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::LIGHT_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::LIGHT_BLEND)], "LIGHT_BLEND BlendState");

		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::PARTICLE_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::PARTICLE_BLEND)], "PARTICLE_BLEND BlendState");


		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::NO_BLEND)]);
		SetDebugName(myBlendStates[u16(eBlendStates::NO_BLEND)], "NO_BLEND BlendState");

		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		myDevice->CreateBlendState(&blendDesc, &myBlendStates[u16(eBlendStates::BLEND_FALSE)]);
		SetDebugName(myBlendStates[u16(eBlendStates::BLEND_FALSE)], "NO_BLEND BlendState");


	}

	void DirectX11::CreateSamplerStates()
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		samplerDesc.MaxAnisotropy = 0;
		samplerDesc.MipLODBias = 0.f;

		myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)]);
		SetDebugName(mySamplerStates[u16(eSamplerStates::LINEAR_CLAMP)], "LINEAR_CLAMP SamplerState");

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_CLAMP)]);
		SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_CLAMP)], "POINT_CLAMP SamplerState");


		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)]);
		SetDebugName(mySamplerStates[u16(eSamplerStates::LINEAR_WRAP)], "LINEAR_WRAP SamplerState");

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		myDevice->CreateSamplerState(&samplerDesc, &mySamplerStates[u16(eSamplerStates::POINT_WRAP)]);
		SetDebugName(mySamplerStates[u16(eSamplerStates::POINT_WRAP)], "POINT_WRAP SamplerState");
	}

	void DirectX11::CreateDepthStencilStates()
	{
		CreateEnabledDepthStencilState();
		CreateDisabledDepthStencilState();
		CreateReadDepthStencilState();

		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.StencilWriteMask = 0xFF;

		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::MASK_TEST)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::MASK_TEST)], "eDepthStencil::MASK_TEST");
		HandleErrors(hr, "Failed to setup depth buffer!");


		ZeroMemory(&stencilDesc, sizeof(stencilDesc));
		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = u8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;

		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[u16(eDepthStencil::LIGHT_MASK)]);
		SetDebugName(myDepthStates[u16(eDepthStencil::LIGHT_MASK)], "eDepthStencil::MASK_TEST");
		HandleErrors(hr, "Failed to setup depth buffer!");


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
			if (displayModeList[i].Width == myWindowWidth && displayModeList[i].Height == myWindowHeight)
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
#endif