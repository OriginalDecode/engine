#pragma once
#include <d3d11.h>
#include "IGraphicsAPI.h"
#include "engine_shared.h"
#include <vector>
#include <unordered_map>

struct IDXGISwapChain;
struct ID3D11Debug;
struct IDXGIAdapter;
struct D3D11_TEXTURE2D_DESC;
struct ID3D11InfoQueue;
namespace graphics
{
	class DirectX11 : public IGraphicsAPI
	{
		friend class DX11Context;
		friend class DX11Device;
	public:
		DirectX11(CreateInfo info);
		~DirectX11();
		
		

		void Initiate() override;
		void ReleasePtr(void* ptr) override;

		void BeginFrame() override;
		void EndFrame() override;

		void Present(uint8 anInterval, uint8 flags);

		Viewport* CreateViewport(uint16 width, uint16 height, float min_depth, float max_depth, uint16 top_left_x, uint16 top_left_y) override;
		void SetDefaultTargets() override;
		void ResetViewport() override;

		void OnAltEnter() override;
		void OnResize() override;

		cl::Color PickColor(Texture* pTexture) override;

		void SaveTextureToDisk(ID3D11Texture2D* texture_resource, const std::string& file_name);
		void SaveTextureToDisk(ID3D11ShaderResourceView* texture_resource, const std::wstring& file_name);

		UINT64 GetFrequency() { return m_Frequency.Frequency; }
		bool Disjointed() { return (bool)m_Frequency.Disjoint; }
	private:

		//INTERNAL USE FOR THE DX11DEVICE AND CONTEXT ONLY!
		static void SetDebugName(void * pResource, std::string debug_name);
		static void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		static DXGI_FORMAT GetFormat(int32 format);
		static DXGI_FORMAT GetFormat(eVertexFormat format);
		static D3D11_USAGE GetUsage(int32 usage);
		static uint32 GetBindFlag(int32 binding);
		static uint32 GetCPUAccessFlag(int32 flags);
		static D3D11_PRIMITIVE_TOPOLOGY GetTopology(eTopology topology);
		static D3D11_MAP GetMapping(eMapping mapping);
		static D3D11_INPUT_CLASSIFICATION GetInputClass(eElementSpecification el);
		//INTERNAL USE FOR THE DX11DEVICE AND CONTEXT ONLY!

		void CreateDeviceAndSwapchain();
		void CreateDepthStencil(int32 slot, const D3D11_DEPTH_STENCIL_DESC& desc, const std::string& debug_name);
		void CreateDepthStencilStates();

		void CreateDepthBuffer();
		void CreateBackBuffer();
		void CreateAdapterList();
		void CreateBlendStates();
		void CreateSamplerStates();
		
		void GetRefreshRate(uint32& aNumerator, uint32& aDenominator);
		
		void CreateRazterizers();
		void CreateRasterizerState(const D3D11_RASTERIZER_DESC& desc, eRasterizer rasterizer, const char* debugname);

		IDXGISwapChain* m_Swapchain = nullptr; 
		ID3D11Debug* m_Debug = nullptr;
		ID3D11InfoQueue* m_InfoQueue = nullptr;

		std::unordered_map<std::string, IDXGIAdapter*> m_Adapters;
		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;

		D3D11_TEXTURE2D_DESC m_PixelPickDesc;


		ID3D11Query* m_FrameQuery = nullptr;

		ID3D11DeviceContext* m_IntContext = nullptr;
		ID3D11Device* m_IntDevice = nullptr;
		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT  m_Frequency;

		
#ifdef _DEBUG
		void ReportLiveObjects();
#endif

	};
};