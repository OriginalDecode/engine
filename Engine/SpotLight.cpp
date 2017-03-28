#include "stdafx.h"
#include "SpotLight.h"
#include "Instance.h"
#include "Camera.h"

CSpotLight::CSpotLight()
{
	Engine::GetInstance()->LoadModel("Data/Model/lightMeshes/cone.fbx", "Data/Shaders/T_Deferred_Spotlight.json", false);
	m_Model = Engine::GetInstance()->GetModel("Data/Model/lightMeshes/cone.fbx");
	m_Model->SetIsLightmesh();
}

void CSpotLight::Render(const CU::Matrix44f& previousOrientation, Camera* aCamera)
{
	m_Model->Render(previousOrientation, aCamera->GetPerspective(), CU::Vector4f(1, 1, 1, 1));
}

SSpotlightData& CSpotLight::GetData()
{
	return myData;
}

void CSpotLight::DoTranslation(const CU::Matrix44f& translationMatrix)
{
	myData.myOrientation = translationMatrix;
	SetDirection(myData.myOrientation.GetForward());
}

CU::Matrix44f CSpotLight::GetOrientatino()
{
	return m_Model->GetOrientation();
}

void CSpotLight::SetPosition(const CU::Vector3f& aPosition)
{
	m_Model->GetOrientation().SetPosition(aPosition);
	//myLightMesh->SetPosition(aPosition);
	myBaseMatrix.SetPosition(aPosition);
	myData.myLightPosition = aPosition;
	myData.myOrientation.SetPosition(aPosition);
}

void CSpotLight::SetDirection(const CU::Vector4f& aDirection)
{
	myData.myDirection.x = aDirection.x;
	myData.myDirection.y = aDirection.y;
	myData.myDirection.z = aDirection.z;


	m_Model->GetOrientation().SetForward(myData.myDirection);
}

void CSpotLight::SetAngle(float anAngle)
{
	myData.myAngle = anAngle;
}

void CSpotLight::SetRange(float aRange)
{
	myData.myRange = aRange;
}

void CSpotLight::SetColor(const CU::Vector4f& aColor)
{
	myData.myLightColor = aColor;
}

