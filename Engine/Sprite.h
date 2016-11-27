#pragma once
#include <Math/Matrix/Matrix.h>
struct ID3D11ShaderResourceView;

namespace Snowblind
{
	class CCamera;
	class CSpriteModel;

	class CSprite
	{
	public:
		CSprite();
		~CSprite();
		void Initiate(const std::string& aTexturePath, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);
		void Initiate(ID3D11ShaderResourceView* aShaderResource, const CU::Math::Vector2<float>& aSize, const CU::Math::Vector2<float>& aPosition);

		void Render(CCamera* aCamera);
		const CU::Math::Vector2<float>& GetPosition();
		CU::Math::Vector2<float> GetSize();

		void SetPosition(const CU::Math::Vector2<float>& aPosition);
		void SetHotspot(const CU::Math::Vector2<float>& aHotspot);
		void SetSize(const CU::Math::Vector2<float>& aSize);
		void SetScale(const CU::Math::Vector2<float>& aScale);
		void SetShaderView(ID3D11ShaderResourceView* srv);
	private:

		CSpriteModel* mySprite;
		CU::Math::Matrix44<float> myOrientation;
		CU::Math::Vector2<float> myHotspot;
		CU::Math::Vector2<float> myPosition;
	};
};