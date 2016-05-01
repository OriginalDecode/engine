#include "stdafx.h"
#include "DirectionalLight.h"

namespace Snowblind
{
	CDirectionalLight::CDirectionalLight()
	{
	}


	CDirectionalLight::~CDirectionalLight()
	{
	}

	void CDirectionalLight::Initiate(const CU::Vector3f& aDirection, const CU::Vector3f& aPosition, const CU::Vector4f& aColor)
	{
		myColor = aColor;
		myOriginalDirection = aDirection;
		myDirection = aDirection;
		myOrientation.SetPosition(aPosition);
	}

	void CDirectionalLight::SetColor(const CU::Vector4f& aColor)
	{
		myColor = aColor;
	}

	void CDirectionalLight::SetDirection(const CU::Vector3f& aDirection)
	{
		myDirection = aDirection;
		myOriginalDirection = myDirection;
	}

	void CDirectionalLight::Update()
	{
		myDirection = myOriginalDirection * myOrientation;
	}

	void CDirectionalLight::Rotate(const eLightAxis& anAxis, float aRotationOverTime)
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