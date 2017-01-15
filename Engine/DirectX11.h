#pragma once
#ifdef SNOWBLIND_DX11
#include "IGraphicsAPI.h"
#include "snowblind_shared.h"
#include <vector>

/*
ID3D11DeviceContext_QueryInterface
ID3D11DeviceContext_AddRef
ID3D11DeviceContext_Release
ID3D11DeviceContext_GetDevice
ID3D11DeviceContext_GetPrivateData
ID3D11DeviceContext_SetPrivateData
ID3D11DeviceContext_SetPrivateDataInterface
ID3D11DeviceContext_VSSetConstantBuffers
ID3D11DeviceContext_PSSetShaderResources
ID3D11DeviceContext_PSSetShader
ID3D11DeviceContext_PSSetSamplers
ID3D11DeviceContext_VSSetShader
ID3D11DeviceContext_DrawIndexed
ID3D11DeviceContext_Draw
ID3D11DeviceContext_Map
ID3D11DeviceContext_Unmap
ID3D11DeviceContext_PSSetConstantBuffers
ID3D11DeviceContext_IASetInputLayout
ID3D11DeviceContext_IASetVertexBuffers
ID3D11DeviceContext_IASetIndexBuffer
ID3D11DeviceContext_DrawIndexedInstanced
ID3D11DeviceContext_DrawInstanced
ID3D11DeviceContext_GSSetConstantBuffers
ID3D11DeviceContext_GSSetShader
ID3D11DeviceContext_IASetPrimitiveTopology
ID3D11DeviceContext_VSSetShaderResources
ID3D11DeviceContext_VSSetSamplers
ID3D11DeviceContext_Begin
ID3D11DeviceContext_End
ID3D11DeviceContext_GetData
ID3D11DeviceContext_SetPredication
ID3D11DeviceContext_GSSetShaderResources
ID3D11DeviceContext_GSSetSamplers
ID3D11DeviceContext_OMSetRenderTargets
ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews
ID3D11DeviceContext_OMSetBlendState
ID3D11DeviceContext_OMSetDepthStencilState
ID3D11DeviceContext_SOSetTargets
ID3D11DeviceContext_DrawAuto
ID3D11DeviceContext_DrawIndexedInstancedIndirect
ID3D11DeviceContext_DrawInstancedIndirect
ID3D11DeviceContext_Dispatch
ID3D11DeviceContext_DispatchIndirect
ID3D11DeviceContext_RSSetState
ID3D11DeviceContext_RSSetViewports
ID3D11DeviceContext_RSSetScissorRects
ID3D11DeviceContext_CopySubresourceRegion
ID3D11DeviceContext_CopyResource
ID3D11DeviceContext_UpdateSubresource
ID3D11DeviceContext_CopyStructureCount
ID3D11DeviceContext_ClearRenderTargetView
ID3D11DeviceContext_ClearUnorderedAccessViewUint
ID3D11DeviceContext_ClearUnorderedAccessViewFloat
ID3D11DeviceContext_ClearDepthStencilView
ID3D11DeviceContext_GenerateMips
ID3D11DeviceContext_SetResourceMinLOD
ID3D11DeviceContext_GetResourceMinLOD
ID3D11DeviceContext_ResolveSubresource
ID3D11DeviceContext_ExecuteCommandList
ID3D11DeviceContext_FinishCommandList
ID3D11DeviceContext_HSSetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_HSSetShader(This,pHullShader,ppClassInstances,NumClassInstances)	\
ID3D11DeviceContext_HSSetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_HSSetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_DSSetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_DSSetShader(This,pDomainShader,ppClassInstances,NumClassInstances)	\
ID3D11DeviceContext_DSSetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_DSSetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_CSSetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_CSSetUnorderedAccessViews(This,StartSlot,NumUAVs,ppUnorderedAccessViews,pUAVInitialCounts)	\
ID3D11DeviceContext_CSSetShader(This,pComputeShader,ppClassInstances,NumClassInstances)	\
ID3D11DeviceContext_CSSetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_CSSetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_VSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_PSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_PSGetShader(This,ppPixelShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_PSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_VSGetShader(This,ppVertexShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_PSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_IAGetInputLayout(This,ppInputLayout)	\
ID3D11DeviceContext_IAGetVertexBuffers(This,StartSlot,NumBuffers,ppVertexBuffers,pStrides,pOffsets)	\
ID3D11DeviceContext_IAGetIndexBuffer(This,pIndexBuffer,Format,Offset)	\
ID3D11DeviceContext_GSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_GSGetShader(This,ppGeometryShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_IAGetPrimitiveTopology(This,pTopology)	\
ID3D11DeviceContext_VSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_VSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_GetPredication(This,ppPredicate,pPredicateValue)	\
ID3D11DeviceContext_GSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_GSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_OMGetRenderTargets(This,NumViews,ppRenderTargetViews,ppDepthStencilView)	\
ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews(This,NumRTVs,ppRenderTargetViews,ppDepthStencilView,UAVStartSlot,NumUAVs,ppUnorderedAccessViews)	\
ID3D11DeviceContext_OMGetBlendState(This,ppBlendState,BlendFactor,pSampleMask)	\
ID3D11DeviceContext_OMGetDepthStencilState(This,ppDepthStencilState,pStencilRef)	\
ID3D11DeviceContext_SOGetTargets(This,NumBuffers,ppSOTargets)	\
ID3D11DeviceContext_RSGetState(This,ppRasterizerState)	\
ID3D11DeviceContext_RSGetViewports(This,pNumViewports,pViewports)	\
ID3D11DeviceContext_RSGetScissorRects(This,pNumRects,pRects)	\
ID3D11DeviceContext_HSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_HSGetShader(This,ppHullShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_HSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_HSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_DSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_DSGetShader(This,ppDomainShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_DSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_DSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_CSGetShaderResources(This,StartSlot,NumViews,ppShaderResourceViews)	\
ID3D11DeviceContext_CSGetUnorderedAccessViews(This,StartSlot,NumUAVs,ppUnorderedAccessViews)	\
ID3D11DeviceContext_CSGetShader(This,ppComputeShader,ppClassInstances,pNumClassInstances)	\
ID3D11DeviceContext_CSGetSamplers(This,StartSlot,NumSamplers,ppSamplers)	\
ID3D11DeviceContext_CSGetConstantBuffers(This,StartSlot,NumBuffers,ppConstantBuffers)	\
ID3D11DeviceContext_ClearState(This)	\
ID3D11DeviceContext_Flush(This)	\
ID3D11DeviceContext_GetType(This)	\
ID3D11DeviceContext_GetContextFlags(This)	\
*/

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