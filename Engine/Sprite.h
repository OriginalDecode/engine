#pragma once
#include <Math/Matrix/Matrix.h>

namespace Snowblind
{
	class CCamera;
	class CSpriteModel;

	class CSprite
	{
	public:
		CSprite();
		~CSprite();
		void Initiate(const char* aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Render(CCamera* aCamera);
		const CU::Math::Vector2<float>& GetPosition();
		const CU::Math::Vector2<float>& GetSize();

		void SetPosition(const CU::Math::Vector2<float>& aPosition);
		void SetHotspot(const CU::Math::Vector2<float>& aHotspot);
		void SetSize(const CU::Math::Vector2<float>& aSize);
	private:

		CSpriteModel* mySprite;
		CU::Math::Matrix44<float> myOrientation;
		CU::Math::Vector2<float> myHotspot;
	};
};