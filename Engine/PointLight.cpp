#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"

PointLight::PointLight()
{
	//m_Model = new LightModel;
	u64 key = Engine::GetInstance()->LoadModel<LightModel>("Data/Model/lightMeshes/sphere.fbx", "Shaders/deferred_pointlight.json", false);
	m_Model = Engine::GetInstance()->GetModel<Model>(key);
	LightModel* model = static_cast<LightModel*>(m_Model.GetData());
	model->Initiate("cone.fbx");



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

void PointLight::Render(const CU::Matrix44f&, const CU::Matrix44f&, const graphics::RenderContext& render_context)
{
	//render_context.m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
	//render_context.m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE, 1);
	static_cast<LightModel*>(m_Model.GetData())->Render(render_context);
}

const SPointlightData& PointLight::GetData() const
{
	return myData;
}

CU::Matrix44f PointLight::GetOrientation()
{
	return myOrientation;
}

void PointLight::RenderSphere()
{
//Should be a transparent sphere
}

