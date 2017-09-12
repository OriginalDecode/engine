#include "stdafx.h"
#include "DX11Context.h"

#include <Engine/Model.h>
#include <Engine/Quad.h>

namespace graphics
{
	DX11Context::DX11Context(ID3D11DeviceContext* context)
		: m_Context(context)
	{
	}

	void DX11Context::VSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->VSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::PSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->PSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::GSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->GSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::DSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->DSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::HSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->HSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::CSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		m_Context->CSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}

	void DX11Context::SetVertexShader(CompiledShader* shader)
	{
		m_Context->VSSetShader(shader ? static_cast<ID3D11VertexShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetPixelShader(CompiledShader* shader)
	{
		m_Context->PSSetShader(shader ? static_cast<ID3D11PixelShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetGeometryShader(CompiledShader* shader)
	{
		m_Context->GSSetShader(shader ? static_cast<ID3D11GeometryShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetHullShader(CompiledShader* shader)
	{
		m_Context->HSSetShader(shader ? static_cast<ID3D11HullShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetDomainShader(CompiledShader* shader)
	{
		m_Context->DSSetShader(shader ? static_cast<ID3D11DomainShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetComputeShader(CompiledShader* shader)
	{
		m_Context->CSSetShader(shader ? static_cast<ID3D11ComputeShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::VSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->VSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::PSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->PSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::GSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->GSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::HSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->HSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::DSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->DSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::CSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->CSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::VSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->VSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::PSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->PSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->GSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->HSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->DSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->CSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}

	void DX11Context::IASetTopology(eTopology topology)
	{
		D3D_PRIMITIVE_TOPOLOGY topology = DirectX11::GetTopology(topology);
	}

	void DX11Context::Draw(Model* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		m_Context->Draw(vtx.GetVertexCount(), vtx.GetStart());
	}

	void DX11Context::DrawIndexed(Quad* quad)
	{
		const auto& vtx = quad->GetVertexWrapper();
		const auto& idx = quad->GetIndexWrapper();

		m_Context->IASetInputLayout(static_cast<ID3D11InputLayout*>(vtx.GetInputLayout()));
		m_Context->IASetPrimitiveTopology(DirectX11::GetTopology(vtx.GetTopology()));

		m_Context->IASetVertexBuffers(vtx.GetStart(), 
									  vtx.GetBufferCount(), 
									  static_cast<ID3D11Buffer*const*>(vtx.GetVertexBuffer()), 
									  &vtx.GetStride(), 
									  &vtx.GetByteOffset());

		m_Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(idx.GetIndexBuffer()),
									DirectX11::GetFormat(idx.GetFormat()), 
									idx.GetByteOffset());

		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
	}

	void DX11Context::DrawIndexed(Model* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		const auto& idx = model->GetIndexWrapper();

		m_Context->IASetInputLayout(static_cast<ID3D11InputLayout*>(vtx.GetInputLayout()));
		m_Context->IASetPrimitiveTopology(DirectX11::GetTopology(vtx.GetTopology()));

		m_Context->IASetVertexBuffers(vtx.GetStart(), 
									  vtx.GetBufferCount(), 
									  static_cast<ID3D11Buffer*const*>(vtx.GetVertexBuffer()), 
									  &vtx.GetStride(), 
									  &vtx.GetByteOffset());

		m_Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(idx.GetIndexBuffer()),
									DirectX11::GetFormat(idx.GetFormat()),
									idx.GetByteOffset());

		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
	}

	void DX11Context::DrawInstanced(Model* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		const auto& ins = model->GetInstanceWrapper();
		m_Context->DrawInstanced(ins.GetVertCountPerInstance(), ins.GetInstanceCount(), vtx.GetStart(), ins.GetStart());
	}

	void DX11Context::DrawIndexedInstanced(Model* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		const auto& idx = model->GetIndexWrapper();
		const auto& ins = model->GetInstanceWrapper();

		m_Context->IASetInputLayout(static_cast<ID3D11InputLayout*>(ins.GetInputLayout()));

		ID3D11Buffer* buffers[] = {
			static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer()),
			static_cast<ID3D11Buffer*>(ins.GetInstanceBuffer())
		};

		u32 strides[] = {
			vtx.GetStride(),
			ins.GetStride()
		};

		u32 byte_offsets[] = {
			vtx.GetByteOffset(),
			ins.GetByteOffset()
		};

		m_Context->IASetVertexBuffers(ins.GetStart(), 
									  ins.GetBufferCount(), 
									  buffers, 
									  strides, 
									  byte_offsets);

		m_Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(idx.GetIndexBuffer()), 
									DirectX11::GetFormat(idx.GetFormat()),
									idx.GetByteOffset());
		
		m_Context->DrawIndexedInstanced(ins.GetIndexCountPerInstance(), ins.GetInstanceCount(), idx.GetStart(), vtx.GetStart(), ins.GetStart());
	}
};