#include "stdafx.h"
#include "Sprite.h"
#include "Effect.h"
#include "SpriteModel.h"


Sprite::Sprite()
{
}

Sprite::~Sprite()
{
	delete mySprite;
	mySprite = nullptr;
}

void Sprite::Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
{
	mySprite = new CSpriteModel();
	mySprite->Initiate(aTexturePath, aSize, aPosition);
	myHotspot.x = 0;
	myHotspot.y = 0;

}

void Sprite::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
{
	mySprite = new CSpriteModel();
	mySprite->Initiate(aShaderResource, aSize, aPosition);
}

void Sprite::Render(Camera* aCamera)
{
	//mySprite->GetEffect()->SetScale({ 1, 1 });
	//mySprite->GetEffect()->SetPosition(myPosition);
	//mySprite->GetEffect()->SetMatrices(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonalMatrix());
	mySprite->Render(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonal());
}

const CU::Math::Vector2<float>& Sprite::GetPosition()
{
	return mySprite->GetPosition();
}

CU::Math::Vector2<float> Sprite::GetSize()
{
	return mySprite->GetSize();
}

void Sprite::SetPosition(const CU::Math::Vector2<float>& aPosition)
{
	CU::Math::Vector2<float> pos = aPosition;
	pos.x = pos.x + myHotspot.x;
	pos.y = -pos.y - myHotspot.y;
	pos.x = pos.x - (Engine::GetInstance()->GetWindowSize().m_Width * 0.5f);
	pos.y = pos.y + (Engine::GetInstance()->GetWindowSize().m_Height * 0.5f);
	myPosition = pos;
	//mySprite->GetEffect()->SetPosition(myPosition);

	//float w = CEngine::GetInstance()->GetWindowSize().myWidth;
	//float h = CEngine::GetInstance()->GetWindowSize().myHeight;
	//CU::Math::Vector2<float> pos(-(w * 0.5f), (h * 0.5f));
	////pos.x = pos.x - (CEngine::GetInstance()->GetWindowSize().myWidth * 0.5f);
	////pos.y = -pos.y + (CEngine::GetInstance()->GetWindowSize().myHeight * 0.5f) - (myFont->GetFontPixelSize());
	//pos.x = pos.x + (w * aPosition.x);
	//pos.y = pos.y - (h * aPosition.y);
	//myPosition = pos;

	myOrientation.SetPosition(CU::Vector3f(myPosition));
}

void Sprite::SetHotspot(const CU::Math::Vector2<float>& aHotspot)
{
	myHotspot = aHotspot;
}

void Sprite::SetSize(const CU::Math::Vector2<float>& aSize)
{
	aSize;
	DL_ASSERT("Not implemented");
	//to be implemented
}

void Sprite::SetScale(const CU::Math::Vector2<float>& aScale)
{
	aScale;
	//mySprite->GetEffect()->SetScale(aScale);
}

void Sprite::SetShaderView(ID3D11ShaderResourceView* srv)
{
	mySprite->SetTexture(srv);
}

