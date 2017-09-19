#include "stdafx.h"
#include "DX11Context.h"

#include <Engine/Model.h>
#include <Engine/Quad.h>
#include <Engine/DX11Device.h>

namespace graphics
{
	DX11Context::DX11Context(ID3D11DeviceContext* context)
		: m_Context(context)
	{
		m_DisableZ = static_cast<ID3D11DepthStencilState*>(Engine::GetAPI()->GetDepthStencilState(Z_DISABLED));
		m_EnableZ = static_cast<ID3D11DepthStencilState*>(Engine::GetAPI()->GetDepthStencilState(Z_ENABLED));
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

	void DX11Context::PSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		PSSetSamplerState(0, 1, Engine::GetAPI()->GetSamplerState(samplerstate));
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

	void DX11Context::IASetInputLayout(IInputLayout* input_layout)
	{
		assert("not implemented!");
	}

	void DX11Context::IASetTopology(eTopology topology)
	{
		D3D_PRIMITIVE_TOPOLOGY d3d_topology = DirectX11::GetTopology(topology);
		m_Context->IASetPrimitiveTopology(d3d_topology);
	}

	void DX11Context::OMSetRenderTargets(s32 num_views, IRenderTargetView* render_targets, IDepthStencilView* dsv)
	{
		m_Context->OMSetRenderTargets(num_views, 
									  static_cast<ID3D11RenderTargetView*const*>(render_targets), 
									  static_cast<ID3D11DepthStencilView*>(dsv));
	}

	void DX11Context::ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4])
	{
		m_Context->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(render_target), clear_color);
	}

	void DX11Context::ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth)
	{
		m_Context->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(dsv), clear_flag, max_depth, 0);
	}

	void DX11Context::Draw(BaseModel* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		m_Context->Draw(vtx.GetVertexCount(), vtx.GetStart());
	}

	void DX11Context::DrawIndexed(Quad* quad, bool depth_on)
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

		m_Context->OMSetDepthStencilState(depth_on ? m_EnableZ : m_DisableZ, 1);
		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
	}

	void DX11Context::DrawIndexed(BaseModel* model)
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

	void DX11Context::SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth)
	{
		m_Context->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(pDepthStencilState), max_depth);
	}

	void DX11Context::SetRasterizerState(IRasterizerState* pRasterizerState)
	{
		m_Context->RSSetState(static_cast<ID3D11RasterizerState*>(pRasterizerState));
	}

	void DX11Context::SetBlendState(IBlendState* pBlendState)
	{
		m_Context->OMSetBlendState(static_cast<ID3D11BlendState*>(pBlendState), blendcolor::black, 0xFFFFFFFF );
	}

	void DX11Context::DrawInstanced(BaseModel* model)
	{
		const auto& vtx = model->GetVertexWrapper();
		const auto& ins = model->GetInstanceWrapper();
		m_Context->DrawInstanced(ins.GetVertCountPerInstance(), ins.GetInstanceCount(), vtx.GetStart(), ins.GetStart());
	}

	void DX11Context::DrawIndexedInstanced(BaseModel* model)
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

	void DX11Context::UpdateBuffer(IBuffer* dest, void* src, s32 size, eMapping mapping)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(dest);
		m_Context->Map(buffer, 0, DirectX11::GetMapping(mapping), 0, &msr);

		if (msr.pData)
		{
			void* data = msr.pData;
			memcpy(data, &src, size);
		}
		m_Context->Unmap(buffer, 0);
	}

	void DX11Context::UpdateConstantBuffer(IBuffer* dest, void* src, s32 size)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(dest);
		m_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (msr.pData)
		{
			void* data = msr.pData;
			memcpy(data, &src, size);
		}
		m_Context->Unmap(buffer, 0);

	}
};