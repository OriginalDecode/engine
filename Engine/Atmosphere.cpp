#include "stdafx.h"
#include "Atmosphere.h"
#include <Engine/DirectX11.h>
#include <Engine/Effect.h>
#include <Engine/Engine.h>
#include <Engine/Model.h>
#include <Engine/Quad.h>
#include <d3dcompiler.h>
#include <string.h>
#include <Engine/AtmosphereModel.h>
void Atmosphere::Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position)
{
	m_Engine = Engine::GetInstance();
	m_API = m_Engine->GetAPI();
	m_Camera = m_Engine->GetCamera();

	m_OuterRadius = outer_radius;
	m_OuterOrientation.SetPosition(position);

	m_InnerRadius = inner_radius;
	m_InnerOrientation.SetPosition(position);

	m_VertexBuffer = m_API->CreateConstantBuffer(sizeof(cbVertex));
	m_PixelBuffer = m_API->CreateConstantBuffer(sizeof(cbPixel));

	m_OuterOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_OuterRadius, m_OuterRadius, m_OuterRadius, 1)) * m_OuterOrientation;
	m_InnerOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_InnerRadius, m_InnerRadius, m_InnerRadius, 1)) * m_InnerOrientation;

	const VirtualFileSystem& vfs = m_Engine->GetVFS();
	//m_OuterSphere = new AtmosphereModel;
	std::string atmosphere = m_Engine->LoadModel(vfs.GetFile("Models/atmosphere.fbx"), "Shaders/T_Skysphere.json", m_OuterSphere, false);
	m_OuterSphere = static_cast<AtmosphereModel*>(Engine::GetInstance()->GetModel(atmosphere));

	m_VertexStruct.m_InnerRadius = m_InnerRadius;
	m_VertexStruct.m_OuterRadius = m_OuterRadius;

	m_PixelStruct.m_InnerRadius = m_InnerRadius;
	m_PixelStruct.m_OuterRadius = m_OuterRadius;

	//m_InnerSphere->SetOrientation(m_InnerOrientation);
	m_OuterSphere->SetOrientation(m_OuterOrientation);
}

void Atmosphere::CleanUp()
{
	SAFE_RELEASE(m_PixelBuffer);
	SAFE_RELEASE(m_VertexBuffer);


}

void Atmosphere::Render(const CU::Matrix44f& orientation, Texture* depth, const RenderContext& render_context)
{

	m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	UpdateCameraData();

	m_API->UpdateConstantBuffer(m_VertexBuffer, &m_VertexStruct);
	m_API->UpdateConstantBuffer(m_PixelBuffer, &m_PixelStruct);

	IDevContext* ctx = m_API->GetContext();
	ctx->OMSetRenderTargets(1, m_API->GetBackbufferRef(), depth->GetDepthView());

	ctx->VSSetConstantBuffers(1, 1, &m_VertexBuffer);
	ctx->PSSetConstantBuffers(0, 1, &m_PixelBuffer);

	m_OuterSphere->Render(orientation, m_Camera->GetPerspective(), render_context);
	//m_InnerSphere->Render(orientation, m_Camera->GetPerspective(), render_context);

}

void Atmosphere::SetLightData(const CU::Vector4f& direction, const CU::Vector4f& position)
{
	m_VertexStruct.m_LightDir = direction;
	m_VertexStruct.m_LightPos = position;

}

void Atmosphere::UpdateCameraData()
{
	m_VertexStruct.m_CameraDir = m_Camera->GetAt();
	m_VertexStruct.m_CameraPos = m_Camera->GetPosition();

	m_PixelStruct.m_CameraDir = m_Camera->GetAt();
	m_PixelStruct.m_CameraPos = m_Camera->GetPosition();

	float camera_magnitude = CU::Math::Length(m_Camera->GetPosition());
	float camera_magnitude2 = CU::Math::Length2(m_Camera->GetPosition());

	m_PixelStruct.m_CameraMagnitude = camera_magnitude;
	m_PixelStruct.m_CameraMagnitude2 = camera_magnitude2;

	m_VertexStruct.m_CameraMagnitude = camera_magnitude;
	m_VertexStruct.m_CameraMagnitude2 = camera_magnitude2;

}
