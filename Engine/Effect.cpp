#include "stdafx.h"
#include "Effect.h"

Effect::Effect(const std::string& aFilePath)
	: m_FileName(aFilePath)
{
}

void Effect::AddShaderResource(IShaderResourceView* pResource, TextureSlot slot)
{
	m_Resources[slot] = pResource;
}

void Effect::AddShaderResource(Texture* pResource, TextureSlot slot)
{
	AddShaderResource(pResource->GetShaderView(), slot);
}

void Effect::Use()
{
	graphics::IGraphicsAPI* api = Engine::GetAPI();
	api->SetVertexShader(m_VertexShader);
	api->SetPixelShader(m_PixelShader);
	api->SetGeometryShader(m_GeometryShader);
	api->SetHullShader(m_HullShader);
	api->SetDomainShader(m_DomainShader);
	api->SetComputeShader(m_ComputeShader);
	api->PSSetShaderResource(0, _COUNT, m_Resources);
}

void Effect::Clear()
{
	graphics::IShaderResourceView* resources[_COUNT] = {};
	graphics::IGraphicsAPI* api = Engine::GetAPI();
	api->PSSetShaderResource(0, _COUNT, m_Resources);
}
