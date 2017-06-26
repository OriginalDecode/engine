#include "stdafx.h"
#include "Effect.h"

Effect::Effect(const std::string& aFilePath)
	: myFileName(aFilePath)
{
}

void Effect::SetPixelShader(CompiledShader* shader)
{
	m_PixelShader = shader;
}

void Effect::AddShaderResource(IShaderResourceView* aShaderResource)
{
	DL_ASSERT_EXP(aShaderResource, "Shader Resource was null. Check if initiated correctly.");
	myShaderResources.Add(aShaderResource);
	myNULLList.Add(nullptr);
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
	DirectX11* api = Engine::GetAPI();
	api->SetVertexShader(m_VertexShader);
	api->SetPixelShader(m_PixelShader);
	api->SetGeometryShader(m_GeometryShader);
	api->SetHullShader(m_HullShader);
	api->SetDomainShader(m_DomainShader);
	api->SetComputeShader(m_ComputeShader);
	api->GetContext()->PSSetShaderResources(0, _COUNT, m_Resources);
}

void Effect::Clear()
{
	IShaderResourceView* resources[_COUNT] = {	};
	DirectX11* api = Engine::GetAPI();
	api->GetContext()->PSSetShaderResources(0, _COUNT, resources);

}
