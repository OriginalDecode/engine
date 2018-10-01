#include "stdafx.h"
#include "PointLight.h"
#include "Model.h"
#include <Engine/Quad.h>
PointLight::PointLight()
{
	//m_Model = new LightModel;
	u64 key = Engine::GetInstance()->LoadModel<LightModel>("Data/Model/lightMeshes/sphere.fbx", "Shaders/deferred_pointlight.json", false);
	m_Model = Engine::GetInstance()->GetModel<Model>(key);
	LightModel* model = static_cast<LightModel*>(m_Model.GetData());
	model->Initiate("cone.fbx");

#if !defined(_PROFILE) && !defined(_FINAL)
	Engine* pEngine = Engine::GetInstance();

	Texture* pQuadTex = pEngine->GetTexture("Data/Textures/lightbulb-on.dds");
	Effect* pEffect = pEngine->GetEffect("Data/Shaders/world_plane.json");
	pEffect->AddShaderResource(pQuadTex, TextureSlot::DIFFUSE);
	m_LightQuad = new Quad(pEffect, 0.5f, 0.5f);
	m_QuadBuffer = pEngine->GetAPI()->GetDevice().CreateConstantBuffer(sizeof(quadbuffer), "quadbuffer");

#endif

	//m_Model = Engine::GetInstance()->GetModel("Data/Model/lightMeshes/sphere.fbx");

}

PointLight::~PointLight()
{
#if !defined(_PROFILE) && !defined(_FINAL)
	delete m_LightQuad;
	Engine::GetInstance()->GetAPI()->ReleasePtr(m_QuadBuffer);

#endif

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
#if !defined(_PROFILE) && !defined(_FINAL)
	//This should be drawn in the entity pass too.
	render_context.GetContext().SetBlendState(graphics::ALPHA_BLEND);
	m_LightQuadBuffer.camera_orientation = render_context.GetEngine().GetCamera()->GetOrientation();
	render_context.GetContext().UpdateConstantBuffer(m_QuadBuffer, &m_LightQuadBuffer);
	render_context.GetContext().VSSetConstantBuffer(2, 1, &m_QuadBuffer);
	m_LightQuad->Render(true);
#endif
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

