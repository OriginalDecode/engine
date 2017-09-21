#include "stdafx.h"
#include "Effect.h"
#include <Engine/IGraphicsContext.h>

Effect::Effect(const std::string& filepath)
	: m_FileName(filepath)
{
}

void Effect::AddShaderResource(void* pResource, TextureSlot slot)
{
	m_Resources[slot] = pResource;
}

void Effect::AddShaderResource(Texture* pResource, TextureSlot slot)
{
	AddShaderResource(pResource->GetShaderView(), slot);
}

void Effect::Use()
{
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	context.SetVertexShader(m_VertexShader);
	context.SetPixelShader(m_PixelShader);
	context.SetGeometryShader(m_GeometryShader);
	context.SetHullShader(m_HullShader);
	context.SetDomainShader(m_DomainShader);
	context.SetComputeShader(m_ComputeShader);
	context.PSSetShaderResource(0, _COUNT, m_Resources);
}

void Effect::Clear()
{
	void* resources[_COUNT] = {};
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	context.PSSetShaderResource(0, _COUNT, m_Resources);
}
