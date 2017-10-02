#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"

PointLight::PointLight()
{
	m_Model = new LightModel;
	cl::HashString key = Engine::GetInstance()->LoadModel<LightModel>("Data/Model/lightMeshes/sphere.fbx",
																	  "Shaders/deferred_pointlight.json", 
																	  false);
	m_Model = static_cast<LightModel*>(Engine::GetInstance()->GetModel(key));

	//m_Model = Engine::GetInstance()->GetModel("Data/Model/lightMeshes/sphere.fbx");
}

void PointLight::SetPosition(const CU::Vector3f& aPosition)
{
	myOriginalPosition = aPosition;
	myOrientation.SetPosition(myOriginalPosition);
	m_Model->SetPosition(aPosition);
}

CU::Vector3f PointLight::GetPosition()
{
	return myOrientation.GetPosition();
}

void PointLight::SetColor(const CU::Vector4f& aColor)
{
	myColor = aColor;
}

const CU::Vector4f& PointLight::GetColor() 
{
	return myColor;
}

void PointLight::SetRange(float aRange)
{
	myRange = aRange;
}

const float& PointLight::GetRange()
{
	return myRange;
}

void PointLight::Update()
{
	myData.myLightColor = GetColor();
	myData.myLightPosition = GetPosition();
	myData.myRange = GetRange();
}

void PointLight::Render(const CU::Matrix44f& camera_orientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context)
{
 	//render_context.m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
 	//render_context.m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 1);
 	m_Model->Render(camera_orientation, camera_projection, render_context);
}

const SPointlightData& PointLight::GetData() const
{
	return myData;
}

CU::Matrix44f PointLight::GetOrientation()
{
	return myOrientation;
}

