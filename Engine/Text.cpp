#include "stdafx.h"
#include "Text.h"
#include "Font.h"
#include "Effect.h"
#include "Camera.h"
namespace Snowblind
{
	CText::CText(const char* aFilepath, int aSize, CCamera* aCamera)
	{
		myCamera = aCamera;
		myFont = CEngine::GetInstance()->LoadFont(aFilepath, aSize);
	}


	CText::~CText()
	{
		delete myFont;
		myFont = nullptr;
	}

	void CText::Render()
	{
		myFont->GetEffect()->SetPosition(myPosition);
		myFont->GetEffect()->SetMatrices(myOrientation, myCamera->GetOrientation(), myCamera->GetOrthogonalMatrix());
		myFont->Render();
	}

	void CText::SetText(const std::string& aString)
	{
		myFont->SetText(aString);
	}

	void CText::SetPosition(const CU::Math::Vector2<float>& aPosition)
	{
		myPosition = aPosition;
	}

	void CText::SetScale(const CU::Math::Vector2<float>& aScale)
	{
		myFont->GetEffect()->SetScale(aScale);
		myScale = aScale;
	}

	const CU::Math::Vector2<float>& CText::GetScale()
	{
		return myScale;
	}

	ID3D11ShaderResourceView* CText::GetAtlas()
	{
		return myFont->GetAtlas();
	}

};