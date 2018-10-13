#include "stdafx.h"
#include "DX11Context.h"

#include "DirectX11.h"
#include <Engine/BaseModel.h>
#include <Engine/Quad.h>
#include <Engine/Line3D.h>
#include <Engine/DX11Device.h>
#include <Engine/Viewport.h>
#include <Engine/EmitterInstance.h>
#include <Engine/VertexWrapper.h>
#include <Engine/IndexWrapper.h>
#include <Engine/InstanceWrapper.h>
namespace graphics
{
	DX11Context::DX11Context(ID3D11DeviceContext* context)
		: m_Context(context)
	{
	}
	//_______________________________________________________________________

	void DX11Context::VSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->VSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->VSSetShaderResources(start_slot, count, &null_resource);
		}
	}
	//_______________________________________________________________________

	void DX11Context::PSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->PSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->PSSetShaderResources(start_slot, count, &null_resource);
		}
	}
	//_______________________________________________________________________

	void DX11Context::GSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->GSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->GSSetShaderResources(start_slot, count, &null_resource);
		}
	}
	//_______________________________________________________________________

	void DX11Context::DSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->DSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->DSSetShaderResources(start_slot, count, &null_resource);
		}
		//m_Context->DSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}
	//_______________________________________________________________________

	void DX11Context::HSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->HSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->HSSetShaderResources(start_slot, count, &null_resource);
		}
		//m_Context->HSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}
	//_______________________________________________________________________

	void DX11Context::CSSetShaderResource(s32 start_slot, s32 count, void* resources)
	{
		if (resources)
		{
			m_Context->CSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
		}
		else
		{
			ID3D11ShaderResourceView* null_resource = nullptr;
			m_Context->CSSetShaderResources(start_slot, count, &null_resource);
		}
		//m_Context->CSSetShaderResources(start_slot, count, static_cast<ID3D11ShaderResourceView*const*>(resources));
	}
	//_______________________________________________________________________

	void DX11Context::SetVertexShader(CompiledShader* shader)
	{
		m_Context->VSSetShader(shader ? static_cast<ID3D11VertexShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetVertexShader(void* shader)
	{
		m_Context->VSSetShader(shader ? static_cast<ID3D11VertexShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::SetPixelShader(CompiledShader* shader)
	{
		m_Context->PSSetShader(shader ? static_cast<ID3D11PixelShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetPixelShader(void* shader)
	{
		m_Context->PSSetShader(shader ? static_cast<ID3D11PixelShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::SetGeometryShader(CompiledShader* shader)
	{
		m_Context->GSSetShader(shader ? static_cast<ID3D11GeometryShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetGeometryShader(void* shader)
	{
		m_Context->GSSetShader(shader ? static_cast<ID3D11GeometryShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::SetHullShader(CompiledShader* shader)
	{
		m_Context->HSSetShader(shader ? static_cast<ID3D11HullShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetHullShader(void* shader)
	{
		m_Context->HSSetShader(shader ? static_cast<ID3D11HullShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::SetDomainShader(CompiledShader* shader)
	{
		m_Context->DSSetShader(shader ? static_cast<ID3D11DomainShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetDomainShader(void* shader)
	{
		m_Context->DSSetShader(shader ? static_cast<ID3D11DomainShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::SetComputeShader(CompiledShader* shader)
	{
		m_Context->CSSetShader(shader ? static_cast<ID3D11ComputeShader*>(shader->m_Shader) : nullptr, nullptr, 0);
	}

	void DX11Context::SetComputeShader(void* shader)
	{
		m_Context->CSSetShader(shader ? static_cast<ID3D11ComputeShader*>(shader) : nullptr, nullptr, 0);
	}

	//_______________________________________________________________________

	void DX11Context::VSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->VSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::PSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->PSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::GSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->GSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::HSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->HSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::DSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->DSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::CSSetConstantBuffer(s32 start_index, s32 buffer_count, IBuffer* pBuffer)
	{
		m_Context->CSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}
	//_______________________________________________________________________

	void DX11Context::VSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->VSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::VSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		VSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::PSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->PSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::PSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		PSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::GSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->GSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::GSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		GSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::HSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->HSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::HSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		HSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::DSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->DSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::DSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		DSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::CSSetSamplerState(s32 start_index, s32 sampler_count, ISamplerState* pSamplers)
	{
		m_Context->CSSetSamplers(start_index, sampler_count, static_cast<ID3D11SamplerState*const*>(pSamplers));
	}
	//_______________________________________________________________________

	void DX11Context::CSSetSamplerState(s32 start_index, s32 sampler_count, eSamplerStates samplerstate)
	{
		CSSetSamplerState(start_index, sampler_count, &m_SamplerStates[samplerstate]);
	}
	//_______________________________________________________________________

	void DX11Context::IASetInputLayout(IInputLayout* input_layout)
	{
		m_Context->IASetInputLayout(static_cast<ID3D11InputLayout*>(input_layout));
	}
	//_______________________________________________________________________

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
	//_______________________________________________________________________

	void DX11Context::OMSetRenderTargets(s32 num_views, Texture* render_targets, Texture* dsv)
	{
		OMSetRenderTargets(num_views, render_targets->GetRenderTargetView(), dsv->GetDepthStencilView());
	}
	//_______________________________________________________________________

	void DX11Context::UpdateConstantBuffer(IBuffer*& dest, void* src, s32 size, s32 byte_place_in_buffer)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(dest);
		m_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (msr.pData)
		{
			s8* data = static_cast<s8*>(msr.pData);
			memcpy(&data[byte_place_in_buffer], static_cast<s8*>(src), size);
		}
		m_Context->Unmap(buffer, 0);
	}
	//_______________________________________________________________________

	s8* DX11Context::Map(IBuffer* in_buffer)
	{
		PROFILE_FUNCTION(profiler::colors::Orange);
		D3D11_MAPPED_SUBRESOURCE msr; //can I do this?
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(in_buffer);
		HRESULT hr = m_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		DirectX11::HandleErrors(hr, "Failed to map buffer");

		return static_cast<s8*>(msr.pData);
	}
	//_______________________________________________________________________

	void DX11Context::Unmap(IBuffer* buffer)
	{
		m_Context->Unmap(static_cast<ID3D11Buffer*>(buffer), 0);
	}
	//_______________________________________________________________________

	void DX11Context::ClearRenderTarget(IRenderTargetView* render_target, const float clear_color[4])
	{
		m_Context->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(render_target), clear_color);
	}
	//_______________________________________________________________________

	void DX11Context::ClearRenderTarget(Texture* render_target, const float clear_color[4])
	{
		ClearRenderTarget(render_target->GetRenderTargetView(), clear_color);
	}
	//_______________________________________________________________________

	void DX11Context::ClearDepthStencilView(IDepthStencilView* dsv, s32 clear_flag, s32 max_depth)
	{
		m_Context->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(dsv), clear_flag, max_depth, 0);
	}
	//_______________________________________________________________________

	void DX11Context::Draw(BaseModel* model, Effect* fx)
	{
		auto& vtx = model->GetVertexWrapper();

		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());
		ID3D11Buffer* pVtxBuffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());
		const u32 stride = vtx.GetStride();
		const u32 offset = vtx.GetByteOffset();

		m_Context->IASetVertexBuffers(0,
									  1,
									  &pVtxBuffer,
									  &stride,
									  &offset);

		fx->Use();
		m_Context->Draw(vtx.GetVertexCount(), vtx.GetStart());
		fx->Clear();
	}
	//_______________________________________________________________________

	void DX11Context::Draw(Line3D* line, bool depth_on /*= true*/)
	{
		auto& vtx = line->GetVertexWrapper();
		auto fx = line->GetEffect();
		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());

		u32 stride = vtx.GetStride();
		u32 offset = vtx.GetByteOffset();
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());

		m_Context->IASetVertexBuffers(vtx.GetStart(),
									  vtx.GetBufferCount(),
									  &buffer,
									  &stride,
									  &offset);

		fx->Use();
		m_Context->Draw(vtx.GetVertexCount(), 0);
		fx->Clear();
	}
	//_______________________________________________________________________

	void DX11Context::Draw(CEmitterInstance* emitter)
	{
		auto& vtx = emitter->GetVertexWrapper();
		auto fx = emitter->GetEffect();
		fx->Use();
		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());

		u32 stride = vtx.GetStride();
		u32 offset = vtx.GetByteOffset();
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());

		m_Context->IASetVertexBuffers(vtx.GetStart(),
									  vtx.GetBufferCount(),
									  &buffer,
									  &stride,
									  &offset);
		m_Context->Draw(vtx.GetVertexCount(), vtx.GetStart());
		fx->Clear();
	}
	//_______________________________________________________________________

	void DX11Context::DrawIndexed(CFont* font, Effect* effect)
	{
		auto& vtx = font->GetVertexWrapper();
		auto& idx = font->GetIndexWrapper();

		assert(vtx.GetBufferCount() > 0 && "Tried to bind 0 buffers!");

		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());

		ID3D11Buffer* pVtxBuffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());
		const u32 stride = vtx.GetStride();
		const u32 offset = vtx.GetByteOffset();

		m_Context->IASetVertexBuffers(vtx.GetStart(),
									  vtx.GetBufferCount(),
									  &pVtxBuffer,
									  &stride,
									  &offset);

		m_Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(idx.GetIndexBuffer()),
									DirectX11::GetFormat(idx.GetFormat()),
									idx.GetByteOffset());


		effect->Use();
		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
		effect->Clear();
	}
	//_______________________________________________________________________

	void DX11Context::DrawIndexed(Quad* quad, bool depth_on)
	{
		auto& vtx = quad->GetVertexWrapper();
		auto& idx = quad->GetIndexWrapper();

		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());


		ID3D11Buffer* pVtxBuffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());
		const u32 stride = vtx.GetStride();
		const u32 offset = vtx.GetByteOffset();

		m_Context->IASetVertexBuffers(vtx.GetStart(),
									  vtx.GetBufferCount(),
									  &pVtxBuffer,
									  &stride,
									  &offset);


		ID3D11Buffer* pIdxBuffer = static_cast<ID3D11Buffer*>(idx.GetIndexBuffer());
		m_Context->IASetIndexBuffer(pIdxBuffer,
									DirectX11::GetFormat(idx.GetFormat()),
									idx.GetByteOffset());

		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
	}
	//_______________________________________________________________________

	void DX11Context::DrawIndexed(BaseModel* model, Effect*)
	{
		auto& vtx = model->GetVertexWrapper();
		auto& idx = model->GetIndexWrapper();

		assert(vtx.GetInputLayout());


		IASetInputLayout(vtx.GetInputLayout());
		IASetTopology(vtx.GetTopology());

		ID3D11Buffer* pVtxBuffer = static_cast<ID3D11Buffer*>(vtx.GetVertexBuffer());
		const u32 stride = vtx.GetStride();
		const u32 offset = vtx.GetByteOffset();
		
		m_Context->IASetVertexBuffers(0,
									  1,
									  &pVtxBuffer,
									  &stride,
									  &offset);

		ID3D11Buffer* pBuffer = static_cast<ID3D11Buffer*>(idx.GetIndexBuffer());
		const DXGI_FORMAT format = DirectX11::GetFormat(idx.GetFormat());
		const u32 idx_offset = idx.GetByteOffset();

		m_Context->IASetIndexBuffer(pBuffer,
									format,
									idx_offset);
	
		m_Context->DrawIndexed(idx.GetIndexCount(), idx.GetStart(), vtx.GetStart());
	}
	//_______________________________________________________________________

	void DX11Context::SetDepthState(IDepthStencilState* pDepthStencilState, s32 max_depth)
	{
		m_Context->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(pDepthStencilState), max_depth);
	}
	//_______________________________________________________________________

	void DX11Context::SetDepthState(eDepthStencilState depth_state, s32 max_depth)
	{
		m_Context->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(m_DepthStencilStates[depth_state]), max_depth);
	}
	//_______________________________________________________________________

	void DX11Context::SetRasterizerState(IRasterizerState* pRasterizerState)
	{
		m_Context->RSSetState(static_cast<ID3D11RasterizerState*>(pRasterizerState));
	}
	//_______________________________________________________________________

	void DX11Context::SetBlendState(IBlendState* pBlendState)
	{
		m_Context->OMSetBlendState(static_cast<ID3D11BlendState*>(pBlendState), blendcolor::black, 0xFFFFFFFF);
	}
	//_______________________________________________________________________

	void DX11Context::SetBlendState(eBlendStates blend_state, const float blend_color[4], u32 mask)
	{
		m_Context->OMSetBlendState(static_cast<ID3D11BlendState*>(m_BlendStates[blend_state]), blend_color, mask);
	}
	//_______________________________________________________________________

	void DX11Context::SetViewport(Viewport* viewport)
	{
		D3D11_VIEWPORT* vp = static_cast<D3D11_VIEWPORT*>(viewport->GetViewport());

		vp->Height = viewport->GetHeight();
		vp->Width = viewport->GetWidth();

		vp->MaxDepth = viewport->GetMaxDepth();
		vp->MinDepth = viewport->GetMinDepth();

		vp->TopLeftX = viewport->GetTopLeftX();
		vp->TopLeftY = viewport->GetTopLeftY();

		m_Context->RSSetViewports(1, vp);
	}
	//_______________________________________________________________________

	void DX11Context::DrawInstanced(BaseModel* model, Effect* fx)
	{
		const auto& vtx = model->GetVertexWrapper();
		const auto& ins = model->GetInstanceWrapper();

		if (fx)
			fx->Use();

		m_Context->DrawInstanced(ins.GetVertCountPerInstance(), ins.GetInstanceCount(), vtx.GetStart(), ins.GetStart());

		if (fx)
			fx->Clear();
	}
	//_______________________________________________________________________

	void DX11Context::DrawIndexedInstanced(BaseModel* model, Effect* fx)
	{
		std::bad_function_call;
	}
	//_______________________________________________________________________

	void DX11Context::DrawIndexedInstanced(Model* model, Effect* fx /*= nullptr*/)
	{
		auto& vtx = model->GetVertexWrapper();
		auto& idx = model->GetIndexWrapper();
		auto& ins = model->GetInstanceWrapper();

		IInputLayout* input_layout = ins.GetInputLayout();
		IASetInputLayout(input_layout);

		IASetTopology(vtx.GetTopology());

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
									  ARRSIZE(buffers),
									  buffers,
									  strides,
									  byte_offsets);

		m_Context->IASetIndexBuffer(static_cast<ID3D11Buffer*>(idx.GetIndexBuffer()),
									DirectX11::GetFormat(idx.GetFormat()),
									idx.GetByteOffset());

		//if (fx)
		//{
		//	//fx->Use();
		//	m_Context->DrawIndexedInstanced(ins.GetIndexCountPerInstance(), model->GetInstanceCount(), idx.GetStart(), vtx.GetStart(), ins.GetStart());
		//	//fx->Clear();
		//	return;
		//}

		m_Context->DrawIndexedInstanced(ins.GetIndexCountPerInstance(), model->GetInstanceCount(), idx.GetStart(), vtx.GetStart(), ins.GetStart());

	}
	//_______________________________________________________________________

	void DX11Context::_InternalUpdateConstantBuffer(IBuffer*& dest, s8* src, s32 size)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ZeroMemory(&msr, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(dest);
		m_Context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (msr.pData)
		{
			s8* data = (s8*)msr.pData;
			memcpy(data, &src[0], size);
		}
		m_Context->Unmap(buffer, 0);
	}
	//_______________________________________________________________________

	void DX11Context::_InternalUpdateBuffer(IBuffer*& dest, s8* src, s32 size, eMapping mapping)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(dest);
		m_Context->Map(buffer, 0, DirectX11::GetMapping(mapping), 0, &msr);

		if (msr.pData)
		{
			s8* data = (s8*)msr.pData;
			memcpy(data, &src[0], size);
		}
		m_Context->Unmap(buffer, 0);
	}
	//_______________________________________________________________________

	void DX11Context::SetRasterState(eRasterizer raster_state)
	{
		m_Context->RSSetState(static_cast<ID3D11RasterizerState*>(m_RasterizerStates[raster_state]));
	}

};