#include "stdafx.h"
#include "Effect.h"
#include <Engine/IGraphicsContext.h>

Effect::Effect()
{
	for (int i = 0; i < eShaderType::NOF_TYPES; i++)
		m_Shaders[i] = nullptr;

	for (int i = 0; i < TextureSlot::_COUNT; i++)
		m_Resources[i] = nullptr;
}


Effect::Effect(const std::string& filepath)
	: m_FileName(filepath)
{
	for (int i = 0; i < eShaderType::NOF_TYPES; i++)
		m_Shaders[i] = nullptr;

	for (int i = 0; i < TextureSlot::_COUNT; i++)
		m_Resources[i] = nullptr;
}

void Effect::AddShaderResource(IShaderResourceView* pResource, s32 slot)
{
	/*if (m_UsedReg[slot] != 0)
	{
		DL_ASSERT("This slot is already occupied!");
		return;
	}*/
	
	m_Resources[slot] = pResource;
	//m_UsedReg[slot] = 1;
}

void Effect::AddShaderResource(Texture* pResource, s32 slot)
{
	AddShaderResource(pResource->GetShaderView(), slot);
}

void Effect::Use()
{
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	if (set_shaders)
	{
		context.SetVertexShader(m_Shaders[eShaderType::VERTEX]);
		context.SetPixelShader(m_Shaders[eShaderType::PIXEL]);
		context.SetGeometryShader(m_Shaders[eShaderType::GEOMETRY]);
		context.SetHullShader(m_Shaders[eShaderType::HULL]);
		context.SetDomainShader(m_Shaders[eShaderType::DOMAINS]);
		context.SetComputeShader(m_Shaders[eShaderType::COMPUTE]);
	}

	context.VSSetShaderResource(0, TextureSlot::_COUNT, m_Resources);
	context.PSSetShaderResource(0, TextureSlot::_COUNT, m_Resources);
	context.DSSetShaderResource(REGISTER_7, 1, &m_Resources[REGISTER_7]);
}

void Effect::Clear()
{
	void* resources[TextureSlot::_COUNT] = { };
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	context.VSSetShaderResource(0, TextureSlot::_COUNT, resources);
	context.PSSetShaderResource(0, TextureSlot::_COUNT, resources);

	if (set_shaders)
	{
		graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
		CompiledShader* null = nullptr;
		context.SetVertexShader(null);
		context.SetPixelShader(null);
		context.SetGeometryShader(null);
		context.SetHullShader(null);
		context.SetDomainShader(null);
		context.SetComputeShader(null);
	}
}

void Effect::Reload(CompiledShader* shader)
{
	m_Shaders[shader->m_Type] = shader;
}
