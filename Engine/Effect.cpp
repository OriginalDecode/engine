#include "stdafx.h"
#include "Effect.h"
#include <d3dcompiler.h>
namespace Snowblind
{
	CEffect::CEffect(const std::string& aFilePath)
		: myFileName(aFilePath)
	{
	}

	CEffect::~CEffect()
	{
	}

	void CEffect::Activate()
	{
		//myContext->SetVertexShader();
	}

}