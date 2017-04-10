#pragma once
#ifdef SNOWBLIND_DX11
#include "IGraphicsAPI.h"
#include "snowblind_shared.h"
#include <vector>

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

enum eBoundBuffer
{
	eVertex = 0,
	eGeometry = 1,
	ePixel = 2,
	eHull = 4,
	eDomain = 8,
	eCompute = 16
};


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

	void SetVertexShader(void* vertex_shader) override;
	void SetPixelShader(void* pixel_shader) override;
	void SetGeometryShader(void* geometry_shader) override;
	void SetHullShader(void* hull_shader) override;
	void SetDomainShader(void* domain_shader) override;
	void SetComputeShader(void* compute_shader) override;

	//__________________________
	// DirectX Functions

	IDevice* GetDevice() override;
	ID3D11DeviceContext* GetContext();

	const std::string& GetAdapterName(u16 anIndex);
	const std::string& GetActiveAdapterName();

	void EnableZBuffer() override;
	void DisableZBuffer() override;

	void HandleErrors(const HRESULT& aResult, const std::string& anErrorString);
	std::string GetAPIName();

	void ResetViewport();
	void ResetRendertarget();

	void ClearDepthStencilState();


	ID3D11RenderTargetView* GetBackbuffer();
	const ID3D11RenderTargetView* GetBackbuffer() const;
	ID3D11RenderTargetView** GetBackbufferRef() { return &myRenderTarget; }

	ID3D11DepthStencilView* GetDepthView();
	const ID3D11DepthStencilView* GetDepthView() const;

	void ResetRenderTargetAndDepth();

	//void SetDepthBufferState(const eDepthStencil& aDepthState);

	void SetRasterizer(const eRasterizer& aRasterizer);
	void SetBlendState(const eBlendStates& blendState);
	void SetSamplerState(const eSamplerStates& samplerState);


	void ReportLiveObjects();

	void SetViewport(Viewport* viewport);
	Viewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y);

	IBuffer* CreateConstantBuffer(s32 size);
	IBuffer* CreateVertexBuffer(s32 size, void* pData);
	IBuffer* CreateIndexBuffer(s32 size, void* pData);

	template<typename T>
	void UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size);

	template<typename T>
	void UpdateConstantBuffer(IBuffer*& dest, T* src);

	//void BindConstantBuffer(s32 bound_buffer_flag, IBuffer*& constant_buffer);

	IBuffer* CreateBuffer(s32 size, void* pData, D3D11_USAGE usage_flag = D3D11_USAGE_IMMUTABLE, u32 bind_flag = D3D11_BIND_VERTEX_BUFFER, u32 cpu_access_flag = 0, u32 misc_flag = 0, u32 structured_byte_width = 0);

	IInputLayout* CreateInputLayout(const void* pShader, s32 shader_byte_size, const D3D11_INPUT_ELEMENT_DESC* pLayout, s32 num_layout_elements);



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

	Viewport* myViewport = nullptr;
	ID3D11Debug* myDebug = nullptr;
	IDevice* myDevice = nullptr;
	IDXGISwapChain* mySwapchain = nullptr;
	ID3D11Texture2D* myDepthBuffer = nullptr;
	IDevContext* myContext = nullptr;
	IRenderTargetView* myRenderTarget = nullptr;
	IDepthStencilView* myDepthView = nullptr;

	//______________________
	// GrowingArray / Map?
	ID3D11DepthStencilState* myDepthStates[static_cast<u16>(eDepthStencilState::_COUNT)];
	ID3D11RasterizerState* myRasterizerStates[static_cast<u16>(eRasterizer::_COUNT)];
	ID3D11BlendState* myBlendStates[static_cast<u16>(eBlendStates::_COUNT)];
	ID3D11SamplerState* mySamplerStates[static_cast<u16>(eSamplerStates::_COUNT)];
	std::unordered_map<std::string, IDXGIAdapter*> myAdapters;

	std::vector<std::string> myAdaptersName;
	std::string myActiveAdapter;

	s32 m_BoundConstantBuffers;

};

__forceinline const std::string& DirectX11::GetActiveAdapterName()
{
	return myActiveAdapter;
}

__forceinline ID3D11Device* DirectX11::GetDevice()
{
	return myDevice;
}

__forceinline ID3D11DeviceContext* DirectX11::GetContext()
{
	return myContext;
}

__forceinline ID3D11RenderTargetView* DirectX11::GetBackbuffer()
{
	return myRenderTarget;
}

__forceinline const ID3D11RenderTargetView* DirectX11::GetBackbuffer() const
{
	return myRenderTarget;
}

__forceinline ID3D11DepthStencilView* DirectX11::GetDepthView()
{
	return myDepthView;
}

__forceinline const ID3D11DepthStencilView* DirectX11::GetDepthView() const
{
	return myDepthView;
}

template<typename T>
void DirectX11::UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	myContext->Map(dest, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	if (msr.pData)
	{
		T* data = (T*)msr.pData;
		memcpy(data, &src[0], size);
	}
	myContext->Unmap(dest, 0);

}

template<typename T>
void DirectX11::UpdateConstantBuffer(IBuffer*& dest, T* src)
{
	UpdateConstantBuffer(dest, src, sizeof(T));
}


#endif