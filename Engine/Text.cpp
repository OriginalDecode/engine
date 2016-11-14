#include "stdafx.h"
#include "Text.h"
#include "Font.h"
#include "Effect.h"
#include "Camera.h"
namespace Snowblind
{
	CText::CText(const char* aFilepath, int aSize, int aBorderWidth)
	{
		myFont = CEngine::GetInstance()->LoadFont(aFilepath, aSize, aBorderWidth);
		myScale = { 1, 1 };
	}

	CText::~CText()
	{
		SAFE_DELETE(myFont);
	}

	void CText::Render(CCamera* aCamera)
	{
		myFont->SetPosition(myPosition);
		myFont->SetScale(myScale);
		myFont->SetMatrices(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonalMatrix());
		myFont->Render();
	}

	void CText::SetText(const std::string& aString)
	{
		myFont->SetText(aString);
	}

	void CText::SetPosition(const CU::Math::Vector2<float>& aPosition)
	{
		float w = CEngine::GetInstance()->GetWindowSize().myWidth;
		float h = CEngine::GetInstance()->GetWindowSize().myHeight;
		CU::Math::Vector2<float> pos(-(w * 0.5f), (h * 0.5f) - (myFont->GetFontPixelSize()));
		//pos.x = pos.x - (CEngine::GetInstance()->GetWindowSize().myWidth * 0.5f);
		//pos.y = -pos.y + (CEngine::GetInstance()->GetWindowSize().myHeight * 0.5f) - (myFont->GetFontPixelSize());
		pos.x = pos.x + (w * aPosition.x);
		pos.y = pos.y - (h * aPosition.y);
		myPosition = pos;
	}

	void CText::SetScale(const CU::Math::Vector2<float>& aScale)
	{
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

	float CText::GetRenderTime()
	{
		return myFont->GetRenderTime();
	}

	float CText::GetUpdateTime()
	{
		return myFont->GetUpdateTime();
	}
};


