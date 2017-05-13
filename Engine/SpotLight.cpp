#include "stdafx.h"
#include "SpotLight.h"
#include "Instance.h"
#include "Camera.h"
#include <Engine/ShadowSpotlight.h>

void SpotLight::Initiate()
{
	std::string key = Engine::GetInstance()->LoadModel<LightModel>("Data/Model/lightMeshes/cone.fbx", "Shaders/T_Deferred_Spotlight.json", 0, false);
	m_Model = static_cast<LightModel*>(Engine::GetInstance()->GetModel(key));

	m_Model->Initiate("cone.fbx");
	m_ShadowSpotlight = new ShadowSpotlight;
	m_ShadowSpotlight->Initiate(2048.f);
	m_ShadowSpotlight->GetCamera()->RotateAroundX(CL::DegreeToRad(90.f));
}

void SpotLight::CleanUp()
{
	m_ShadowSpotlight->CleanUp();
	SAFE_DELETE(m_ShadowSpotlight);
}

void SpotLight::Render(const CU::Matrix44f& previousOrientation, Camera* aCamera, const RenderContext& render_context)
{
	render_context.m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
	m_Model->Render(previousOrientation, aCamera->GetPerspective(), render_context);
}

void SpotLight::SetData(const SpotlightData& data)
{
	myData = data;
	SetDirection(myData.myOrientation.GetForward());
	SetPosition(myData.myLightPosition);
	//m_ShadowSpotlight->SetAngle(myData.myAngle);
	const float buffer_size = m_ShadowSpotlight->GetBufferSize();
	//m_ShadowSpotlight->GetCamera()->RecalculatePerspective(buffer_size, buffer_size, 0.1f, myData.myRange);
}

const SpotlightData& SpotLight::GetData() const
{
	return myData;
}

void SpotLight::SetPosition(const CU::Vector3f& aPosition)
{
	m_Model->GetOrientation().SetPosition(aPosition);
	myData.myLightPosition = aPosition;
	myData.myOrientation.SetPosition(aPosition);
	m_ShadowSpotlight->GetCamera()->SetPosition(aPosition);
}

void SpotLight::SetDirection(const CU::Vector4f& aDirection)
{
	myData.myDirection.x = aDirection.x;
	myData.myDirection.y = aDirection.y;
	myData.myDirection.z = aDirection.z;
	m_Model->GetOrientation().SetForward(aDirection);
	//m_ShadowSpotlight->GetCamera()->SetAt(aDirection);
}