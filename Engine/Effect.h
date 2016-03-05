#pragma once
#include <string>
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "../DL_Debug/DL_Debug.h"

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVariable;
struct ID3DX11EffectShaderResourceVariable;

namespace Snowblind
{
	class CTexture;
	class CEffect
	{
	public:
		CEffect();
		~CEffect();

		void Initiate(const std::string& aFile);
		void SetMatrices(Matrix44f& aToWorld, Matrix44f& aToView, Matrix44f& aProjection);

		ID3DX11Effect* GetEffect();
		ID3DX11EffectTechnique* GetTechnique();

		void SetAlbedo(CTexture* aTexturePtr);


		template <typename T>
		void Validate(T* anEffectVariable, const std::string& anErrorMessage);
	private:

		ID3DX11Effect* myEffect;

		ID3DX11EffectTechnique* myTechnique;

		ID3DX11EffectMatrixVariable* myProjectionMatrix;
		ID3DX11EffectMatrixVariable* myViewMatrix;
		ID3DX11EffectMatrixVariable* myWorldMatrix;
		ID3DX11EffectShaderResourceVariable* myTexture;
	};

	__forceinline ID3DX11Effect* CEffect::GetEffect()
	{
		return myEffect;
	}

	__forceinline ID3DX11EffectTechnique* CEffect::GetTechnique()
	{
		return myTechnique;
	}

	template <typename T>
	void CEffect::Validate(T* anEffectVariable, const std::string& aString)
	{
		DL_ASSERT_EXP(anEffectVariable->IsValid() == TRUE, aString.c_str());
	}

}