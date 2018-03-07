#include "stdafx.h"
#include "SpotLight.h"
#include "Instance.h"
#include "Camera.h"
#include <Engine/ShadowSpotlight.h>

#include <Engine/Quad.h>

#ifdef _DEBUG
#include <Engine/Synchronizer.h>
#endif

static bool s_Wireframe = false;

const float c0 = cosf(cl::DegreeToRad(0.f));
const float c90 = cosf(cl::DegreeToRad(90.f));
const float c180 = cosf(cl::DegreeToRad(180.f));
const float c270 = cosf(cl::DegreeToRad(270.f));

const float s0 = sinf(cl::DegreeToRad(0.f));
const float s90 = sinf(cl::DegreeToRad(90.f));
const float s180 = sinf(cl::DegreeToRad(180.f));
const float s270 = sinf(cl::DegreeToRad(270.f));



SpotLight::SpotLight()
{
	Engine* pEngine = Engine::GetInstance();
	u64 key = pEngine->LoadModel<LightModel>("Data/Model/lightMeshes/cone.fbx", "Shaders/deferred_spotlight.json", false);

	
	m_Model = pEngine->GetModel<Model>(key);
	LightModel* model = static_cast<LightModel*>(m_Model.GetData());
	model->Initiate("cone.fbx");




#if !defined(_PROFILE) && !defined(_FINAL)

	Texture* pQuadTex = pEngine->GetTexture("Data/Textures/lightbulb-on.dds");
	Effect* pEffect = pEngine->GetEffect("Data/Shaders/world_plane.json");
	pEffect->AddShaderResource(pQuadTex, Effect::DIFFUSE);
	m_LightQuad = new Quad(pEffect, 0.5f, 0.5f);
	m_QuadBuffer = pEngine->GetAPI()->GetDevice().CreateConstantBuffer(sizeof(quadbuffer), "quadbuffer");

#endif
	//This is for the light volume and shadows

	// 	m_ShadowSpotlight = new ShadowSpotlight;
	// 	m_ShadowSpotlight->Initiate(2048.f);
	// 	m_ShadowSpotlight->GetCamera()->RotateAroundX(cl::DegreeToRad(90.f));

	// 	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/lightvolume_spot.json");
	// 	CompiledShader* shader = effect->GetVertexShader();
	// 
	// 	D3D11_INPUT_ELEMENT_DESC layout[] =
	// 	{
	// 		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// 		{ "ANGLE", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// 		{ "RANGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	// 		{ "DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	// 	};
	// 
	// 	m_InputLayout = Engine::GetAPI()->CreateInputLayout(shader->compiledShader, shader->shaderSize, layout, ARRAYSIZE(layout));
	// 
	// 	m_VertexBuffer.myStride = sizeof(spotlight);
	// 	m_VertexBuffer.myStartSlot = 0;
	// 	m_VertexBuffer.myNrOfBuffers = 1;
	// 	m_VertexBuffer.myByteOffset = 0;
	// 
	// 	m_VertexData.myNrOfVertexes = 1;
	// 	m_VertexData.myStride = sizeof(spotlight);
	// 	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	// 	m_VertexData.myVertexData = new s8[m_VertexData.mySize];
	// 	m_VertexBuffer.myVertexBuffer = Engine::GetAPI()->CreateVertexBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
	// 
	// 	//Engine::GetInstance()->AddCheckBox(&s_Wireframe, "Wireframe Spotlight");
	// 	m_gsCBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(gsbuffer));
	// 	m_psCBuffer = Engine::GetAPI()->GetDevice().CreateConstantBuffer(sizeof(psbuffer));
}

SpotLight::~SpotLight()
{


#if !defined(_PROFILE) && !defined(_FINAL)
	SAFE_DELETE(m_LightQuad);
	Engine::GetInstance()->GetAPI()->ReleasePtr(m_QuadBuffer);
#endif

	SAFE_DELETE(m_ShadowSpotlight);

}

/**
void SpotLight::Render(const CU::Matrix44f& previousOrientation, Camera* aCamera)
{
	render_context.m_API->SetBlendState(eBlendStates::LIGHT_BLEND);
	render_context.m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE,1);
	m_Model->Render(previousOrientation, aCamera->GetPerspective(), render_context);

	render_context.m_API->UpdateConstantBuffer(m_VertexBuffer.myVertexBuffer, &m_Data, sizeof(spotlight));

	m_gsBuffer.view_projection = CU::Math::Inverse(previousOrientation) * aCamera->GetPerspective();

	m_psBuffer.color = myData.myLightColor;
	m_psBuffer.eye_pos = aCamera->GetOrientation().GetPosition();
	m_psBuffer.light_pos = myData.myOrientation.GetPosition();

	ID3D11DeviceContext* context = render_context.m_Context;
	context->IASetInputLayout(m_InputLayout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetVertexBuffers(	m_VertexBuffer.myStartSlot
								, m_VertexBuffer.myNrOfBuffers
								, &m_VertexBuffer.myVertexBuffer
								, &m_VertexBuffer.myStride
								, &m_VertexBuffer.myByteOffset);

	render_context.m_API->UpdateConstantBuffer(m_gsCBuffer, &m_gsBuffer);
	render_context.m_API->UpdateConstantBuffer(m_psCBuffer, &m_psBuffer);

	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/lightvolume_spot.json");

	render_context.m_Context->GSSetConstantBuffers(0, 1, &m_gsCBuffer);
	render_context.m_Context->PSSetConstantBuffers(0, 1, &m_psCBuffer);

	render_context.m_API->SetDepthStencilState(eDepthStencilState::READ_NO_WRITE_PARTICLE, 1);
	render_context.m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	render_context.m_API->SetRasterizer( s_Wireframe ? eRasterizer::WIREFRAME : eRasterizer::CULL_NONE);
	effect->Use();
	context->Draw(1, 0);
	effect->Clear();

}
*/
void SpotLight::Render(const graphics::RenderContext& render_context)
{
	static_cast<LightModel*>(m_Model.GetData())->Render(render_context);
#if !defined(_PROFILE) && !defined(_FINAL)
	//This should be drawn in the entity pass too.
 	render_context.GetContext().SetBlendState(render_context.GetAPI().GetBlendState(graphics::ALPHA_BLEND));
 	m_LightQuadBuffer.camera_orientation = render_context.GetEngine().GetCamera()->GetOrientation();
 	render_context.GetContext().UpdateConstantBuffer(m_QuadBuffer, &m_LightQuadBuffer);
 	render_context.GetContext().VSSetConstantBuffer(2, 1, &m_QuadBuffer);
 	m_LightQuad->Render(true);
#endif
#ifdef _DEBUG
	RenderDebugCone();
#endif

}

