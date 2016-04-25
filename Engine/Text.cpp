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
		myScale = { 1, 1 };
	}


	CText::~CText()
	{
		delete myFont;
		myFont = nullptr;
	}

	void CText::Render()
	{
		myFont->GetEffect()->SetPosition(myPosition);
		myFont->GetEffect()->SetScale(myScale);
		myFont->GetEffect()->SetMatrices(myOrientation, myCamera->GetOrientation(), myCamera->GetOrthogonalMatrix());
		myFont->Render();
	}

	void CText::SetText(const std::string& aString)
	{
		myFont->SetText(aString);
	}

	void CText::SetPosition(const CU::Math::Vector2<float>& aPosition)
	{
		CU::Math::Vector2<float> pos = aPosition;
		pos.x = pos.x - (CEngine::GetInstance()->GetWindowSize().myWidth * 0.5f) + 2;
		pos.y = -pos.y + (CEngine::GetInstance()->GetWindowSize().myHeight * 0.5f) - (myFont->GetFontPixelSize() + 4);
		myPosition = pos;
	}

	void CText::SetScale(const CU::Math::Vector2<float>& aScale)
	{
		myScale = aScale;// * 0.36f; //magic?
	}

	const CU::Math::Vector2<float>& CText::GetScale()
	{
		return myScale;
	}

	ID3D11ShaderResourceView* CText::GetAtlas()
	{
		return myFont->GetAtlas();
	}

	void CText::operator>>(const std::string& aString)
	{
		myFont->SetText(aString);
	}

	float CText::GetRenderTime()
	{
		return myFont->GetRenderTime();
	}

	float CText::GetUpdateTime()
	{
		return myFont->GetUpdateTime();
	}

};