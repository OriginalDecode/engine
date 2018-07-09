#pragma once
#include "../engine/engine_shared.h"
#include <Graphics/graphics_flags.h>

class CompiledShader;
class BaseModel;
class CEmitterInstance;
class Quad;
class Line3D;
class CFont;
class Model;
class Texture;
class Viewport;
class Effect;


namespace graphics
{
	class IGraphicsContext
	{
		friend class IGraphicsAPI;
	public:

		~IGraphicsContext();

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


		virtual void VSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void PSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void GSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void HSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void DSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void CSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate) = 0;



		virtual void IASetInputLayout(IInputLayout* input_layout) = 0;
		virtual void IASetTopology(eTopology topology) = 0;

		virtual void OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) = 0;
		virtual void OMSetRenderTargets(s32 num_views, Texture* render_targets, Texture* dsv) = 0;
		virtual void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) = 0;
		virtual void ClearRenderTarget(Texture* render_target, const float clear_color[4]) = 0;

		virtual void ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth) = 0;
		virtual void ClearDepthStencilView(Texture* dsv, s32 clear_flag, s32 max_depth) = 0;

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

		virtual void SetDepthState(eDepthStencilState depth_state, s32 max_depth) = 0;
		virtual void SetRasterState(eRasterizer raster_state) = 0;
		virtual void SetBlendState(eBlendStates blend_state, const float blend_color[4], u32 mask) = 0;



		virtual void SetViewport(Viewport* viewport) = 0;
		
		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src);

		virtual void UpdateConstantBuffer(IBuffer*& dest, void* src, s32 size, s32 byte_place_in_buffer) = 0;

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size);

		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping);
		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, s32 size, eMapping mapping);


		virtual s8* Map(IBuffer* buffer) = 0;
		virtual void Unmap(IBuffer* buffer) = 0;

	protected:
		virtual void* GetContext() = 0;

		virtual void _InternalUpdateConstantBuffer(IBuffer*& dest, s8* src, s32 size) = 0;
		virtual void _InternalUpdateBuffer(IBuffer*& dest, s8* src, s32 size, eMapping mapping) = 0;

		ISamplerState* m_SamplerStates[NOF_SS];
		IDepthStencilState* m_DepthStencilStates[NOF_DSS];
		IRasterizerState* m_RasterizerStates[NOF_RS];
		IBlendState* m_BlendStates[NOF_BS];

	};
	

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src, s32 size)
	{
		_InternalUpdateConstantBuffer(dest, (s8*)src, size);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src)
	{
		_InternalUpdateConstantBuffer(dest, (s8*)src, sizeof(T));
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping)
	{
		_InternalUpdateBuffer(dest, (s8*)src, sizeof(T), mapping);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, s32 size, eMapping mapping)
	{
		_InternalUpdateBuffer(dest, (s8*)src, size, mapping);
	}


};
