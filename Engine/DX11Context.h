#pragma once
#include <Engine/engine_shared.h>
#include "IGraphicsContext.h"

struct ID3D11DeviceContext;
class CompiledShader;
namespace graphics
{
	class DX11Context : public IGraphicsContext
	{
		friend class DirectX11;
		friend class DX11Device;
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
		void PSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) override;
		void GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;
		void CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) override;


		void IASetInputLayout(IInputLayout* input_layout) override;
		void IASetTopology(eTopology topology) override;

		void OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) override;
		void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) override;

		void ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth) override;

// 		void Draw(s32 vertex_start, s32 vertex_count) override;
// 		void Draw(s32 index_start, s32 index_count, s32 vertex_start) override;
// 		void Draw(s32 index_count, s32 instance_count, s32 index_start, s32 vertex_start, s32 instance_start) override;

		void Draw(BaseModel* model) override;
		void DrawIndexed(BaseModel* model) override;
		void DrawInstanced(BaseModel* model) override;
		void DrawIndexedInstanced(BaseModel* model) override;

		void DrawIndexed(Quad* quad, bool depth_on) override;
		void Draw(Line3D* line, bool depth_on = true) override;

		void UpdateConstantBuffer(IBuffer* dest, void* src, s32 size) override;

		void UpdateBuffer(IBuffer* dest, void* src, s32 size, eMapping mapping) override;

		void SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth) override;
		void SetRasterizerState(IRasterizerState* pRasterizerState) override;
		void SetBlendState(IBlendState* pBlendState) override;

		void SetViewport(Viewport* viewport) override;


	private:
		ID3D11DeviceContext* m_Context = nullptr;
		void* GetContext() override { return m_Context; }

		ID3D11DepthStencilState* m_EnableZ = nullptr;
		ID3D11DepthStencilState* m_DisableZ = nullptr;




	};



};