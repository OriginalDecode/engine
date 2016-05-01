#pragma once


namespace Snowblind
{
	class CPointLight
	{
	public:
		CPointLight();
		~CPointLight();

		void Initiate(const CU::Vector3f& aPosition, const CU::Vector4f& aColor, float aRange);

		void SetPosition(const CU::Vector3f& aPosition);
		const CU::Vector3f& GetPosition();

		void SetColor(const CU::Vector4f& aColor);
		const CU::Vector4f& GetColor();
		
		void SetRange(float aRange);
		const float& GetRange();

	private:
		CU::Matrix44f myOrientation;
		CU::Vector3f myOriginalPosition;
		CU::Vector4f myColor;
		float myRange;

	};
};