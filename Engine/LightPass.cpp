#include "stdafx.h"
#include "LightPass.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <Engine/engine_shared.h>
#include <Engine/GBuffer.h>

namespace graphics
{
	LightPass::LightPass(const GBuffer& gbuffer)
	{
		m_Effect[POINTLIGHT] = Engine::GetInstance()->GetEffect("Shaders/deferred_pointlight.json");
		m_Effect[POINTLIGHT]->AddShaderResource(gbuffer.GetDiffuse(), Effect::DIFFUSE);
		m_Effect[POINTLIGHT]->AddShaderResource(gbuffer.GetNormal(), Effect::NORMAL);
		m_Effect[POINTLIGHT]->AddShaderResource(gbuffer.GetDepth(), Effect::DEPTH);


		m_Effect[SPOTLIGHT] = Engine::GetInstance()->GetEffect("Shaders/deferred_spotlight.json");
		m_Effect[SPOTLIGHT]->AddShaderResource(gbuffer.GetDiffuse(), Effect::DIFFUSE);
		m_Effect[SPOTLIGHT]->AddShaderResource(gbuffer.GetNormal(), Effect::NORMAL);
		m_Effect[SPOTLIGHT]->AddShaderResource(gbuffer.GetDepth(), Effect::DEPTH);

		graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
		m_LightBuffers[POINTLIGHT_VERTEX] = device.CreateConstantBuffer(sizeof(PointlightConstantBuffer), "LightPass PointlightVertex ConstBuffer");
		m_LightBuffers[POINTLIGHT_PIXEL] = device.CreateConstantBuffer(sizeof(PixelConstantBuffer), "LightPass PointlightPixel ConstBuffer");

		m_LightBuffers[SPOTLIGHT_VERTEX] = device.CreateConstantBuffer(sizeof(SpotlightConstantBuffer), "LightPass SpotlightVertex ConstBuffer");
		m_LightBuffers[SPOTLIGHT_PIXEL] = device.CreateConstantBuffer(sizeof(SpotPixelConstantBuffer), "LightPass SpotlightPixel ConstBuffer");
	}

	LightPass::~LightPass()
	{
		graphics::IGraphicsAPI* api = Engine::GetAPI();
		api->ReleasePtr(m_LightBuffers[POINTLIGHT_VERTEX]);
		api->ReleasePtr(m_LightBuffers[POINTLIGHT_PIXEL]);
		api->ReleasePtr(m_LightBuffers[SPOTLIGHT_VERTEX]);
		api->ReleasePtr(m_LightBuffers[SPOTLIGHT_PIXEL]);
	}

	void LightPass::RenderPointlight(PointLight* pointlight,
									 const CU::Matrix44f& camera_view,
									 const CU::Matrix44f& camera_projection,
									 const CU::Matrix44f& shadow_matrix,
									 const RenderContext& render_context) 
	{
		UpdatePointlightBuffers(pointlight, camera_view, camera_projection, shadow_matrix, render_context);
		pointlight->Render(camera_view, camera_projection, render_context);
	}

	void LightPass::RenderSpotlight(SpotLight* spotlight,
									const CU::Matrix44f& camera_view,
									const CU::Matrix44f& camera_projection,
									const CU::Matrix44f& shadow_matrix,
									const RenderContext& render_context) 
	{
		UpdateSpotlightBuffers(spotlight, camera_view, camera_projection, shadow_matrix, render_context);
		spotlight->Render(render_context);
	}

	Effect* LightPass::GetPointlightEffect()
	{
		return m_Effect[POINTLIGHT];
	}

	Effect* LightPass::GetSpotlightEffect()
	{
		return m_Effect[SPOTLIGHT];
	}

	void LightPass::UpdatePointlightBuffers(PointLight* pointlight, 
											const CU::Matrix44f& camera_view, 
											const CU::Matrix44f& camera_projection, 
											const CU::Matrix44f& shadow_matrix, 
											const RenderContext& render_context)
	{
		graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();

		m_cbPointlightVtx.world = pointlight->GetOrientation();
		m_cbPointlightVtx.invertedView = CU::Math::Inverse(camera_view);
		m_cbPointlightVtx.projection = camera_projection;
		m_cbPointlightVtx.scale.x = pointlight->GetRange();
		m_cbPointlightVtx.scale.y = pointlight->GetRange();
		m_cbPointlightVtx.scale.z = pointlight->GetRange();
		m_cbPointlightVtx.scale.w = pointlight->GetRange();
		context.UpdateConstantBuffer(m_LightBuffers[POINTLIGHT_VERTEX], &m_cbPointlightVtx, sizeof(PointlightConstantBuffer));

		//____________________________________________________________________________

		
		//m_cbPointlightPix.m_InvertedProjection = CU::Math::InverseReal(camera_projection);
		//m_cbPointlightPix.m_View = camera_view;
		m_cbPointlightPix.m_Color = pointlight->GetColor();
		m_cbPointlightPix.m_Position = pointlight->GetPosition();
		m_cbPointlightPix.m_CameraPosition = camera_view.GetPosition();
		m_cbPointlightPix.m_ShadowMVP = shadow_matrix;
		context.UpdateConstantBuffer(m_LightBuffers[POINTLIGHT_PIXEL], &m_cbPointlightPix, sizeof(PixelConstantBuffer));

		context.VSSetConstantBuffer(1, 1, &m_LightBuffers[POINTLIGHT_VERTEX]);
		context.PSSetConstantBuffer(1, 1, &m_LightBuffers[POINTLIGHT_PIXEL]);

	}

	void LightPass::UpdateSpotlightBuffers(SpotLight* spotlight,
										   const CU::Matrix44f& camera_view,
										   const CU::Matrix44f& camera_projection,
										   const CU::Matrix44f& shadow_matrix,
										   const RenderContext& render_context) {

		graphics::IGraphicsContext& context = Engine::GetAPI()->GetContext();
		const SpotlightData& data = spotlight->GetData();

		m_cbSpotlightVtx.m_Orientation = data.myOrientation;
		m_cbSpotlightVtx.m_Range = data.myRange;
		m_cbSpotlightVtx.m_Angle = data.myAngle;

		context.UpdateConstantBuffer(m_LightBuffers[SPOTLIGHT_VERTEX], &m_cbSpotlightVtx, sizeof(SpotlightConstantBuffer));

		//____________________________________________________________________________

		//m_cbSpotlightPix.m_InvertedProjection = CU::Math::InverseReal(camera_projection);
		//m_cbSpotlightPix.m_View = camera_view;
		m_cbSpotlightPix.m_Color = data.myLightColor;
		m_cbSpotlightPix.m_Position = data.myOrientation.GetPosition();
		m_cbSpotlightPix.m_Position.w = 1.f;
		m_cbSpotlightPix.m_CameraPosition = camera_view.GetPosition();
		m_cbSpotlightPix.m_Direction = data.myDirection;
		m_cbSpotlightPix.m_Direction.w = 1.f;
		m_cbSpotlightPix.m_ShadowMVP = shadow_matrix;
		context.UpdateConstantBuffer(m_LightBuffers[SPOTLIGHT_PIXEL], &m_cbSpotlightPix, sizeof(SpotPixelConstantBuffer));

		context.VSSetConstantBuffer(1, 1, &m_LightBuffers[SPOTLIGHT_VERTEX]);
		context.PSSetConstantBuffer(1, 1, &m_LightBuffers[SPOTLIGHT_PIXEL]);

	}
};