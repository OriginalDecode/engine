#include "DirectX11.h"

#include <assert.h>
#include <D3D11.h>
#include <sstream>
#include <DL_Debug.h>
#include "EngineDefines.h"
#include <vector>
#include <cstdlib>
#include "../CommonLib/JSON/JSONReader.h"
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

		CreateAdapterList();
		CreateDeviceAndSwapchain();
		CreateDepthBuffer();
		CreateBackBuffer();
		CreateViewport();
	}

	CDirectX11::~CDirectX11()
	{
		for (auto it = myAdapters.begin(); it != myAdapters.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}

		SAFE_RELEASE(myDepthState);
		SAFE_RELEASE(myDepthView);
		SAFE_RELEASE(myDepthBuffer);
		SAFE_RELEASE(myRenderTarget);
		SAFE_RELEASE(mySwapchain);
		SAFE_RELEASE(myContext);
		SAFE_RELEASE(myDeferredContext);
		SAFE_RELEASE(myDevice);

		std::stringstream ss;
		ss << "Debug is released last. Will report as Live Object! 0x" << myDebug << "\n";
		OutputDebugString(ss.str().c_str());
		SAFE_RELEASE(myDebug);

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
		if (myWindowFlags[int(eWindowFlags::FULLSCREEN)] == NOT_FULLSCREEN)
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

		JSONReader reader("Data/Config/game.json");
		std::string adapterString;
		reader.ForceReadElement("GraphicsAdapter", adapterString);
		
		IDXGIAdapter* adapterToUse;
		adapterToUse = myAdapters[adapterString];
		DL_ASSERT_EXP(adapterToUse != nullptr, "Error locating graphics adapter, check game.json");
		myActiveAdapter = adapterString;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapterToUse,
			D3D_DRIVER_TYPE_HARDWARE,
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
				D3D_DRIVER_TYPE_HARDWARE,
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
		myDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&myDebug);
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

		myContext->OMSetDepthStencilState(myDepthState, 1);
		SetDebugName(myDepthBuffer, "DirectX11 DepthBuffer Object");
		SetDebugName(myDepthView, "DirectX11 DepthView Object");
	}

	void CDirectX11::CreateBackBuffer()
	{
		HRESULT hr;
		ID3D11Texture2D* backbuffer;
		hr = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);

		switch (hr)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! File not found!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Too many unique state objects!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Too many view objects!");
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Deferred Context Map Without Initial Discard!");
			break;
		case DXGI_ERROR_INVALID_CALL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Invalid Call");
			break;
		case DXGI_ERROR_WAS_STILL_DRAWING:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Were still drawing!");
			break;
		case E_FAIL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Failed!");
			break;
		case E_INVALIDARG:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! One or more arguments were invalid!");
			break;
		case E_OUTOFMEMORY:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! Out of Memory!");
			break;
		case E_NOTIMPL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to get buffer! The method call isn't implemented with the passed parameter combination.");
			break;
		case S_FALSE:
			break;
		}

		hr = mySwapchain->SetFullscreenState(FALSE, nullptr);
		switch (hr)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! File not found!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Too many unique state objects!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Too many view objects!");
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Deferred Context Map Without Initial Discard!");
			break;
		case DXGI_ERROR_INVALID_CALL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Invalid Call");
			break;
		case DXGI_ERROR_WAS_STILL_DRAWING:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Were still drawing!");
			break;
		case E_FAIL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Failed!");
			break;
		case E_INVALIDARG:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! One or more arguments were invalid!");
			break;
		case E_OUTOFMEMORY:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! Out of Memory!");
			break;
		case E_NOTIMPL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to set to fullscreen/borderless. I don't know which one! The method call isn't implemented with the passed parameter combination.");
			break;
		case S_FALSE:
			break;
		}

		hr = myDevice->CreateRenderTargetView(backbuffer, NULL, &myRenderTarget);
		switch (hr)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! File not found!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Too many unique state objects!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Too many view objects!");
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Deferred Context Map Without Initial Discard!");
			break;
		case DXGI_ERROR_INVALID_CALL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Invalid Call");
			break;
		case DXGI_ERROR_WAS_STILL_DRAWING:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Were still drawing!");
			break;
		case E_FAIL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Failed!");
			break;
		case E_INVALIDARG:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! One or more arguments were invalid!");
			break;
		case E_OUTOFMEMORY:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! Out of Memory!");
			break;
		case E_NOTIMPL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create render target view! I don't know which one! The method call isn't implemented with the passed parameter combination.");
			break;
		case S_FALSE:
			break;
		}

		SAFE_RELEASE(backbuffer);
		myContext->OMSetRenderTargets(1, &myRenderTarget, myDepthView);
		SetDebugName(myRenderTarget, "DirectX11 RenderTarget(Back Buffer) object");
	}

	void CDirectX11::CreateViewport()
	{

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = FLOAT(myWidth);
		viewport.Height = FLOAT(myHeight);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;
		myContext->RSSetViewports(1, &viewport);
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
};
#undef VOID