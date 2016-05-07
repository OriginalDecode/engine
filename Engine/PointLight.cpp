#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"
#include "Instance.h"
#include "AssetsContainer.h"

namespace Snowblind
{
	CPointLight::CPointLight()
	{
		myLightMesh = new Snowblind::CInstance();
		myLightMesh->Initiate("Data/Model/lightMeshes/sphere.fbx", "Data/Shaders/DeferredLightMesh.fx");
	}


	CPointLight::~CPointLight()
	{
		SAFE_DELETE(myLightMesh);
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
#endif

	}

	void CPointLight::SetPosition(const CU::Vector3f& aPosition)
	{
		myOriginalPosition = aPosition;
		myOrientation.SetPosition(myOriginalPosition);
		myLightMesh->SetPosition(aPosition);
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

	void CPointLight::Update()
	{
		myData.myLightColor = GetColor();
		myData.myLightPosition = GetPosition();
		myData.myRange = GetRange();
	}

	void CPointLight::Render(CCamera* camera)
	{
		myLightMesh->Render(*camera);
	}

	const SPointlightData& CPointLight::GetData() const
	{
		return myData;
	}

#ifdef _DEBUG
	CInstance* CPointLight::GetInstance()
	{
		return myInstance;
	}
#endif
};