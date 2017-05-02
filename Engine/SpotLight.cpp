#include "stdafx.h"
#include <Engine/snowblind_shared.h>
#include "SpotLight.h"
#include "Instance.h"
#include "Camera.h"
SpotLight::SpotLight()
{
	m_Model = new LightModel;
	Engine::GetInstance()->LoadModel("Data/Model/lightMeshes/cone.fbx", "Shaders/T_Deferred_Spotlight.json", m_Model, false);
	m_Model->Initiate("cone.fbx");
	//m_Model = Engine::GetInstance()->GetModel("Data/Model/lightMeshes/cone.fbx");
}

void SpotLight::Render(const CU::Matrix44f& previousOrientation, Camera* aCamera, const RenderContext& render_context)
{
	m_Model->Render(previousOrientation, aCamera->GetPerspective(), render_context);
}

SSpotlightData& SpotLight::GetData()
{
	return myData;
}

void SpotLight::DoTranslation(const CU::Matrix44f& translationMatrix)
{
	myData.myOrientation = translationMatrix;
	SetDirection(myData.myOrientation.GetForward());
}

CU::Matrix44f SpotLight::GetOrientatino()
{
	return m_Model->GetOrientation();
}

void SpotLight::SetPosition(const CU::Vector3f& aPosition)
{
	m_Model->GetOrientation().SetPosition(aPosition);
	//myLightMesh->SetPosition(aPosition);
	myBaseMatrix.SetPosition(aPosition);
	myData.myLightPosition = aPosition;
	myData.myOrientation.SetPosition(aPosition);
}

void SpotLight::SetDirection(const CU::Vector4f& aDirection)
{
	myData.myDirection.x = aDirection.x;
	myData.myDirection.y = aDirection.y;
	myData.myDirection.z = aDirection.z;


	m_Model->GetOrientation().SetForward(myData.myDirection);
}

void SpotLight::SetAngle(float anAngle)
{
	myData.myAngle = anAngle;
}

void SpotLight::SetRange(float aRange)
{
	myData.myRange = aRange;
}

void SpotLight::SetColor(const CU::Vector4f& aColor)
{
	myData.myLightColor = aColor;
}

