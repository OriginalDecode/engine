#pragma once
#include <d3d11.h>
#include "IGraphicsAPI.h"
#include "engine_shared.h"
#include <vector>
#include <Engine/ShaderFactory.h>
#include <functional>

struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11Debug;
struct IDXGIAdapter;
struct D3D11_VIEWPORT;

namespace graphics
{
	class DirectX11 : public IGraphicsAPI
	{
	public:
		DirectX11(CreateInfo info);
		~DirectX11();
		static void SetDebugName(void * pResource, std::string debug_name);
		static void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		static DXGI_FORMAT GetFormat(s32 format);
		static D3D11_USAGE GetUsage(s32 usage);
		static u32 GetBindFlag(s32 binding);
		static u32 GetCPUAccessFlag(s32 flags);
		static D3D11_PRIMITIVE_TOPOLOGY GetTopology(eTopology topology);
		static D3D11_MAP GetMapping(eMapping mapping);
		static D3D11_INPUT_CLASSIFICATION GetInputClass(eElementSpecification el);
		void ReleasePtr(void* ptr) override;

		void Initiate() override;

		void EndFrame() override;
		void BeginFrame() override;
		//__________________________
		// Virtual Functions

		void Present(u8 anInterval, u8 flags) override;
		void Clear() override;

		void OnAltEnter() override;
		void OnResize() override;


		Viewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y) override;

		void SetDefaultTargets() override;



		ID3D11RenderTargetView* GetBackbuffer() { return m_RenderTarget; }
		const ID3D11RenderTargetView* GetBackbuffer() const { return m_RenderTarget; }
		ID3D11RenderTargetView** GetBackbufferRef() { return &m_RenderTarget; }

		ID3D11DepthStencilView* GetDepthView() { return m_DepthView; }
		const ID3D11DepthStencilView* GetDepthView() const { return m_DepthView; }


 #ifdef _DEBUG
		void ReportLiveObjects();
 #endif

	private:
		void CreateDeviceAndSwapchain();
		void CreateDepthStencilStates();

		void CreateDepthBuffer();
		void CreateBackBuffer();
		void CreateAdapterList();
		void CreateBlendStates();
		void CreateSamplerStates();
		
		void GetRefreshRate(u32& aNumerator, u32& aDenominator);
		
		void CreateRazterizers();
		void CreateRasterizerState(const D3D11_RASTERIZER_DESC& desc, eRasterizer rasterizer, const char* debugname);

		IDXGISwapChain* m_Swapchain = nullptr; 


		ID3D11Debug* m_Debug = nullptr;
		

		//Should be moved to a default buffer object
		ID3D11RenderTargetView* m_RenderTarget = nullptr;
		ID3D11DepthStencilView* m_DepthView = nullptr;
		ID3D11Texture2D* m_DepthBuffer = nullptr;


		//ID3D11BlendState*			myBlendStates[eBlendStates::NOF_BS];


		std::unordered_map<std::string, IDXGIAdapter*> m_Adapters;
		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;

	};
};