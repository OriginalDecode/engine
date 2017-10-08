#pragma once
#include "LightStructs.h"

class DirectionalLight
{
public:

	DirectionalLight() = default;

	void Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor);
	void SetColor(const CU::Vector4f& aColor);
	void SetDirection(const CU::Vector3f& aDirection);

	void Update();
	void Rotate(const eLightAxis& anAxis, float aRotationOverTime);
private:

	CU::Matrix44f m_Orientation;

	CU::Vector4f m_Color;
	CU::Vector3f m_Direction;
	CU::Vector3f m_OriginalDirection;
};

