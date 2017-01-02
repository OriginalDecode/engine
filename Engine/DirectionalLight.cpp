#include "stdafx.h"
#include "DirectionalLight.h"

namespace Hex
{
	
	void DirectionalLight::Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor)
	{
		myColor = aColor;
		myOriginalDirection = aDirection;
		myDirection = aDirection;
		myOrientation.SetPosition(aPosition);
	}

	void DirectionalLight::SetColor(const CU::Vector4f& aColor)
	{
		myColor = aColor;
	}

	void DirectionalLight::SetDirection(const CU::Vector3f& aDirection)
	{
		myDirection = aDirection;
		myOriginalDirection = myDirection;
	}

	void DirectionalLight::Update()
	{
		myDirection = myOriginalDirection * myOrientation;
	}

	void DirectionalLight::Rotate(const eLightAxis& anAxis, float aRotationOverTime)
	{
		switch (anAxis)
		{
		case eLightAxis::XAxis:
			myOrientation *= CU::Matrix44f::CreateRotateAroundX(aRotationOverTime);
			break;
		case eLightAxis::YAxis:
			myOrientation *= CU::Matrix44f::CreateRotateAroundY(aRotationOverTime);
			break;		
		case eLightAxis::ZAxis:
			myOrientation *= CU::Matrix44f::CreateRotateAroundZ(aRotationOverTime);
			break;
		case eLightAxis::NONE:
			break;
		}
	}

};