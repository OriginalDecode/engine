#pragma once

class CompiledShader;
namespace graphics
{
	class IGraphicsContext
	{
	public:
		virtual void* GetContext() = 0;

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

		virtual void VSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;
		virtual void PSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;
		virtual void GSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;
		virtual void HSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;
		virtual void DSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;
		virtual void CSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer) = 0;



	private:


	};
};