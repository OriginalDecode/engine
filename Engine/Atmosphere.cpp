
#include "stdafx.h"
#include "Atmosphere.h"

#include <Engine/DirectX11.h>
#include <Engine/Engine.h>
#include <Engine/IGraphicsDevice.h>


Atmosphere::~Atmosphere()
{
	Engine::GetAPI()->ReleasePtr(m_PixelBuffer);
}

void Atmosphere::Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position)
{
	graphics::IGraphicsDevice& device = Engine::GetInstance()->GetAPI()->GetDevice();
	m_PixelBuffer =  device.CreateConstantBuffer(sizeof(cbPixel), "Atmosphere cbPixel");
}

void Atmosphere::UpdateBuffer(const graphics::RenderContext& rc, Camera* active_camere)
{
	Engine* pEngine = Engine::GetInstance();
	graphics::IGraphicsContext& ctx = pEngine->GetAPI()->GetContext();

	m_PixelStruct.light_dir = CU::Vector4f(pEngine->GetLightDir(), 1);
	m_PixelStruct.view_dir = active_camere->GetAt();
	m_PixelStruct.view_pos = active_camere->GetPosition();

	ctx.UpdateConstantBuffer(m_PixelBuffer, &m_PixelStruct);
	ctx.PSSetConstantBuffer(2, 1, &m_PixelBuffer);
}
