#pragma once
#ifdef SNOWBLIND_DX11
#include "IGraphicsAPI.h"
#include "snowblind_shared.h"
#include <vector>

#if defined (_cplusplus)

#define ID3D11DeviceContext_IASetInputLayout(context, input_layout)\
	context->IASetInputLayout(input_layout)

#define ID3D11DeviceContext_IASetVertexBuffers(context, start_slot, num_buffers, ppVertexBuffers, pStrides, pOffsets)\
	context->IASetVertexBuffers(start_slot, num_buffers, ppVertexBuffers, pStrides, pOffsets)

#define ID3D11DeviceContext_IASetIndexBuffer(context, pIndexBuffer, format, offset)\
	context->IASetIndexBuffer(pIndexBuffer,format,offset)

#define ID3D11DeviceContext_IASetPrimitiveTopology(context, format)\
	context->IASetPrimitiveTopology(format);

#define ID3D11DeviceContext_VSSetShader(context, pVSShader, pClassInstance, NumClassInstances)\
	context->VSSetShader(pVSShader, pClassInstance, NumClassInstances)

#define ID3D11DeviceContext_VSSetConstantBuffers(context, StartSlot, NumBuffers, ppConstantBuffer)\
	context->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffer)

#define ID3D11DeviceContext_PSSetShader(context, pPSShader, pClassInstance, NumClassInstances)\
	context->PSSetShader(pPSShader, pClassInstance, NumClassInstances)

#define ID3D11DeviceContext_PSSetSamplers(context, StartSlot, NumSamplers, ppSamplers)\
	context->PSSetSamplers(StartSlot, NumSamplers, ppSamplers)

#define ID3D11DeviceContext_OMSetBlendState(context, blend_state, blend_factor, mask)\
	context->OMSetBlendState(blend_state, blend_factor, mask);

#define ID3D11DeviceContext_RSSetState(context, pRasterizer)\
	context->RSSetState(pRasterizer)

#define ID3D11DeviceContext_RSSetViewports(context, numViewports, pViewports)\
	context->RSSetViewports(numViewports, pViewports)

#define ID3D11DeviceContext_Map(context, pResource, subresource, MapType, MapFlags, pMappedResource)\
	context->Map(pResource,subresource,MapType, MapFlags, pMappedResource)
#define ID3D11DeviceContext_Unmap(context, pResource, subresource)\
	context->Unmap(pResource, subresource)

#define ID3D11DeviceContext_PSSetShaderResources(context, StartSlot, NumViews, ppShaderResourceView)\
	context->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceView)

#define ID3D11DeviceContext_RSSetScissorRects(context, NumRects, pRects)\
	context->RSSetScissorRects(NumRects, pRects)

#define ID3D11DeviceContext_DrawIndexed(context, NumIndexes, StartIndexLocation, BaseVertexLocation)\
	context->DrawIndexed(NumIndexes, StartIndexLocation, BaseVertexLocation)

#define ID3D11Device_CreateRasterizerState(device, desc, pRasterizerState)\
	device->CreateRasterizerState(desc, pRasterizerState)

#define ID3D11Device_CreateVertexShader(device, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)\
	device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)

#define ID3D11Device_CreateInputLayout(device, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLenght, ppInputLayout)\
	device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLenght, ppInputLayout)

#define ID3D11Device_CreateBuffer(device, desc, pInitialData, ppBuffer)\
	device->CreateBuffer(desc, pInitialData, ppBuffer)

#define ID3D11Device_CreatePixelShader(device, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)\
	device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)

#define ID3D11Device_CreateBlendState(device, desc, pBlendState)\
	device->CreateBlendState(desc, pBlendState)

#define ID3D11Device_CreateSamplerState(device, desc, pSamplerState)\
	device->CreateSamplerState(desc, pSamplerState)

#define ID3D11Device_CreateTexture2D(device, pDesc, pInitialData, ppTexture2D)\
	device->CreateTexture2D(pDesc, pInitialData, ppTexture2D)

#define ID3D11Device_CreateShaderResourceView(device, pResource, pDesc, ppSRView)\
	device->CreateShaderResourceView(pResource, pDesc, ppSRView)

#define ID3D11Device_AddRef(device)\
	device->AddRef()

#define ID3D11Texture2D_Release(This)\
	This->Release()
#define ID3D11SamplerState_Release(This)\
	This->Release()
#define ID3D11ShaderResourceView_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11BlendState_Release(This)\
	This->Release()
#define ID3D11PixelShader_Release(This)\
	This->Release()
#define ID3D11Buffer_Release(This)\
	This->Release()
#define ID3D11VertexShader_Release(This)\
	This->Release()
#define ID3D11InputLayout_Release(This)\
	This->Release()
#define ID3D11RasterizerState_Release(This)\
	This->Release()
#define ID3D11Device_Release(This)\
	This->Release()

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

namespace Hex
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

		void SetVertexShader(void* vertex_shader);
		void SetPixelShader(void* pixel_shader);
		void SetGeometryShader(void* geometry_shader);
		void SetHullShader(void* hull_shader);
		void SetDomainShader(void* domain_shader);
		void SetComputeShader(void* compute_shader);
		void ReportLiveObjects();

		void SetViewport(Viewport* viewport);
		Viewport* CreateViewport(u16 width, u16 height, float min_depth, float max_depth, u16 top_left_x, u16 top_left_y);

		void CreateConstantBuffer(IBuffer*& buffer, s32 size);

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size);

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src);

		void ClearConstantBuffers();
		void BindConstantBuffer(s32 bound_buffer_flag, IBuffer*& constant_buffer);

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
		ID3D11DepthStencilState* myDepthStates[static_cast<u16>(eDepthStencil::_COUNT)];
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

};
#endif