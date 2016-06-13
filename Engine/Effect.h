#pragma once
#include <string>
#include <DL_Debug.h>
#include <Math/Matrix/Matrix44.h>
#include "LightStructs.h"
#include <DataStructures/StaticArray.h>

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3D11ShaderResourceView;
struct ID3DX11EffectVectorVariable;
struct ID3D11BlendState;

namespace Snowblind
{
	class CTexture;
	class CEffect
	{
	public:
		CEffect();
		~CEffect();

		void Initiate(const std::string& aFile);

		ID3DX11Effect* GetEffect();
		ID3DX11EffectTechnique* GetTechnique();
		ID3DX11EffectVariable* GetVariableByName(const std::string& variableName);

		void SetTechniqueName(const std::string& aTechniqueName);
		void SetAlbedo(CTexture* aTexturePtr);
		void SetAlbedo(ID3D11ShaderResourceView* aTexturePtr);

		void SetTexture(ID3D11ShaderResourceView* aTexturePtr, const std::string& textureName);
		void SetTexture(CTexture* texturePtr, const std::string& textureName);
		void SetDepthTexture(CTexture* texturePtr);


		void SetPosition(const CU::Math::Vector2<float>& aPosition);
		void SetScale(const CU::Math::Vector2<float>& aPosition);
		void SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask = 0xFFFFFFFF);

		void SetMatrices(CU::Matrix44f& aToWorld, CU::Matrix44f& aToView, CU::Matrix44f& aProjection);
		void SetViewMatrix(const CU::Matrix44f& aViewMatrix);
		void SetProjectionMatrix(const CU::Matrix44f& aProjectionMatrix);
		void SetWorldMatrix(const CU::Matrix44f& aWorldMatrix);

		void UpdateLight(CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE>& someData);
		void UpdateLight(CU::StaticArray<SSpotlightData, SPOTLIGHT_SIZE>& someData);
		void UpdateLight(CU::StaticArray<SPointlightData, POINTLIGHT_SIZE>& someData);

		void GetShaderResource(ID3DX11EffectShaderResourceVariable*& aShaderResource, const std::string& aVariableName);
		void GetShaderResource(ID3DX11EffectVectorVariable*& aShaderResource, const std::string& aVariableName);
		void GetShaderResource(ID3DX11EffectMatrixVariable*& aShaderResource, const std::string& aVariableName);
		void GetShaderResource(ID3DX11EffectVariable*& aShaderResource, const std::string& aVariableName);


		template <typename T>
		void Validate(T* anEffectVariable, const std::string& anErrorMessage);
	private:

		std::string myTechniqueName;

		ID3DX11Effect* myEffect;

		ID3DX11EffectTechnique* myTechnique;
		ID3DX11EffectMatrixVariable* myProjectionMatrix;
		ID3DX11EffectMatrixVariable* myViewMatrix;
		ID3DX11EffectMatrixVariable* myWorldMatrix;
		ID3DX11EffectShaderResourceVariable* myTexture;
		ID3DX11EffectShaderResourceVariable* myDepthTexture;

		ID3DX11EffectVectorVariable* mySpritePos;

		ID3DX11EffectVariable* myDirectionalLightData;
		ID3DX11EffectVariable* myPointLightData;
		ID3DX11EffectVariable* mySpotLightData;

	};

	__forceinline ID3DX11Effect* CEffect::GetEffect()
	{
		return myEffect;
	}

	template <typename T>
	void CEffect::Validate(T* anEffectVariable, const std::string& aString)
	{
		DL_ASSERT_EXP(anEffectVariable->IsValid() == TRUE, aString.c_str());
	}

}