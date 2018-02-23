#include "stdafx.h"
#include "Effect.h"
#include <Engine/IGraphicsContext.h>

Effect::Effect(const std::string& filepath)
	: m_FileName(filepath)
{
}

void Effect::AddShaderResource(IShaderResourceView* pResource, s32 slot)
{
	m_Resources[slot] = pResource;
}

void Effect::AddShaderResource(Texture* pResource, s32 slot)
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
	context.VSSetShaderResource(0, _COUNT, m_Resources);
	context.PSSetShaderResource(0, _COUNT, m_Resources);
	context.DSSetShaderResource(REGISTER_7, 1, &m_Resources[REGISTER_7]);
}

void Effect::Clear()
{
	void* resources[_COUNT] = { };
	graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
	context.VSSetShaderResource(0, _COUNT, resources);
	context.PSSetShaderResource(0, _COUNT, resources);
}
