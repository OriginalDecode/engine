#pragma once


namespace Snowblind
{

#ifdef _DEBUG
	class CInstance;
#endif

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

#ifdef _DEBUG
		CInstance* GetInstance();
#endif

	private:

#ifdef _DEBUG
		CInstance* myInstance;
#endif

		CU::Matrix44f myOrientation;
		CU::Vector3f myOriginalPosition;
		CU::Vector4f myColor;
		float myRange;

	};
};