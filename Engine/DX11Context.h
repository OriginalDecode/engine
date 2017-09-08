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

	private:
		ID3D11DeviceContext* m_Context;
	};
};