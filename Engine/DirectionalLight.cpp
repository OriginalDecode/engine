#include "stdafx.h"
#include "DirectionalLight.h"



void DirectionalLight::Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor)
{
	m_Color = aColor;
	m_OriginalDirection = aDirection;
	m_Direction = aDirection;
	m_Orientation.SetPosition(aPosition);
}

void DirectionalLight::SetColor(const CU::Vector4f& aColor)
{
	m_Color = aColor;
}

void DirectionalLight::SetDirection(const CU::Vector3f& aDirection)
{
	m_Direction = aDirection;
	m_OriginalDirection = m_Direction;
}

void DirectionalLight::Update()
{
	m_Direction = m_OriginalDirection * m_Orientation;
}

void DirectionalLight::Rotate(const eLightAxis& anAxis, float aRotationOverTime)
{
	switch (anAxis)
	{
		case eLightAxis::XAxis:
		m_Orientation *= CU::Matrix44f::CreateRotateAroundX(aRotationOverTime);
		break;
		case eLightAxis::YAxis:
		m_Orientation *= CU::Matrix44f::CreateRotateAroundY(aRotationOverTime);
		break;
		case eLightAxis::ZAxis:
		m_Orientation *= CU::Matrix44f::CreateRotateAroundZ(aRotationOverTime);
		break;
		case eLightAxis::NONE:
		break;
	}
}

