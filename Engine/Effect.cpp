#include "stdafx.h"
#include "Effect.h"
namespace Snowblind
{
	Effect::Effect(const std::string& aFilePath)
		: myFileName(aFilePath)
	{
	
#ifdef SNOWBLIND_DX11
		myContext = Engine::GetAPI()->GetContext();
#endif
	}

	Effect::~Effect()
	{
	}

	//VertexShader* Effect::GetVertexShader()
	//{
	//	return myVertexShader;
	//}

//	PixelShader* Effect::GetPixelShader()
//	{
//#ifdef SNOWBLIND_DX11
//		if (myPixelShader != nullptr)
//		{
//			return myPixelShader;
//		}
//#endif
//		return nullptr;
//	}

	void Effect::Activate()
	{
#ifdef SNOWBLIND_DX11
		Engine::GetAPI()->SetVertexShader(m_VertexShader->m_Shader);
		Engine::GetAPI()->SetPixelShader(m_PixelShader->m_Shader);

		if (myShaderResources.Size() > 0)
		{
			myShaderResources.Optimize();
			Engine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myShaderResources[0]);
		}
#endif
	}

	void Effect::Deactivate()
	{
#ifdef SNOWBLIND_DX11
		Engine::GetAPI()->GetContext()->PSSetShaderResources(0, myShaderResources.Size(), &myNULLList[0]);
#endif
	}

	void Effect::AddShaderResource(ID3D11ShaderResourceView* aShaderResource)
	{
#ifdef SNOWBLIND_DX11
		myShaderResources.Add(aShaderResource);
		if (!firstOptimize)
			myNULLList.Add(nullptr);
#endif
	}
};