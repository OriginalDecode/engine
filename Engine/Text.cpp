#include "stdafx.h"
#include "Text.h"
#include "Font.h"
#include "Effect.h"
#include "Camera.h"

CText::CText(const int8* filepath, uint16 aSize, uint16 aBorderWidth)
{
	myFont = Engine::GetInstance()->LoadFont(filepath, aSize, aBorderWidth);
	myScale = { 1, 1 };
}

CText::~CText()
{
	SAFE_DELETE(myFont);
}

void CText::Render(const graphics::RenderContext& rc)
{
	myFont->SetPosition(myPosition);
	myFont->SetScale(myScale);
	//myFont->SetMatrices(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonal());
	myFont->Render(rc);
}

void CText::SetText(std::string aString)
{
	myFont->SetText(aString);
}

void CText::SetPosition(const CU::Math::Vector2<float>& aPosition)
{
	myPosition = aPosition;
	return;


	const float w = (float)Engine::GetInstance()->GetInnerSize().m_Width;
	const float h = (float)Engine::GetInstance()->GetInnerSize().m_Height;
	CU::Vector2f pos(-(w * 0.5f), (h * 0.5f) - (myFont->GetFontSize()));
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

