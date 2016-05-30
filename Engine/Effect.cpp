#include "stdafx.h"
#include "Effect.h"
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

		myTechniqueName = "";
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


		ID3D11Device* device = nullptr;
		device = CEngine::GetDirectX()->GetDevice();

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

	}

	void CEffect::SetMatrices(CU::Matrix44f& aToWorld, CU::Matrix44f& aToView, CU::Matrix44f& aProjection)
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
		//mySpritePos = myEffect->GetVariableByName("Position")->AsVector();
		//Validate(mySpritePos, "SpritePosition Invalid!");
		GetShaderResource(mySpritePos, "Position");
		mySpritePos->SetFloatVector(&aPosition.x);
	}

	void CEffect::SetScale(const CU::Math::Vector2<float>& aPosition)
	{
		//mySpritePos = myEffect->GetVariableByName("Scale")->AsVector();
		//Validate(mySpritePos, "SpritePosition Invalid!");
		GetShaderResource(mySpritePos, "Scale");
		mySpritePos->SetFloatVector(&aPosition.x);
	}

	void CEffect::SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask)
	{
		CEngine::GetDirectX()->GetContext()->OMSetBlendState(aBlendState, aBlendFactor, aSampleMask);
	}

	void CEffect::SetViewMatrix(const CU::Matrix44f& aViewMatrix)
	{
		myViewMatrix->SetMatrix(&aViewMatrix.myMatrix[0]);
	}

	void CEffect::SetProjectionMatrix(const CU::Matrix44f& aProjectionMatrix)
	{
		myProjectionMatrix->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}

	void CEffect::SetWorldMatrix(const CU::Matrix44f& aWorldMatrix)
	{
		myWorldMatrix->SetMatrix(&aWorldMatrix.myMatrix[0]);
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

	void CEffect::GetShaderResource(ID3DX11EffectShaderResourceVariable*& aShaderResource, const std::string& aVariableName)
	{
		aShaderResource = GetVariableByName(aVariableName.c_str())->AsShaderResource();
		const std::string validationString(aVariableName + " was not found. Check shader/code");
		Validate(aShaderResource, validationString);
	}

	void CEffect::GetShaderResource(ID3DX11EffectVectorVariable*& aShaderResource, const std::string& aVariableName)
	{
		aShaderResource = GetVariableByName(aVariableName.c_str())->AsVector();
		const std::string validationString(aVariableName + " was not found. Check shader/code");
		Validate(aShaderResource, validationString);
	}

	void CEffect::GetShaderResource(ID3DX11EffectMatrixVariable*& aShaderResource, const std::string& aVariableName)
	{
		aShaderResource = GetVariableByName(aVariableName.c_str())->AsMatrix();
		const std::string validationString(aVariableName + " was not found. Check shader/code");
		Validate(aShaderResource, validationString);
	}

	ID3DX11EffectTechnique* CEffect::GetTechnique()
	{
		if (myTechniqueName != "")
		{
			return myEffect->GetTechniqueByName(myTechniqueName.c_str());
		}

		return myTechnique;
	}

	ID3DX11EffectVariable* CEffect::GetVariableByName(const std::string& variableName)
	{
		return myEffect->GetVariableByName(variableName.c_str());
	}

	void CEffect::SetTechniqueName(const std::string& aTechniqueName)
	{
		myTechniqueName = aTechniqueName;
	}

	void CEffect::SetAlbedo(CTexture* aTexturePtr)
	{
		GetShaderResource(myTexture, "AlbedoTexture");
		myTexture->SetResource(aTexturePtr->GetShaderView());
	}

	void CEffect::SetAlbedo(ID3D11ShaderResourceView* aTexturePtr)
	{
		GetShaderResource(myTexture, "AlbedoTexture");
		myTexture->SetResource(aTexturePtr);
	}

	void CEffect::SetTexture(CTexture* texturePtr)
	{
		GetShaderResource(myTexture, "DiffuseTexture");
		myTexture->SetResource(texturePtr->GetShaderView());
	}

	void CEffect::SetTexture(ID3D11ShaderResourceView* aTexturePtr, const std::string& textureName)
	{
		GetShaderResource(myTexture, textureName);
		myTexture->SetResource(aTexturePtr);
	}

}