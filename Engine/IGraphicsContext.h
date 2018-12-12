#pragma once
#include <Engine/engine_shared.h>
#include <Engine/graphics_flags.h>

struct CompiledShader;
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

		virtual void VSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;
		virtual void PSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;
		virtual void GSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;
		virtual void DSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;
		virtual void HSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;
		virtual void CSSetShaderResource(int32 start_slot, int32 count, void* resources) = 0;

		virtual void SetVertexShader(CompiledShader* shader) = 0;
		virtual void SetPixelShader(CompiledShader* shader) = 0;
		virtual void SetGeometryShader(CompiledShader* shader) = 0;
		virtual void SetHullShader(CompiledShader* shader) = 0;
		virtual void SetDomainShader(CompiledShader* shader) = 0;
		virtual void SetComputeShader(CompiledShader* shader) = 0;

		virtual void SetVertexShader(void* shader) = 0;
		virtual void SetPixelShader(void* shader) = 0;
		virtual void SetGeometryShader(void* shader) = 0;
		virtual void SetHullShader(void* shader) = 0;
		virtual void SetDomainShader(void* shader) = 0;
		virtual void SetComputeShader(void* shader) = 0;

		virtual void VSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void PSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void GSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void HSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void DSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;
		virtual void CSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) = 0;


		virtual void VSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void PSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void GSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void HSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void DSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;
		virtual void CSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) = 0;


		virtual void VSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void PSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void GSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void HSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void DSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;
		virtual void CSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) = 0;



		virtual void IASetInputLayout(IInputLayout* input_layout) = 0;
		virtual void IASetTopology(eTopology topology) = 0;

		virtual void OMSetRenderTargets(int32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) = 0;
		virtual void OMSetRenderTargets(int32 num_views, Texture* render_targets, Texture* dsv) = 0;
		virtual void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) = 0;
		virtual void ClearRenderTarget(Texture* render_target, const float clear_color[4]) = 0;

		virtual void ClearDepthStencilView(IDepthStencilView* dsv, int32 clear_flag, int32 max_depth) = 0;
		virtual void ClearDepthStencilView(Texture* dsv, int32 clear_flag, int32 max_depth) = 0;

		virtual void Draw(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void Draw(CEmitterInstance* emitter) = 0;
		virtual void Draw(Line3D* line, bool depth_on = true) = 0;

		virtual void DrawIndexed(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void DrawIndexed(Quad* quad, bool depth_on = false) = 0;
		virtual void DrawIndexed(CFont* font, Effect* effect) = 0;

		virtual void DrawInstanced(BaseModel* model, Effect* fx = nullptr) = 0;

		virtual void DrawIndexedInstanced(BaseModel* model, Effect* fx = nullptr) = 0;
		virtual void DrawIndexedInstanced(Model* model, IBuffer* instance_buffer, size_t stride) = 0;



		virtual void SetDepthState(IDepthStencilState* pDepthStencilState, int32 max_depth) = 0;
		virtual void SetRasterizerState(IRasterizerState* pRasterizerState) = 0;
		virtual void SetBlendState(IBlendState* pBlendState) = 0;

		virtual void SetDepthState(eDepthStencilState depth_state, int32 max_depth) = 0;
		virtual void SetRasterState(eRasterizer raster_state) = 0;
		//virtual void SetBlendState(eBlendStates blend_state, const float blend_color[4], u32 mask) = 0;
		virtual void SetBlendState(eBlendStates blend_state, const float blend_color[4] = blendcolor::black, uint32 mask = 0xFFFFFFFF) = 0;



		virtual void SetViewport(Viewport* viewport) = 0;
		
		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src);

		virtual void UpdateConstantBuffer(IBuffer*& dest, void* src, int32 size, int32 byte_place_in_buffer) = 0;

		template<typename T>
		void UpdateConstantBuffer(IBuffer*& dest, T* src, int32 size);

		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping);
		template<typename T>
		void UpdateBuffer(IBuffer*& dest, T* src, int32 size, eMapping mapping);


		virtual int8* Map(IBuffer* buffer) = 0;
		virtual void Unmap(IBuffer* buffer) = 0;

	protected:

		CompiledShader* m_ActiveShaders[SamplerState::NOF_TYPES];


		virtual void* GetContext() = 0;

		virtual void _InternalUpdateConstantBuffer(IBuffer*& dest, int8* src, int32 size) = 0;
		virtual void _InternalUpdateBuffer(IBuffer*& dest, int8* src, int32 size, eMapping mapping) = 0;

		ISamplerState* m_SamplerStates[NOF_SS];
		IDepthStencilState* m_DepthStencilStates[NOF_DSS];
		IRasterizerState* m_RasterizerStates[NOF_RS];
		IBlendState* m_BlendStates[NOF_BS];

	};
	

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src, int32 size)
	{
		_InternalUpdateConstantBuffer(dest, (int8*)src, size);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateConstantBuffer(IBuffer*& dest, T* src)
	{
		_InternalUpdateConstantBuffer(dest, (int8*)src, sizeof(T));
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, eMapping mapping)
	{
		_InternalUpdateBuffer(dest, (int8*)src, sizeof(T), mapping);
	}

	template<typename T>
	void graphics::IGraphicsContext::UpdateBuffer(IBuffer*& dest, T* src, int32 size, eMapping mapping)
	{
		_InternalUpdateBuffer(dest, (int8*)src, size, mapping);
	}


};
