#pragma once
#include <d3d11.h>
#include "IGraphicsAPI.h"
#include "engine_shared.h"
#include <vector>
#include <Engine/ShaderFactory.h>
#include <functional>
#define SAFE_RELEASE_UNKNOWN(x) \
IUnknown* unknown_pointer = static_cast< IUnknown* >( x ); \
unknown_pointer->Release(); \
unknown_pointer = nullptr; 

typedef long HRESULT;

//A ton of forward declarations. Maybe I should rewrite these things to something else

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

namespace graphics
{
	class DirectX11 : public IGraphicsAPI
	{
	public:
		DirectX11() = default;

		//__________________________
		// Virtual Functions

		bool Initiate(CreateInfo create_info) override;
		bool CleanUp() override;

		void Present(u8 anInterval, u8 flags) override;
		void Clear() override;

		void OnAltEnter() override;
		void OnResize() override;

		void CopyResource(void * pDestination, void * pSource) override;
		void SetDebugName(void * pResource, std::string debug_name) override;

		void SetDepthStencilState(eDepthStencilState depth_stencil_state, s32 depth_value) override;

		void SetVertexShader(CompiledShader* vertex_shader) override;
		void SetPixelShader(CompiledShader* pixel_shader) override;
		void SetGeometryShader(CompiledShader* geometry_shader) override;
		void SetHullShader(CompiledShader* hull_shader) override;
		void SetDomainShader(CompiledShader* domain_shader) override;
		void SetComputeShader(CompiledShader* compute_shader) override;

		void* CreateVertexShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;
		void* CreatePixelShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;
		void* CreateGeometryShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;
		void* CreateHullShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;
		void* CreateDomainShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;
		void* CreateComputeShader(void* pBuffer, float buffer_size, const std::string& debug_name) override;

		void VSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void PSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void GSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void DSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void HSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void CSSetShaderResource(s32 start_slot, s32 count, void* resources) override;





		//__________________________
		// DirectX Functions

		const CreateInfo& GetInfo() const { return m_CreateInfo; }

		IDevice* GetDevice() override { return m_Device; }
		IDevContext* GetContext() { return m_Context; }

		const std::string& GetAdapterName(u16 anIndex);
		const std::string& GetActiveAdapterName();

		void EnableZBuffer() override;
		void DisableZBuffer() override;

		void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
		std::string GetAPIName();

		void ResetViewport();
		void ResetRendertarget();

		void ClearDepthStencilState();


		ID3D11RenderTargetView* GetBackbuffer() { return m_RenderTarget; }
		const ID3D11RenderTargetView* GetBackbuffer() const { return m_RenderTarget; }
		ID3D11RenderTargetView** GetBackbufferRef() { return &m_RenderTarget; }

		ID3D11DepthStencilView* GetDepthView() { return m_DepthView; }
		const ID3D11DepthStencilView* GetDepthView() const { return m_DepthView; }

		void ResetRenderTargetAndDepth();

		//void SetDepthBufferState(const eDepthStencil& aDepthState);

		void SetRasterizer(const eRasterizer& aRasterizer);
		void SetBlendState(const eBlendStates& blendState);
		void SetSamplerState(const eSamplerStates& samplerState);



		void ReportLiveObjects();

		void SetViewport(IViewport* viewport);
		IViewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y);

		IBuffer* CreateConstantBuffer(s32 size);
		IBuffer* CreateVertexBuffer(s32 size, void* pData);
		IBuffer* CreateIndexBuffer(s32 size, void* pData);

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size);

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src);

		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, s32 size, D3D11_MAP map_type);


		IBuffer* CreateBuffer(s32 size, void* pData, D3D11_USAGE usage_flag = D3D11_USAGE_IMMUTABLE, u32 bind_flag = D3D11_BIND_VERTEX_BUFFER, u32 cpu_access_flag = 0, u32 misc_flag = 0, u32 structured_byte_width = 0);
		IBuffer* CreateBuffer(D3D11_BUFFER_DESC buffer_desc);
		IInputLayout* CreateInputLayout(const void* pShader, s32 shader_byte_size, const D3D11_INPUT_ELEMENT_DESC* pLayout, s32 num_layout_elements);

		ID3D11SamplerState* GetSampler(s32 index);

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

		IDXGISwapChain* m_Swapchain = nullptr;

		IDevice* m_Device = nullptr;
		IViewport* m_Viewport = nullptr;
		ITexture2D* m_DepthBuffer = nullptr;
		IDevContext* m_Context = nullptr;
		IRenderTargetView* m_RenderTarget = nullptr;
		IDepthStencilView* m_DepthView = nullptr;

		ID3D11Debug* m_Debug = nullptr; //Can't change this one. DX11 Specific

		//______________________
		// GrowingArray / Map?
		ID3D11DepthStencilState* myDepthStates[eDepthStencilState::NOF_DSS];
		ID3D11RasterizerState* myRasterizerStates[eRasterizer::NOF_RS];
		ID3D11BlendState* myBlendStates[eBlendStates::NOF_BS];
		ID3D11SamplerState* mySamplerStates[eSamplerStates::NOF_SS];
		std::unordered_map<std::string, IDXGIAdapter*> myAdapters;

		std::vector<std::string> myAdaptersName;
		std::string myActiveAdapter;

	};

	__forceinline const std::string& DirectX11::GetActiveAdapterName()
	{
		return myActiveAdapter;
	}

	template<typename T>
	void DirectX11::UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Context->Map(dest, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (msr.pData)
		{
			T* data = (T*)msr.pData;
			memcpy(data, &src[0], size);
		}
		m_Context->Unmap(dest, 0);

	}

	template<typename T>
	void DirectX11::UpdateConstantBuffer(IBuffer*& dest, T* src)
	{
		UpdateConstantBuffer(dest, src, sizeof(T));
	}

	template<typename T>
	void DirectX11::UpdateBuffer(IBuffer*& dest, T* src, s32 size, D3D11_MAP map_type)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Context->Map(dest, 0, map_type, 0, &msr);

		if (msr.pData)
		{
			T* data = (T*)msr.pData;
			memcpy(data, &src[0], size);
		}
		m_Context->Unmap(dest, 0);
	}

};