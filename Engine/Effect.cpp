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

		ID3D11Device* device = CEngine::GetDirectX()->GetDevice();

		ENGINE_LOG("Loading %s", aFile.c_str());
		HRESULT hr;
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMessage = 0;

		std::wstring fileName(aFile.begin(), aFile.end());
		hr = D3DCompileFromFile(fileName.c_str(), NULL, NULL, NULL, NULL, shaderFlag, NULL, &compiledShader, &compilationMessage);
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to Compile Effect.");



	}



}