#include "stdafx.h"
#include "Effect.h"
#include <d3dcompiler.h>
namespace Snowblind
{
	CEffect::CEffect()
	{
	}

	CEffect::~CEffect()
	{
	}

	void CEffect::Initiate(const std::string& aFile)
	{
		//Call the Shader Factory to create an effect?	
		//ShaderFactory-> ... no ... This seems dumb. I should be able to do this with just
	}


	void CEffect::Activate()
	{
		//myContext->SetVertexShader();
	}

}