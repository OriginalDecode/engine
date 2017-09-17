#pragma once

class CompiledShader;
class BaseModel;
class Quad;
namespace graphics
{
	class IGraphicsContext
	{
	public:
		virtual void VSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
		virtual void PSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
		virtual void GSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
		virtual void DSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
		virtual void HSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;
		virtual void CSSetShaderResource(s32 start_slot, s32 count, void* resources) = 0;

		virtual void SetVertexShader(CompiledShader* vertex_shader) = 0;
		virtual void SetPixelShader(CompiledShader* vertex_shader) = 0;
		virtual void SetGeometryShader(CompiledShader* vertex_shader) = 0;
		virtual void SetHullShader(CompiledShader* vertex_shader) = 0;
		virtual void SetDomainShader(CompiledShader* vertex_shader) = 0;
		virtual void SetComputeShader(CompiledShader* vertex_shader) = 0;

		virtual void VSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void PSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void GSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void HSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void DSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void CSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer) = 0;


		virtual void VSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void PSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;



		virtual void IASetInputLayout(IInputLayout* input_layout) = 0;
		virtual void IASetTopology(eTopology topology) = 0;

		virtual void OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) = 0;
		virtual void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) = 0;

		virtual void ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth) = 0;



		virtual void Draw(s32 vertex_start, s32 vertex_count) = 0;
		virtual void Draw(s32 index_start, s32 index_count, s32 vertex_start) = 0;
		virtual void Draw(s32 index_count, s32 instance_count, s32 index_start, s32 vertex_start, s32 instance_start) = 0;

		virtual void Draw(BaseModel* model) = 0;
		virtual void DrawIndexed(BaseModel* model) = 0;
		virtual void DrawInstanced(BaseModel* model) = 0;
		virtual void DrawIndexedInstanced(BaseModel* model) = 0;

		virtual void DrawIndexed(Quad* quad, bool depth_on) = 0;

		template<typename T>
		virtual void UpdateConstantBuffer(IBuffer* dest, T* src, s32 size) = 0;
		
		template<typename T>
		virtual void UpdateBuffer(IBuffer* dest, T* src, s32 size, eMapping mapping) = 0;

		virtual void SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth) = 0;


	protected:

	};
};