void SpotLight::SetData(const SpotlightData& data)
{
	myData = data;
	m_Data.m_Range = myData.myRange;
	m_Data.m_Angle = myData.myAngle;
	SetDirection(myData.myOrientation.GetForward());
	SetPosition(myData.myLightPosition);
	//m_ShadowSpotlight->SetAngle(myData.myAngle);
	if(m_ShadowSpotlight)
		const float buffer_size = m_ShadowSpotlight->GetBufferSize();
	//m_ShadowSpotlight->GetCamera()->RecalculatePerspective(buffer_size, buffer_size, 0.1f, myData.myRange);
}

const SpotlightData& SpotLight::GetData() const
{
	return myData;
}

#ifdef _DEBUG
void SpotLight::RenderDebugCone()
{

	Synchronizer* sync = Engine::GetInstance()->GetSynchronizer();

	const CU::Vector4f col = { 0.72f, 0.51f,  0.25f, 1.f };
	const int sides = 32;
	const float range = myData.myRange;
	const float theta = tan(myData.myAngle);
	const float halfwidth = theta * myData.myRange;
	const int _360 = 360;
	const int max = _360 / sides;


	LinePoint lamp;
	lamp.color = col;
	lamp.position = myData.myOrientation.GetPosition();

	LinePoint _0deg, _90deg, _180deg, _270deg;
	_0deg.color = _90deg.color = _180deg.color = _270deg.color = col;


	const float y_pos = range;


	_0deg.position = { c0 * halfwidth, y_pos, s0 * halfwidth, 1 };
	_90deg.position = { c90 * halfwidth, y_pos, s90 * halfwidth, 1 };
	_180deg.position = { c180 * halfwidth, y_pos, s180 * halfwidth, 1 };
	_270deg.position = { c270 * halfwidth, y_pos, s270 * halfwidth, 1 };


	CU::Matrix44f _rot = myData.myOrientation;
	_rot = CU::Matrix44f::CreateRotateAroundX(cl::DegreeToRad(90.f)) * _rot;

	_0deg.position = _0deg.position	 * _rot;
	_90deg.position = _90deg.position	 * _rot;
	_180deg.position = _180deg.position * _rot;
	_270deg.position = _270deg.position * _rot;

	sync->AddRenderCommand(LineCommand(lamp, _0deg, true));
	sync->AddRenderCommand(LineCommand(lamp, _90deg, true));
	sync->AddRenderCommand(LineCommand(lamp, _180deg, true));
	sync->AddRenderCommand(LineCommand(lamp, _270deg, true));


	for (int i = 0; i < _360; i += max)
	{

		LinePoint p0, p1;
		p0.color = col;
		p1.color = col;

		const float x = cl::DegreeToRad(static_cast<float>(i));
		const float y = cl::DegreeToRad(static_cast<float>(i + max));

		p0.position = { cosf(x) * halfwidth, y_pos, sinf(x) * halfwidth, 1.f };
		p1.position = { cosf(y) * halfwidth, y_pos , sinf(y) * halfwidth, 1.f };

		p0.position = p0.position * _rot;
		p1.position = p1.position * _rot;

		sync->AddRenderCommand(LineCommand(p0, p1, true));
	}
}
#endif

void SpotLight::SetPosition(const CU::Vector3f& aPosition)
{
	m_Model->GetOrientation().SetPosition(aPosition);
	myData.myLightPosition = aPosition;
	m_Data.m_Position = aPosition;
	myData.myOrientation.SetPosition(aPosition);

	if(m_ShadowSpotlight)
		m_ShadowSpotlight->GetCamera()->SetPosition(aPosition);
}

void SpotLight::SetDirection(const CU::Vector4f& aDirection)
{
	myData.myDirection.x = aDirection.x;
	myData.myDirection.y = aDirection.y;
	myData.myDirection.z = aDirection.z;
	m_Model->GetOrientation().SetForward(aDirection);
	m_Data.m_Direction = myData.myDirection;
	//m_ShadowSpotlight->GetCamera()->SetAt(aDirection);
}