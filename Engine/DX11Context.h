#pragma once
#include <Engine/engine_shared.h>
#include "IGraphicsContext.h"

struct ID3D11DeviceContext;
class CompiledShader;
namespace graphics
{
	class DX11Context : public IGraphicsContext
	{
		friend class DX11Device;
		friend class DirectX11;
	public:
		DX11Context(ID3D11DeviceContext* context);


		void VSSetShaderResource(int32 start_slot, int32 count, void* resources) override;
		void PSSetShaderResource(int32 start_slot, int32 count, void* resources) override;
		void GSSetShaderResource(int32 start_slot, int32 count, void* resources) override;
		void DSSetShaderResource(int32 start_slot, int32 count, void* resources) override;
		void HSSetShaderResource(int32 start_slot, int32 count, void* resources) override;
		void CSSetShaderResource(int32 start_slot, int32 count, void* resources) override;

		void SetVertexShader(CompiledShader* shader) override;
		void SetPixelShader(CompiledShader* shader) override;
		void SetGeometryShader(CompiledShader* shader) override;
		void SetHullShader(CompiledShader* shader) override;
		void SetDomainShader(CompiledShader* shader) override;
		void SetComputeShader(CompiledShader* shader) override;

		void SetVertexShader(void* shader) override;
		void SetPixelShader(void* shader) override;
		void SetGeometryShader(void* shader) override;
		void SetHullShader(void* shader) override;
		void SetDomainShader(void* shader) override;
		void SetComputeShader(void* shader) override;


		void VSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;
		void PSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;
		void GSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;
		void HSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;
		void DSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;
		void CSSetConstantBuffer(int32 start_index, int32 buffer_count, IBuffer* pBuffer) override;

		void VSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;
		void PSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;
		void GSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;
		void HSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;
		void DSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;
		void CSSetSamplerState(int32 start_index, int32 sampler_count, ISamplerState* pSamplers) override;



		void VSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;
		void PSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;
		void GSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;
		void HSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;
		void DSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;
		void CSSetSamplerState(int32 start_index, int32 sampler_count, eSamplerStates samplerstate) override;



		void IASetInputLayout(IInputLayout* input_layout) override;
		void IASetTopology(eTopology topology) override;

		void OMSetRenderTargets(int32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv) override;
		void ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4]) override;

		void ClearDepthStencilView(IDepthStencilView* dsv, int32 clear_flag, int32 max_depth) override;


		void Draw(BaseModel* model, Effect* fx = nullptr) override;
		void Draw(CEmitterInstance* emitter) override;
		void Draw(Line3D* line, bool depth_on = true) override;

		void DrawIndexed(BaseModel* model, Effect* fx = nullptr) override;
		void DrawIndexed(Quad* quad, bool depth_on) override;
		void DrawIndexed(CFont* font, Effect* effect) override;

		void DrawInstanced(BaseModel* model, Effect* fx = nullptr) override;

		void DrawIndexedInstanced(BaseModel* model, Effect* fx = nullptr) override;
		void DrawIndexedInstanced(Model* model, IBuffer* instance_buffer, size_t stride) override;



		void SetDepthState(IDepthStencilState* pDepthStencilState, int32 max_depth) override;
		void SetRasterizerState(IRasterizerState* pRasterizerState) override;
		void SetBlendState(IBlendState* pBlendState) override;

		void SetViewport(Viewport* viewport) override;

		void SetDepthState(eDepthStencilState depth_state, int32 max_depth) override;
		void SetRasterState(eRasterizer raster_state) override;
		void SetBlendState(eBlendStates blend_state, const float blend_color[4] = blendcolor::black, uint32 mask = 0xFFFFFFFF) override;

		ID3D11DeviceContext* m_Context = nullptr;
		void* GetContext() override { return m_Context; }


		virtual void ClearDepthStencilView(Texture*, int32, int32) { assert(false && "not implemented"); }
		virtual void ClearRenderTarget(Texture* render_target, const float clear_color[4]);
		virtual void OMSetRenderTargets(int32 num_views, Texture* render_targets, Texture* dsv);

		void UpdateConstantBuffer(IBuffer*& dest, void* src, int32 size, int32 byte_place_in_buffer) override;
		int8* Map(IBuffer* buffer) override;
		void Unmap(IBuffer* buffer) override;
	private:


		virtual void _InternalUpdateConstantBuffer(IBuffer*& dest, int8* src, int32 size) override;
		virtual void _InternalUpdateBuffer(IBuffer*& dest, int8* src, int32 size, eMapping mapping) override;

	};



};