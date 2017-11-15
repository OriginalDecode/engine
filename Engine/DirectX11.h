#pragma once
#include <d3d11.h>
#include "IGraphicsAPI.h"
#include "engine_shared.h"
#include <vector>

struct IDXGISwapChain;
struct ID3D11Debug;
struct IDXGIAdapter;
struct D3D11_TEXTURE2D_DESC;
namespace graphics
{
	class DirectX11 : public IGraphicsAPI
	{
	public:
		DirectX11(CreateInfo info);
		~DirectX11();
		
		//INTERNAL USE FOR THE DX11DEVICE AND CONTEXT ONLY!
		static void SetDebugName(void * pResource, std::string debug_name);
		static void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		static DXGI_FORMAT GetFormat(s32 format);
		static DXGI_FORMAT GetFormat(eVertexFormat format);
		static D3D11_USAGE GetUsage(s32 usage);
		static u32 GetBindFlag(s32 binding);
		static u32 GetCPUAccessFlag(s32 flags);
		static D3D11_PRIMITIVE_TOPOLOGY GetTopology(eTopology topology);
		static D3D11_MAP GetMapping(eMapping mapping);
		static D3D11_INPUT_CLASSIFICATION GetInputClass(eElementSpecification el);
		//INTERNAL USE FOR THE DX11DEVICE AND CONTEXT ONLY!

		void Initiate() override;
		void ReleasePtr(void* ptr) override;

		void BeginFrame() override;
		void EndFrame() override;

		void Present(u8 anInterval, u8 flags);

		Viewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y) override;
		void SetDefaultTargets() override;
		void ResetViewport() override;

		void OnAltEnter() override;
		void OnResize() override;



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
			
		std::unordered_map<std::string, IDXGIAdapter*> m_Adapters;
		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;

#ifdef _DEBUG
		void ReportLiveObjects();
		D3D11_TEXTURE2D_DESC m_PixelPickDesc;
#endif

	};
};