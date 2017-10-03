#pragma once

class CompiledShader;
class BaseModel;
class CEmitterInstance;
class Quad;
class Line3D;
class CFont;
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
		virtual void PSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers) = 0;



		virtual void IASetInputLayout(IInputLayout* input_layout) = 0;
		virtual void IASetTopology(eTopology topology) = 0;

		virtual void OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) = 0;
		virtual void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) = 0;

		virtual void ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth) = 0;

		virtual void Draw(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void Draw(CEmitterInstance* emitter) = 0;
		virtual void Draw(Line3D* line, bool depth_on = true) = 0;

		virtual void DrawIndexed(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void DrawIndexed(Quad* quad, bool depth_on = false) = 0;
		virtual void DrawIndexed(CFont* font, Effect* effect) = 0;

		virtual void DrawInstanced(BaseModel* model, Effect* fx = nullptr) = 0;

		virtual void DrawIndexedInstanced(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void DrawIndexedInstanced(Model* model, Effect* fx = nullptr) = 0;



		virtual void SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth) = 0;
		virtual void SetRasterizerState(IRasterizerState* pRasterizerState) = 0;
		virtual void SetBlendState(IBlendState* pBlendState) = 0;

		virtual void SetViewport(Viewport* viewport) = 0;
		
		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src);

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size);

		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping);
		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, s32 size, eMapping mapping);

	protected:
		virtual void* GetContext() = 0;

		virtual void UpdateConstantBuffer(IBuffer*& dest, s8* src, s32 size, bool internal) = 0;
		virtual void UpdateBuffer(IBuffer*& dest, s8* src, s32 size, eMapping mapping, bool internal) = 0;

	};

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size)
	{
		UpdateConstantBuffer(dest, (s8*)src, size, true);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src)
	{
		UpdateConstantBuffer(dest, (s8*)src, sizeof(T), true);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping)
	{
		UpdateBuffer(dest, (s8*)src, sizeof(T), mapping, true);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, s32 size, eMapping mapping)
	{
		UpdateBuffer(dest, (s8*)src, size, mapping, true);
	}


};
