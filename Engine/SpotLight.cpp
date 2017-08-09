#include "stdafx.h"
#include "SpotLight.h"
#include "Instance.h"
#include "Camera.h"
#include <Engine/ShadowSpotlight.h>




void SpotLight::Initiate()
{




	std::string key = Engine::GetInstance()->LoadModel<LightModel>("Data/Model/lightMeshes/cone.fbx", "Shaders/deferred_spotlight.json", 0, false);
	//m_Model = static_cast<LightModel*>(Engine::GetInstance()->GetModel(key));

	//m_Model->Initiate("cone.fbx");
	m_ShadowSpotlight = new ShadowSpotlight;
	m_ShadowSpotlight->Initiate(2048.f);
	m_ShadowSpotlight->GetCamera()->RotateAroundX(cl::DegreeToRad(90.f));


	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json");
	CompiledShader* shader = effect->GetVertexShader();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_InputLayout = Engine::GetAPI()->CreateInputLayout(shader->compiledShader, shader->shaderSize, layout, ARRAYSIZE(layout));

	m_VertexBuffer.myStride = sizeof(spotlight);
	m_VertexBuffer.myStartSlot = 0;
	m_VertexBuffer.myNrOfBuffers = 1;
	m_VertexBuffer.myByteOffset = 0;

	m_VertexData.myNrOfVertexes = 1;
	m_VertexData.myStride = sizeof(spotlight);
	m_VertexData.mySize = m_VertexData.myNrOfVertexes * m_VertexData.myStride;
	m_VertexData.myVertexData = new s8[m_VertexData.mySize];
	m_VertexBuffer.myVertexBuffer = Engine::GetAPI()->CreateVertexBuffer(m_VertexData.mySize, m_VertexData.myVertexData);
}





void SpotLight::CleanUp()
{
	m_Model = nullptr;
	m_ShadowSpotlight->CleanUp();
	SAFE_DELETE(m_ShadowSpotlight);
	
}

void SpotLight::Render(const CU::Matrix44f& previousOrientation, Camera* aCamera, const RenderContext& render_context)
{

	render_context.m_API->UpdateConstantBuffer(m_VertexBuffer.myVertexBuffer, &m_Data, sizeof(spotlight));

	ID3D11DeviceContext* context = render_context.m_Context;
	context->IASetInputLayout(m_InputLayout);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetVertexBuffers(	m_VertexBuffer.myStartSlot
								, m_VertexBuffer.myNrOfBuffers
								, &m_VertexBuffer.myVertexBuffer
								, &m_VertexBuffer.myStride
								, &m_VertexBuffer.myByteOffset);

	Effect* effect = Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json");
	effect->Use();
	context->Draw(1, 0);
	effect->Clear();


	//render_context.m_Context->Draw(v, 0);

	//render_context.m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	//m_Model->Render(previousOrientation, aCamera->GetPerspective(), render_context);
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
	//m_Model->GetOrientation().SetPosition(aPosition);
	myData.myLightPosition = aPosition;
	myData.myOrientation.SetPosition(aPosition);
	m_ShadowSpotlight->GetCamera()->SetPosition(aPosition);
}

void SpotLight::SetDirection(const CU::Vector4f& aDirection)
{
	myData.myDirection.x = aDirection.x;
	myData.myDirection.y = aDirection.y;
	myData.myDirection.z = aDirection.z;
	//m_Model->GetOrientation().SetForward(aDirection);
	//m_ShadowSpotlight->GetCamera()->SetAt(aDirection);
}