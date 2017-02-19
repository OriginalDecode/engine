#include "stdafx.h"
#include "Effect.h"
namespace Hex
{
	Effect::Effect(const std::string& aFilePath)
		: myFileName(aFilePath)
	{
	
		//This should be API agnostic
		m_Context = Engine::GetAPI()->GetContext();
	}

	void Effect::Activate()
	{
		//The set vertex & pixel ... should be API agnostic.
		Engine::GetAPI()->SetVertexShader(m_VertexShader->m_Shader);
		Engine::GetAPI()->SetPixelShader(m_PixelShader->m_Shader);

		if (myShaderResources.Size() > 0)
		{
			myShaderResources.Optimize();
			m_Context->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
		}
	}

	void Effect::Deactivate()
	{
		m_Context->PSSetShaderResources(0, myShaderResources.Size(), &myNULLList[0]);
	}

	void Effect::AddShaderResource(IShaderResourceView* aShaderResource)
	{
		DL_ASSERT_EXP(aShaderResource, "Shader Resource was null. Check if initiated correctly.");
		myShaderResources.Add(aShaderResource);
		myNULLList.Add(nullptr);
	}
};