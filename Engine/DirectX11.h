#pragma once
#include <vector>
#include <unordered_map>
#include <bitset>
#include "IGraphicsAPI.h"

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

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

struct ID3D11SamplerState;

struct IUnknown;

namespace Snowblind
{
	class CDirectX11 : public CIGraphicsAPI
	{
	public:
		CDirectX11() = default;

		//__________________________
		// Virtual Functions

		bool Initiate(HWND window_handle, float window_width, float window_height) override;
		bool CleanUp() override;

		void Present(u8 anInterval, u8 flags) override;
		void Clear() override;

		void OnAltEnter() override;

		//__________________________
		// DirectX Functions

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
		
		const std::string& GetAdapterName(u16 anIndex);
		const std::string& GetActiveAdapterName();
		
		void EnableZBuffer();
		void DisableZBuffer();
		
		void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		const std::string& GetAPIName();

		void SetViewport(u16 aWidth, u16 aHeight, u8 aDepth);
		void ResetViewport();
		void ResetRendertarget();
		void SetDebugName(ID3D11DeviceChild* aChild, const std::string& aDebugName);

		ID3D11RenderTargetView* GetBackbuffer();
		const ID3D11RenderTargetView* GetBackbuffer() const;

		ID3D11DepthStencilView* GetDepthView();
		const ID3D11DepthStencilView* GetDepthView() const;

		void ResetRenderTargetAndDepth();

		void SetDepthBufferState(const eDepthStencil& aDepthState);
		void SetRasterizer(const eRasterizer& aRasterizer);
		void SetBlendState(const eBlendStates& blendState);
		void SetSamplerState(const eSamplerStates& samplerState);

		void SetVertexShader(ID3D11VertexShader* aVertexShader);
		void SetPixelShader(ID3D11PixelShader* aPixelShader);
		void SetGeometryShader(ID3D11GeometryShader* aGeometryShader);
		void SetHullShader(ID3D11HullShader* aHullShader);
		void SetDomainShader(ID3D11DomainShader* aDomainShader);
		void SetComputeShader(ID3D11ComputeShader* aComputeShader);

	private:

		void CreateDeviceAndSwapchain();
		void CreateDepthBuffer();
		void CreateBackBuffer();
		void CreateViewport();
		void CreateAdapterList();
		void CreateEnabledDepthStencilState();
		void CreateDisabledDepthStencilState();
		void CreateReadDepthStencilState();
		void CreateRazterizers();
		void CreateBlendStates();
		void CreateSamplerStates();
		void CreateDepthStencilStates();

		void GetRefreshRate(u32& aNumerator, u32& aDenominator);


		HWND myHWND;

		D3D11_VIEWPORT* myViewport = nullptr;
		ID3D11Debug* myDebug = nullptr;
		ID3D11Device* myDevice = nullptr;
		IDXGISwapChain* mySwapchain = nullptr;
		ID3D11Texture2D* myDepthBuffer = nullptr;
		ID3D11DeviceContext* myContext = nullptr;
		ID3D11RenderTargetView* myRenderTarget = nullptr;
		ID3D11DepthStencilView* myDepthView = nullptr;

		//______________________
		// GrowingArray / Map?
		ID3D11DepthStencilState* myDepthStates[static_cast<u16>(eDepthStencil::_COUNT)];
		ID3D11RasterizerState* myRasterizerStates[static_cast<u16>(eRasterizer::_COUNT)];
		ID3D11BlendState* myBlendStates[static_cast<u16>(eBlendStates::_COUNT)];
		ID3D11SamplerState* mySamplerStates[static_cast<u16>(eSamplerStates::_COUNT)];
		std::unordered_map<std::string, IDXGIAdapter*> myAdapters;

		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;


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

	template<typename T>
	bool SafeDelete(T* object_to_delete)
	{
		delete object_to_delete;
		object_to_delete = nullptr;

		if (object_to_delete)
			return false;

		return true;
	}

	template<typename T>
	bool SafeRelease(T* object_to_release)
	{
		object_to_release->Release();
		object_to_release = nullptr;

		if (object_to_release)
			return false;

		return true;
	}


};
