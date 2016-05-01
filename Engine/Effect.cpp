#include "stdafx.h"

namespace Snowblind
{
	CEffect::CEffect()
	{
	}

	CEffect::~CEffect()
	{
		myEffect->Release();
		myEffect = nullptr;
	}

	void CEffect::Initiate(const std::string& aFile)
	{
		ENGINE_LOG("Loading %s", aFile.c_str());
		HRESULT hr;
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = 0;
		ID3D10Blob* compilationMessage = 0;

		hr = D3DX11CompileFromFile(aFile.c_str(), 0, 0, 0, "fx_5_0", shaderFlag,
			0, 0, &compiledShader, &compilationMessage, 0);

		DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! File not found!");


		ID3D11Device* device = CEngine::GetInstance()->GetAPI()->GetDevice();

		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &myEffect);
		switch (hr)
		{
		case D3D11_ERROR_FILE_NOT_FOUND:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! File not found!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Too many unique state objects!");
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Too many view objects!");
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Deferred Context Map Without Initial Discard!");
			break;
		case DXGI_ERROR_INVALID_CALL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Invalid Call");
			break;
		case DXGI_ERROR_WAS_STILL_DRAWING:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Were still drawing!");
			break;
		case E_FAIL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Failed!");
			break;
		case E_INVALIDARG:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! One or more arguments were invalid!");
			break;
		case E_OUTOFMEMORY:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! Out of Memory!");
			break;
		case E_NOTIMPL:
			DL_ASSERT_EXP(hr == S_OK, "Failed to Create effect! The method call isn't implemented with the passed parameter combination.");
			break;
		case S_FALSE:
			break;
		}
		ENGINE_LOG("Successfully created effect.");

		Validate(myEffect, "Effect Invalid!");

		myTechnique = myEffect->GetTechniqueByName("Render");
		Validate(myTechnique, "Technique Invalid!");

		myWorldMatrix = myEffect->GetVariableByName("World")->AsMatrix();
		Validate(myWorldMatrix, "World Matrix Invalid!");

		myViewMatrix = myEffect->GetVariableByName("View")->AsMatrix();
		Validate(myViewMatrix, "View Matrix Invalid!");

		myProjectionMatrix = myEffect->GetVariableByName("Projection")->AsMatrix();
		Validate(myProjectionMatrix, "Projection Matrix Invalid!");

		myDirectionalLightData = myEffect->GetVariableByName("DirectionalLight");
		//Validate(myDirectionalLightData);

		myPointLightData = myEffect->GetVariableByName("PointLight");
		//Validate(myPointLightData);

		mySpotLightData = myEffect->GetVariableByName("SpotLight");
		//Validate(mySpotLightData);



	}

	void CEffect::SetMatrices(CU::Math::Matrix44<float>& aToWorld, CU::Math::Matrix44<float>& aToView, CU::Math::Matrix44<float>& aProjection)
	{
		CU::Math::Matrix44<float> temp = aToWorld;

		HRESULT hr = myWorldMatrix->SetMatrix(static_cast<float*>(&temp.myMatrix[0]));
		DL_ASSERT_EXP(hr == S_OK, "Failed to set world matrix!");
		temp = CU::Math::Inverse(aToView);

		hr = myViewMatrix->SetMatrix(static_cast<float*>(&temp.myMatrix[0]));
		DL_ASSERT_EXP(hr == S_OK, "Failed to set view matrix!");

		hr = myProjectionMatrix->SetMatrix(static_cast<float*>(&aProjection.myMatrix[0]));
		DL_ASSERT_EXP(hr == S_OK, "Failed to set projection matrix!");
	}

	void CEffect::SetPosition(const CU::Math::Vector2<float>& aPosition)
	{
		mySpritePos = myEffect->GetVariableByName("Position")->AsVector();
		Validate(mySpritePos, "SpritePosition Invalid!");

		mySpritePos->SetFloatVector(&aPosition.x);
	}

	void CEffect::SetScale(const CU::Math::Vector2<float>& aPosition)
	{
		mySpritePos = myEffect->GetVariableByName("Scale")->AsVector();
		Validate(mySpritePos, "SpritePosition Invalid!");

		mySpritePos->SetFloatVector(&aPosition.x);
	}

	void CEffect::UpdateLight(CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE>& someData)
	{
		if (myEffect == nullptr)
			return;

		if (myDirectionalLightData != nullptr)
		{
			myDirectionalLightData->SetRawValue(&someData[0], 0, sizeof(SDirectionallightData) * DIRECTIONAL_SIZE);
		}
	}

	void CEffect::UpdateLight(CU::StaticArray<SSpotlightData, SPOTLIGHT_SIZE>& someData)
	{
		if (myEffect == nullptr)
			return;

		if (mySpotLightData != nullptr)
		{
			mySpotLightData->SetRawValue(&someData[0], 0, sizeof(SSpotlightData) * SPOTLIGHT_SIZE);
		}
	}

	void CEffect::UpdateLight(CU::StaticArray<SPointlightData, POINTLIGHT_SIZE>& someData)
	{
		if (myEffect == nullptr)
			return;

		if (myPointLightData != nullptr)
		{
			myPointLightData->SetRawValue(&someData[0], 0, sizeof(SPointlightData) * POINTLIGHT_SIZE);
		}
	}

	void CEffect::SetAlbedo(CTexture* aTexturePtr)
	{
		myTexture = myEffect->GetVariableByName("AlbedoTexture")->AsShaderResource();
		Validate(myTexture, "Texture Invalid");

		myTexture->SetResource(aTexturePtr->GetShaderView());
	}

	void CEffect::SetAlbedo(ID3D11ShaderResourceView* aTexturePtr)
	{
		myTexture = myEffect->GetVariableByName("AlbedoTexture")->AsShaderResource();
		Validate(myTexture, "Texture Invalid");

		myTexture->SetResource(aTexturePtr);
	}

}