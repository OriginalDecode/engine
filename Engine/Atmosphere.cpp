#include "stdafx.h"
#include "Atmosphere.h"
#include <Engine/DirectX11.h>
#include <Engine/Effect.h>
#include <Engine/Engine.h>
#include <Engine/Model.h>
#include <Engine/Quad.h>
#include <Engine/AtmosphereModel.h>
#include <Engine/IGraphicsDevice.h>

#include <string.h>

Atmosphere::~Atmosphere()
{
	Engine::GetAPI()->ReleasePtr(m_PixelBuffer);
	Engine::GetAPI()->ReleasePtr(m_VertexBuffer);
}

#define if_unlikely(cond) if(!(cond)); else //black magic statement

void Atmosphere::Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position)
{
	/*m_OuterRadius = outer_radius;
	m_OuterOrientation.SetPosition(position);

	m_InnerRadius = inner_radius;
	m_InnerOrientation.SetPosition(position);
*/
	graphics::IGraphicsDevice& device = Engine::GetInstance()->GetAPI()->GetDevice();

	//m_VertexBuffer = device.CreateConstantBuffer(sizeof(cbVertex), "Atmosphere cbVertex");
	m_PixelBuffer =  device.CreateConstantBuffer(sizeof(cbPixel), "Atmosphere cbPixel");

	//m_OuterOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_OuterRadius, m_OuterRadius, m_OuterRadius, 1)) * m_OuterOrientation;
	//m_InnerOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_InnerRadius, m_InnerRadius, m_InnerRadius, 1)) * m_InnerOrientation;

	//const VirtualFileSystem& vfs = Engine::GetInstance()->GetVFS();
	//u64 atmosphere = Engine::GetInstance()->LoadModel<AtmosphereModel>(vfs.GetFile("Models/atmosphere.fbx"), "Shaders/skysphere.json", false);
	//m_OuterSphere = Engine::GetInstance()->GetModel<Model>(atmosphere);

	//m_VertexStruct.m_InnerRadius = m_InnerRadius;
	//m_VertexStruct.m_OuterRadius = m_OuterRadius;

	//static_cast<AtmosphereModel*>(m_OuterSphere.GetData())->SetOrientation(m_OuterOrientation);
}

void Atmosphere::Render(const graphics::RenderContext& rc)
{
	Engine* pEngine = Engine::GetInstance();
	graphics::IGraphicsContext& ctx = pEngine->GetAPI()->GetContext();
	graphics::IGraphicsAPI* api = pEngine->GetAPI();


	m_PixelStruct.light_dir = Engine::GetInstance()->GetLightDir();
	m_PixelStruct.light_dir.w = 1;
	m_PixelStruct.view_dir = pEngine->GetCamera()->GetAt();
	m_PixelStruct.view_pos = pEngine->GetCamera()->GetPosition();
	//static_cast<AtmosphereModel*>(m_OuterSphere.GetData())->SetPosition(pEngine->GetCamera()->GetPosition());
	//m_OuterOrientation.SetPosition(pEngine->GetCamera()->GetPos());
	//m_VertexStruct.orientation = m_OuterOrientation;

	ctx.UpdateConstantBuffer(m_PixelBuffer, &m_PixelStruct);
	//ctx.UpdateConstantBuffer(m_VertexBuffer, &m_VertexStruct);
	ctx.PSSetConstantBuffer(2, 1, &m_PixelBuffer);
	//ctx.VSSetConstantBuffer(1, 1, &m_VertexBuffer);
	
	//ctx.SetBlendState(api->GetBlendState(graphics::NO_BLEND));
	//ctx.SetDepthState(api->GetDepthStencilState(graphics::Z_ENABLED), 1);
	//ctx.SetRasterizerState(api->GetRasterizerState(graphics::CULL_NONE));
	//static_cast<AtmosphereModel*>(m_OuterSphere.GetData())->SetOrientation(m_OuterOrientation);
	//static_cast<AtmosphereModel*>(m_OuterSphere.GetData())->Render(rc);

}

void Atmosphere::SetLightData(const CU::Vector4f& direction, const CU::Vector4f& position)
{
	//m_VertexStruct.m_LightDir = direction;
	//m_VertexStruct.m_LightPos = position;

}