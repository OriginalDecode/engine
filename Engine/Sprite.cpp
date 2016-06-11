#include "stdafx.h"
#include "Sprite.h"
#include "Effect.h"
#include "SpriteModel.h"

namespace Snowblind
{
	CSprite::CSprite()
	{
	}

	CSprite::~CSprite()
	{
		delete mySprite;
		mySprite = nullptr;
	}

	void CSprite::Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
		mySprite = new CSpriteModel();
		mySprite->Initiate(aTexturePath, aSize, aPosition);
		myHotspot.x = 0;
		myHotspot.y = 0;
		
	}

	void CSprite::Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition)
	{
		mySprite = new CSpriteModel();
		mySprite->Initiate(aShaderResource, aSize, aPosition);
	}

	void CSprite::Render(CCamera* aCamera)
	{
		//mySprite->GetEffect()->SetScale({ 1, 1 });
		//mySprite->GetEffect()->SetPosition(myPosition);
		//mySprite->GetEffect()->SetMatrices(myOrientation, aCamera->Get2DOrientation(), aCamera->GetOrthogonalMatrix());
		//mySprite->Render();
	}

	const CU::Math::Vector2<float>& CSprite::GetPosition()
	{
		return mySprite->GetPosition();
	}

	const CU::Math::Vector2<float>& CSprite::GetSize()
	{
		return mySprite->GetSize();
	}

	void CSprite::SetPosition(const CU::Math::Vector2<float>& aPosition)
	{
		//CU::Math::Vector2<float> pos = aPosition;
		//pos.x = pos.x + myHotspot.x;
		//pos.y = -pos.y - myHotspot.y;
		//pos.x = pos.x - (CEngine::GetInstance()->GetWindowSize().myWidth * 0.5f);
		//pos.y = pos.y + (CEngine::GetInstance()->GetWindowSize().myHeight * 0.5f);
		//myPosition = pos;
		//mySprite->GetEffect()->SetPosition(myPosition);
	}

	void CSprite::SetHotspot(const CU::Math::Vector2<float>& aHotspot)
	{
		myHotspot = aHotspot;
	}

	void CSprite::SetSize(const CU::Math::Vector2<float>& aSize)
	{
		DL_ASSERT("Not implemented");
		//to be implemented
	}

	void CSprite::SetScale(const CU::Math::Vector2<float>& aScale)
	{
		//mySprite->GetEffect()->SetScale(aScale);
	}

};