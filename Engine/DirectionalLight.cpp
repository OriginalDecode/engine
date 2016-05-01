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
		myOriginalPosition = aPosition;
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

	void CDirectionalLight::SetPosition(const CU::Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
		myOriginalPosition = myOrientation.GetPosition();
	}

	void CDirectionalLight::Update()
	{
		myDirection = myOriginalDirection * myOrientation;
	}

};