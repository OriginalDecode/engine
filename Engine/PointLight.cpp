#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"

CPointLight::CPointLight()
{
	Engine::GetInstance()->LoadModel("Data/Model/lightMeshes/sphere.fbx", "Shaders/T_Deferred_Lightmesh.json", false);
	m_Model = Engine::GetInstance()->GetModel("Data/Model/lightMeshes/sphere.fbx");
}

void CPointLight::SetPosition(const CU::Vector3f& aPosition)
{
	myOriginalPosition = aPosition;
	myOrientation.SetPosition(myOriginalPosition);
	m_Model->SetPosition(aPosition);
}

CU::Vector3f CPointLight::GetPosition()
{
	return myOrientation.GetPosition();
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

void CPointLight::Render(const CU::Matrix44f& previousOrientation, Camera* camera)
{
	//m_Model->Render(previousOrientation, camera->GetPerspective());
}

const SPointlightData& CPointLight::GetData() const
{
	return myData;
}

CU::Matrix44f CPointLight::GetOrientation()
{
	return myOrientation;
}

