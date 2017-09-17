#pragma once
#include <Engine/engine_shared.h>
#include "IGraphicsContext.h"

struct ID3D11DeviceContext;
class CompiledShader;
namespace graphics
{
	class DX11Context : public IGraphicsContext
	{
	public:
		DX11Context(ID3D11DeviceContext* context);
		void VSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void PSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void GSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void DSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void HSSetShaderResource(s32 start_slot, s32 count, void* resources) override;
		void CSSetShaderResource(s32 start_slot, s32 count, void* resources) override;

		void SetVertexShader(CompiledShader* shader) override;
		void SetPixelShader(CompiledShader* shader) override;
		void SetGeometryShader(CompiledShader* shader) override;
		void SetHullShader(CompiledShader* shader) override;
		void SetDomainShader(CompiledShader* shader) override;
		void SetComputeShader(CompiledShader* shader) override;

		void VSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;
		void PSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;
		void GSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;
		void HSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;
		void DSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;
		void CSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) override;

		void VSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void PSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;


		void IASetInputLayout(IInputLayout* input_layout) override;
		void IASetTopology(eTopology topology) override;

		void OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) override;
		void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) override;

		void ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth) override;

		void Draw(s32 vertex_start, s32 vertex_count) override;
		void Draw(s32 index_start, s32 index_count, s32 vertex_start) override;
		void Draw(s32 index_count, s32 instance_count, s32 index_start, s32 vertex_start, s32 instance_start) override;

		void Draw(BaseModel* model) override;
		void DrawIndexed(BaseModel* model) override;
		void DrawInstanced(BaseModel* model) override;
		void DrawIndexedInstanced(BaseModel* model) override;

		void DrawIndexed(Quad* quad, bool depth_on) override;

		template<typename T>
		void UpdateConstantBuffer(IBuffer* dest, T* src, s32 size) override;

		template<typename T>
		void UpdateBuffer(IBuffer* dest, T* src, s32 size, eMapping mapping) override;

		void SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth) override;



	private:
		ID3D11DeviceContext* m_Context = nullptr;





	};

	template<typename T>
	void DX11Context::UpdateBuffer(IBuffer* dest, T* src, s32 size, eMapping mapping)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		m_Context->Map(dest, 0, DirectX11::GetMapping(mapping), 0, &msr);

		if (msr.pData)
		{
			T* data = (T*)msr.pData;
			memcpy(data, &src[0], size);
		}
		m_Context->Unmap(dest, 0);
	}

	template<typename T>
	void DX11Context::UpdateConstantBuffer(IBuffer* dest, T* src, s32 size)
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

};