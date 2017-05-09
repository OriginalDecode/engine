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

void Effect::Activate()
{
	DirectX11* api = Engine::GetAPI();
	api->SetVertexShader(m_VertexShader->m_Shader);
	api->SetPixelShader(m_PixelShader->m_Shader);

	if ( myShaderResources.Empty() )
		return;
	
	myShaderResources.Optimize();
	api->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
}

void Effect::Deactivate()
{
	if (myNULLList.Size() > 0)
		Engine::GetAPI()->GetContext()->PSSetShaderResources(0, myNULLList.Size(), &myNULLList[0]);
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
	api->SetVertexShader(m_VertexShader->m_Shader);
	api->SetPixelShader(m_PixelShader->m_Shader);

	api->GetContext()->PSSetShaderResources(0, _COUNT, m_Resources);

}

void Effect::Clear()
{
	IShaderResourceView* resources[_COUNT] = {	};
	DirectX11* api = Engine::GetAPI();
	api->GetContext()->PSSetShaderResources(0, _COUNT, resources);

}
