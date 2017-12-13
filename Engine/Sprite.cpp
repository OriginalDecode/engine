#include "stdafx.h"
#include "Sprite.h"
#include "Effect.h"
#include <Engine/Quad.h>

Sprite::~Sprite()
{
	SAFE_DELETE(m_Quad);
}

void Sprite::Initiate(const std::string& /*aTexturePath*/, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
{
	m_Quad = new Quad(Engine::GetInstance()->GetEffect("Shaders/sprite.json"), aSize.x, aSize.y);

	m_cbSprite = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbSprite), "Sprite ConstantBuffer");
	SetPosition(aPosition);
}

//void Sprite::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
//{
//	mySprite = new SpriteModel();
//	mySprite->Initiate(aShaderResource, aSize, aPosition);
//}

void Sprite::Initiate(const std::string& /*path*/)
{
	//m_cbSprite = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(cbSprite), path + " Sprite - ConstantBuffer");
// 	m_Quad = new Quad(0, 1, 1);
// 	mySprite->Initiate(path);
}

void Sprite::Render(Camera* /*aCamera*/)
{
	//mySprite->GetEffect()->SetScale({ 1, 1 });
	//mySprite->GetEffect()->SetPosition(myPosition);
	//mySprite->GetEffect()->SetMatrices(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonalMatrix());
	UpdateConstantBuffer();
	m_Quad->Render(false);
	//mySprite->Render(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonal());
}

const CU::Vector2f& Sprite::GetPosition()
{
	return myPosition;
}

CU::Vector2f Sprite::GetSize()
{
	return{ 0,0 };//mySprite->GetSize();
}

void Sprite::SetPosition(const CU::Math::Vector2<float>& aPosition)
{
	CU::Math::Vector2<float> pos = aPosition;
	pos.x = pos.x + myHotspot.x;
	pos.y = -pos.y - myHotspot.y;
	pos.x = pos.x - (Engine::GetInstance()->GetInnerSize().m_Width * 0.5f);
	pos.y = pos.y + (Engine::GetInstance()->GetInnerSize().m_Height * 0.5f);
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

void Sprite::UpdateConstantBuffer()
{
	Engine::GetAPI()->GetContext().UpdateConstantBuffer(m_cbSprite, &m_cbStruct, sizeof(cbSprite));
}

//void Sprite::SetShaderView(ID3D11ShaderResourceView* srv)
//{
//	mySprite->SetTexture(srv);
//}
//
