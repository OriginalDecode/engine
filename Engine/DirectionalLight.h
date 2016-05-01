#pragma once
#include "LightStructs.h"
namespace Snowblind
{
	class CDirectionalLight
	{
	public:

		CDirectionalLight();
		~CDirectionalLight();

		void Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor);

		void SetColor(const CU::Vector4f& aColor);
		const CU::Vector4f& GetColor();

		void SetDirection(const CU::Vector3f& aDirection);
		const CU::Vector3f& GetDirection();
		const CU::Vector3f& GetOriginalDirection();

		void Update();
		void Rotate(const eLightAxis& anAxis, float aRotationOverTime);
	private:

		CU::Matrix44f myOrientation;
		CU::Vector4f myColor;
		CU::Vector3f myDirection;
		CU::Vector3f myOriginalDirection;
	};

	__forceinline const CU::Vector3f& CDirectionalLight::GetDirection()
	{
		return myDirection;
	}

	__forceinline const CU::Vector3f& CDirectionalLight::GetOriginalDirection()
	{
		return myOriginalDirection;
	}

	__forceinline const CU::Vector4f& CDirectionalLight::GetColor()
	{
		return myColor;
	}
};