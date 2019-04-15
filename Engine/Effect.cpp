#include "stdafx.h"
#include "Effect.h"
#include <Engine/IGraphicsContext.h>
#include <Engine/Texture.h>

Effect::Effect()
{
	for (int i = 0; i < eShaderType::NOF_TYPES; i++)
		m_Shaders[i] = nullptr;

	for (int i = 0; i < TextureSlot::SLOT_COUNT; i++)
		m_Resources[i] = nullptr;
}


Effect::Effect(const std::string& filepath)
	: m_FileName(filepath)
{
	for (int i = 0; i < eShaderType::NOF_TYPES; i++)
		m_Shaders[i] = nullptr;

	for (int i = 0; i < TextureSlot::SLOT_COUNT; i++)
		m_Resources[i] = nullptr;
}

void Effect::AddShaderResource(IShaderResourceView* pResource, int32 slot)
{
	m_Resources[slot] = pResource;
}

void Effect::AddShaderResource(Texture* pResource, int32 slot)
{
	AddShaderResource(pResource->GetShaderView(), slot);
}

void Effect::Use()
{
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	if (set_shaders)
	{
		context.SetVertexShader(m_Shaders[EShaderType_VERTEX]);
		context.SetPixelShader(m_Shaders[EShaderType_PIXEL]);
		context.SetGeometryShader(m_Shaders[EShaderType_GEOMETRY]);
		context.SetHullShader(m_Shaders[EShaderType_HULL]);
		context.SetDomainShader(m_Shaders[EShaderType_DOMAIN]);
		context.SetComputeShader(m_Shaders[EShaderType_COMPUTE]);
	}

	context.VSSetShaderResource(0, TextureSlot::SLOT_COUNT, m_Resources);
	context.PSSetShaderResource(0, TextureSlot::SLOT_COUNT, m_Resources);
	context.DSSetShaderResource(REGISTER_7, 1, &m_Resources[REGISTER_7]);
}

void Effect::Clear()
{
	void* resources[TextureSlot::SLOT_COUNT] = { };
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	context.VSSetShaderResource(0, TextureSlot::SLOT_COUNT, resources);
	context.PSSetShaderResource(0, TextureSlot::SLOT_COUNT, resources);

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
