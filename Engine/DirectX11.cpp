#include "stdafx.h"
#include "DirectX11.h"

#include <D3D11.h>
#include <DL_Debug.h>
#include "EngineDefines.h"
#include "../CommonLib/JSON/JSONReader.h"

#include <sstream>
#include <stdlib.h>

#include "AssetsContainer.h"

#undef VOID
#define VOID (void**)
namespace Snowblind
{
	CDirectX11::CDirectX11(HWND aWindowHandle, float aWidth, float aHeight)
		: myWidth(aWidth)
		, myHeight(aHeight)
		, myHWND(aWindowHandle)
	{
		myAPI = "DirectX11";

		CAssetsContainer::Create();

		CreateAdapterList();
		CreateDeviceAndSwapchain();
		CreateDepthBuffer();
		CreateDisabledDepthStencilState();
		CreateEnabledDepthStencilState();
		CreateReadDepthStencilState();
		CreateBackBuffer();
		CreateViewport();
		CreateRazterizers();
		//SetRasterizer();
	}

	CDirectX11::~CDirectX11()
	{
		for (auto it = myAdapters.begin(); it != myAdapters.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		CAssetsContainer::Destroy();

		SAFE_DELETE(myViewport);
		SAFE_RELEASE(myDepthStates[static_cast<int>(eDepthStencil::Z_ENABLED)]);
		SAFE_RELEASE(myDepthStates[static_cast<int>(eDepthStencil::Z_DISABLED)]);
		SAFE_RELEASE(myDepthStates[static_cast<int>(eDepthStencil::READ_NO_WRITE)]);
		SAFE_RELEASE(myDepthView);
		SAFE_RELEASE(myDepthBuffer);
		SAFE_RELEASE(myRenderTarget);
		SAFE_RELEASE(mySwapchain);
		SAFE_RELEASE(myDeferredContext);
		SAFE_RELEASE(myRasterizerStates[0]);
		SAFE_RELEASE(myRasterizerStates[1]);

		myContext->ClearState();
		myContext->Flush();

		SAFE_RELEASE(myContext);
		SAFE_RELEASE(myDevice);

		if (myDebug != nullptr)
		{
			std::stringstream ss;
			ss << "Debug is released last. Will report as Live Object! 0x" << myDebug << "\n";
			OutputDebugString(ss.str().c_str());
			myDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			SAFE_RELEASE(myDebug);
		}
	}

	void CDirectX11::Present()
	{
		mySwapchain->Present(0, 0);
	}

	void CDirectX11::Clear()
	{
		float color[4] = { 0.4f, 0.0f, 0.2f, 1.f }; //replace with something else? DX10Math?
		myContext->ClearRenderTargetView(myRenderTarget, color);
		myContext->ClearDepthStencilView(myDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	const std::string& CDirectX11::GetAdapterName(unsigned short anIndex)
	{
		return myAdaptersName[anIndex];
	}

	void CDirectX11::SetDebugName(ID3D11DeviceChild* aChild, const std::string& aDebugName)
	{
		if (aChild != nullptr)
		{
			aChild->SetPrivateData(WKPDID_D3DDebugObjectName, aDebugName.size(), aDebugName.c_str());
		}
	}

	void CDirectX11::SetDepthBufferState(const eDepthStencil& aDepthState)
	{
		switch (aDepthState)
		{
		case eDepthStencil::READ_NO_WRITE:
			myContext->OMSetDepthStencilState(myDepthStates[static_cast<int>(eDepthStencil::READ_NO_WRITE)], 0);
			break;
		case eDepthStencil::Z_DISABLED:
			myContext->OMSetDepthStencilState(myDepthStates[static_cast<int>(eDepthStencil::Z_DISABLED)], 1);
			break;
		case eDepthStencil::Z_ENABLED:
			myContext->OMSetDepthStencilState(myDepthStates[static_cast<int>(eDepthStencil::Z_ENABLED)], 1);
			break;
		}
	}

	ID3D11RenderTargetView* CDirectX11::GetBackbuffer()
	{
		return myRenderTarget;
	}

	ID3D11DepthStencilView* CDirectX11::GetDepthView()
	{
		return myDepthView;
	}

	void CDirectX11::CreateDeviceAndSwapchain()
	{
		DXGI_SWAP_CHAIN_DESC scDesc;
		ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		scDesc.BufferCount = 1;
		scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scDesc.BufferDesc.Width = UINT(myWidth);
		scDesc.BufferDesc.Height = UINT(myHeight);
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.OutputWindow = myHWND;
		scDesc.SampleDesc.Count = 1;
		scDesc.SampleDesc.Quality = 0;
		if (myEngineFlags[int(eEngineFlags::FULLSCREEN)] == FALSE)
			scDesc.Windowed = true;
		else
			scDesc.Windowed = false;

		scDesc.Flags = 0;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		const D3D_FEATURE_LEVEL requested_feature_levels[] = {
			D3D_FEATURE_LEVEL_11_0,
		};
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
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
				nullptr,
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
		mySwapchain->SetPrivateData(WKPDID_D3DDebugObjectName, swapchainName.size(), swapchainName.c_str());
		myDevice->SetPrivateData(WKPDID_D3DDebugObjectName, deviceName.size(), deviceName.c_str());

#ifdef DEFERREDCONTEXT
		CreateDeferredContext();
#endif

	}

	void CDirectX11::CreateDepthBuffer()
	{
		HRESULT hr;

		D3D11_TEXTURE2D_DESC depthDesc;
		ZeroMemory(&depthDesc, sizeof(depthDesc));

		depthDesc.Width = UINT(myWidth);
		depthDesc.Height = UINT(myHeight);
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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
		DL_ASSERT_EXP(hr == S_OK, "Failed to create depth stencil");


		SetDebugName(myDepthBuffer, "DirectX11 DepthBuffer Object");
		SetDebugName(myDepthView, "DirectX11 DepthView Object");
	}

	void CDirectX11::CreateBackBuffer()
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

	void CDirectX11::CreateViewport()
	{
		myViewport = new D3D11_VIEWPORT();
		//ZeroMemory(&myViewport, sizeof(D3D11_VIEWPORT));
		myViewport->TopLeftX = 0;
		myViewport->TopLeftY = 0;
		myViewport->Width = FLOAT(myWidth);
		myViewport->Height = FLOAT(myHeight);
		myViewport->MinDepth = 0.f;
		myViewport->MaxDepth = 1.f;
		myContext->RSSetViewports(1, myViewport);
	}

	void CDirectX11::CreateDeferredContext()
	{
		HRESULT hr = myDevice->CreateDeferredContext(0, &myDeferredContext);
		switch (hr)
		{
		case DXGI_ERROR_DEVICE_REMOVED:
			DL_ASSERT_EXP(hr == S_OK, "Failed to create Deferred Context. Device were removed!");
			break;
		case DXGI_ERROR_INVALID_CALL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to create Deferred Context. Invalid DXGI Call!");
			break;
		case D3D11_CREATE_DEVICE_SINGLETHREADED:
			DL_ASSERT_EXP(hr == S_OK, "Failed to create Deferred Context. Device were created Single Threaded!");
			break;
		case E_INVALIDARG:
			DL_ASSERT_EXP(hr == S_OK, "Failed to create Deferred Context. One or more args were invalid!");
			break;
		case E_OUTOFMEMORY:
			DL_ASSERT_EXP(hr == S_OK, "Failed to create Deferred Context. Out of Memory!");
			break;
		}
	}

	void CDirectX11::CreateAdapterList()
	{
		std::vector<IDXGIAdapter*> enumAdapter;
		IDXGIFactory* factory = nullptr;
		CreateDXGIFactory(__uuidof(IDXGIFactory), VOID &factory);

		IDXGIAdapter* adapter;

		for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			enumAdapter.push_back(adapter);
		}

		factory->Release();

		for (unsigned int i = 0; i < enumAdapter.size(); ++i)
		{
			DXGI_ADAPTER_DESC adapterDesc;
			enumAdapter[i]->GetDesc(&adapterDesc);
			WCHAR* temp = adapterDesc.Description;
			char dst[128];
			std::wcstombs(dst, temp, 128);
			std::string actualString(dst);
			myAdaptersName.push_back(actualString);
			myAdapters.insert(std::pair<std::string, IDXGIAdapter*>(actualString, enumAdapter[i]));
		}
	}

	void CDirectX11::EnableZBuffer()
	{
		myContext->OMSetDepthStencilState(myDepthStates[static_cast<int>(eDepthStencil::Z_ENABLED)], 1);
	}

	void CDirectX11::DisableZBuffer()
	{
		myContext->OMSetDepthStencilState(myDepthStates[static_cast<int>(eDepthStencil::Z_DISABLED)], 1);
	}

	void CDirectX11::HandleErrors(const HRESULT& aResult, const std::string& anErrorString)
	{
		std::string toError;
		switch (aResult)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			toError = (anErrorString + " File not found!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			toError = (anErrorString + " Too many unique state objects!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			toError = (anErrorString + " Too many view objects!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			toError = (anErrorString + " Deferred Context Map Without Initial Discard!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case DXGI_ERROR_INVALID_CALL:
			toError = (anErrorString + " Invalid call!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case DXGI_ERROR_WAS_STILL_DRAWING:
			toError = (anErrorString + " Were still drawing!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case E_FAIL:
			toError = (anErrorString + " Failed!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case E_INVALIDARG:
			toError = (anErrorString + " One or more arguments were invalid!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case E_OUTOFMEMORY:
			toError = (anErrorString + " Out of Memory!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case E_NOTIMPL:
			toError = (anErrorString + " The method call isn't implemented with the passed parameter combination.");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		case S_FALSE:
			toError = (anErrorString + " Something went wrong. Returned false!");
			DL_ASSERT_EXP(aResult == S_OK, toError.c_str());
			break;
		}
	}

	const char* CDirectX11::GetAPIName()
	{
		return myAPI;
	}

	void CDirectX11::SetViewport(int aWidth, int aHeight, int aDepth)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = FLOAT(aWidth);
		viewport.Height = FLOAT(aHeight);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = FLOAT(aDepth);
		myContext->RSSetViewports(1, &viewport);
	}

	void CDirectX11::ResetViewport()
	{
		myContext->RSSetViewports(1, myViewport);
	}

	void CDirectX11::ResetRendertarget()
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
	}

	void CDirectX11::CreateEnabledDepthStencilState()
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

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[static_cast<int>(eDepthStencil::Z_ENABLED)]);
		SetDebugName(myDepthStates[static_cast<int>(eDepthStencil::Z_ENABLED)], "eDepthStencil::Z_ENABLED");
		HandleErrors(hr, "Failed to setup Enabled Depth!");
	}

	void CDirectX11::CreateDisabledDepthStencilState()
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

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[static_cast<int>(eDepthStencil::Z_DISABLED)]);
		SetDebugName(myDepthStates[static_cast<int>(eDepthStencil::Z_DISABLED)], "eDepthStencil::Z_DISABLED");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void CDirectX11::CreateReadDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC  stencilDesc;
		ZeroMemory(&stencilDesc, sizeof(stencilDesc));

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = 0xFFFFFFFF;
		stencilDesc.StencilWriteMask = 0x0;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;

		HRESULT hr = myDevice->CreateDepthStencilState(&stencilDesc, &myDepthStates[static_cast<int>(eDepthStencil::READ_NO_WRITE)]);
		SetDebugName(myDepthStates[static_cast<int>(eDepthStencil::READ_NO_WRITE)], "eDepthStencil::READ_NO_WRITE");
		HandleErrors(hr, "Failed to setup depth buffer!");
	}

	void CDirectX11::SetRasterizer()
	{
		D3D11_RASTERIZER_DESC rDesc;
		rDesc.FillMode = D3D11_FILL_SOLID;
		rDesc.CullMode = D3D11_CULL_BACK;
		rDesc.FrontCounterClockwise = FALSE;

		myDevice->CreateRasterizerState(&rDesc, &myRasterizerStates[0]);
		myContext->RSSetState(myRasterizerStates[0]);
		SetDebugName(myRasterizerStates[0], "Rasterizer State");


	}

	void CDirectX11::SetRasterizer(const eRasterizer& aRasterizer)
	{
		switch (aRasterizer)
		{
		case eRasterizer::WIREFRAME:
			myContext->RSSetState(myRasterizerStates[static_cast<int>(eRasterizer::WIREFRAME)]);
			break;
		case eRasterizer::CULL_NONE:
			myContext->RSSetState(myRasterizerStates[static_cast<int>(eRasterizer::CULL_NONE)]);
			break;
		case eRasterizer::CULL_BACK:
			myContext->RSSetState(myRasterizerStates[static_cast<int>(eRasterizer::CULL_BACK)]);
			break;
		}
	}

	void CDirectX11::CreateRazterizers()
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
		desc.CullMode = D3D11_CULL_BACK;
		myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::WIREFRAME)]);
		SetDebugName(myRasterizerStates[static_cast<int>(eRasterizer::WIREFRAME)], "Wireframe Rasterizer");


		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::CULL_BACK)]);
		SetDebugName(myRasterizerStates[static_cast<int>(eRasterizer::CULL_BACK)], "CULL_BACK Rasterizer");


		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		myDevice->CreateRasterizerState(&desc, &myRasterizerStates[static_cast<int>(eRasterizer::CULL_NONE)]);
		SetDebugName(myRasterizerStates[static_cast<int>(eRasterizer::CULL_NONE)], "CULL_NONE Rasterizer");
	}

};
#undef VOID