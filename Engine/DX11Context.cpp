#include "stdafx.h"
#include "DX11Context.h"


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

	void DX11Context::VSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->VSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::PSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->PSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::GSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->GSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::HSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->HSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::DSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->DSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

	void DX11Context::CSSetConstantBuffer(s32 start_index, s32 buffer_count, void* pBuffer)
	{
		m_Context->CSSetConstantBuffers(start_index, buffer_count, static_cast<ID3D11Buffer*const*>(pBuffer));
	}

};