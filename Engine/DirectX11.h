#pragma once
#include <vector>
#include <unordered_map>
#include <bitset>
#ifndef _WINDEF_
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;
struct HWND__;
typedef HWND__* HWND;
#endif

typedef long HRESULT;

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DeviceChild;
struct ID3D11Debug;
struct ID3D11CommandList;
struct IDXGIAdapter;
struct D3D11_VIEWPORT;
struct ID3D11RasterizerState;
struct ID3D11BlendState;

enum class eEngineFlags
{
	FULLSCREEN,
	_COUNT
};

enum class eDepthStencil
{
	Z_ENABLED,
	Z_DISABLED,
	READ_NO_WRITE,
	_COUNT
};

enum class eRasterizer
{
	WIREFRAME,
	CULL_BACK,
	CULL_NONE,
};

enum class eBlendStates
{
	NO_BLEND,
	ALPHA_BLEND,
	_COUNT
};



namespace Snowblind
{
	class CDirectX11
	{
	public:
		CDirectX11(HWND aWindowHandle, float aWidth, float aHeight);

		~CDirectX11();
		void Present();
		void Clear();
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
		const std::string& GetAdapterName(unsigned short anIndex);
		const std::string& GetActiveAdapterName();
		void EnableZBuffer();
		void DisableZBuffer();
		void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		const char*	GetAPIName();

		//	Width of Viewport
		//	Height of Viewport
		//	Depth of Viewport (0 - 1)
		void SetViewport(int aWidth, int aHeight, int aDepth);
		void ResetViewport();
		void ResetRendertarget();
		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aDebugName);
		void SetDepthBufferState(const eDepthStencil& aDepthState);
		void SetRasterizer(const eRasterizer& aRasterizer);

		ID3D11RenderTargetView* GetBackbuffer();
		const ID3D11RenderTargetView* GetBackbuffer() const;

		ID3D11DepthStencilView* GetDepthView();
		const ID3D11DepthStencilView* GetDepthView() const;

		void SetBlendState(const eBlendStates& blendState);

	private:

		void CreateDeviceAndSwapchain();
		void CreateDepthBuffer();
		void CreateBackBuffer();
		void CreateViewport();
		void CreateDeferredContext();
		void CreateAdapterList();
		void CreateEnabledDepthStencilState();
		void CreateDisabledDepthStencilState();
		void CreateReadDepthStencilState();
		void CreateRazterizers();
		void CreateBlendStates();

		HWND myHWND;

		D3D11_VIEWPORT* myViewport;
		ID3D11Debug* myDebug;
		ID3D11Device* myDevice;
		IDXGISwapChain* mySwapchain;
		ID3D11Texture2D* myDepthBuffer;
		ID3D11DeviceContext* myContext;
		ID3D11DeviceContext* myDeferredContext;
		ID3D11RenderTargetView* myRenderTarget;
		ID3D11DepthStencilView* myDepthView;
		ID3D11DepthStencilState* myDepthStates[static_cast<int>(eDepthStencil::_COUNT)];
		ID3D11CommandList* myCommandList[2];
		ID3D11RasterizerState* myRasterizerStates[3];
		ID3D11BlendState* myBlendStates[static_cast<int>(eBlendStates::_COUNT)];

		std::unordered_map<std::string, IDXGIAdapter*>	myAdapters;
		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;

		float myWidth;
		float myHeight;
		const char*	myAPI;

		std::bitset<int(eEngineFlags::_COUNT)> myEngineFlags;
	};

	__forceinline const std::string& CDirectX11::GetActiveAdapterName()
	{
		return myActiveAdapter;
	}

	__forceinline ID3D11Device* CDirectX11::GetDevice()
	{
		return myDevice;
	}

	__forceinline ID3D11DeviceContext* CDirectX11::GetContext()
	{
		return myContext;
	}

	__forceinline ID3D11RenderTargetView* CDirectX11::GetBackbuffer()
	{
		return myRenderTarget;
	}

	__forceinline const ID3D11RenderTargetView* CDirectX11::GetBackbuffer() const
	{
		return myRenderTarget;
	}

	__forceinline ID3D11DepthStencilView* CDirectX11::GetDepthView()
	{
		return myDepthView;
	}

	__forceinline const ID3D11DepthStencilView* CDirectX11::GetDepthView() const
	{
		return myDepthView;
	}

};