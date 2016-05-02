#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"
#include "Instance.h"
namespace Snowblind
{
	CPointLight::CPointLight()
	{
	}


	CPointLight::~CPointLight()
	{
	}

	void CPointLight::Initiate(const CU::Vector3f& aPosition, const CU::Vector4f& aColor, float aRange)
	{
		myOriginalPosition = aPosition;
		myOrientation.SetPosition(myOriginalPosition);
		myColor = aColor;
		myRange = aRange;

#ifdef _DEBUG
		CModel* lightCube = new CModel();
		lightCube->CreateCube("Data/Shaders/Cube.fx", { aColor.r, aColor.g, aColor.b });
		myInstance = new CInstance();
		myInstance->Initiate(lightCube);
		myInstance->SetPosition(aPosition);
#endif

	}

	void CPointLight::SetPosition(const CU::Vector3f& aPosition)
	{
		myOriginalPosition = aPosition;
		myOrientation.SetPosition(myOriginalPosition);
	}

	const CU::Vector3f& CPointLight::GetPosition()
	{
		return myOriginalPosition;
	}

	void CPointLight::SetColor(const CU::Vector4f& aColor)
	{
		myColor = aColor;
	}

	const CU::Vector4f& CPointLight::GetColor()
	{
		return myColor;
	}

	void CPointLight::SetRange(float aRange)
	{
		myRange = aRange;
	}

	const float& CPointLight::GetRange()
	{
		return myRange;
	}
#ifdef _DEBUG
	CInstance* CPointLight::GetInstance()
	{
		return myInstance;
	}
#endif
};