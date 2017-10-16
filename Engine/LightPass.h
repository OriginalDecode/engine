#pragma once
#include "VertexStructs.h"
#include "engine_shared.h"

class SpotLight;
class PointLight;
namespace graphics
{
	class GBuffer;
	class RenderContext;

	class LightPass
	{
	public:
		LightPass(const GBuffer& gbuffer);
		~LightPass();

		void RenderPointlight(PointLight* pointlight, 
							  const CU::Matrix44f& camera_view, 
							  const CU::Matrix44f& camera_projection, 
							  const CU::Matrix44f& shadow_matrix, 
							  const RenderContext& render_context);
		void RenderSpotlight(SpotLight* spotlight, const CU::Matrix44f& camera_view, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_matrix, const RenderContext& render_context);

		Effect* GetPointlightEffect();
		Effect* GetSpotlightEffect();

	private:
		void UpdatePointlightBuffers(PointLight* pointlight, const CU::Matrix44f& camera_view, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_matrix, const RenderContext& render_context);
		void UpdateSpotlightBuffers(SpotLight* spotlight, const CU::Matrix44f& camera_view, const CU::Matrix44f& camera_projection, const CU::Matrix44f& shadow_matrix, const RenderContext& render_context);

		struct PointlightConstantBuffer : public VertexBaseStruct
		{
		} m_cbPointlightVtx;

		struct SpotlightConstantBuffer : public VertexBaseStruct
		{
		} m_cbSpotlightVtx;

		struct PixelConstantBuffer
		{
			CU::Vector4f m_Color;
			CU::Vector4f m_Position;
			CU::Vector4f m_CameraPosition;
			CU::Matrix44f m_ShadowMVP;

		} m_cbPointlightPix;

		struct SpotPixelConstantBuffer : public PixelConstantBuffer
		{
			CU::Vector4f m_Direction;
		} m_cbSpotlightPix;

		enum eBuffer
		{
			POINTLIGHT_VERTEX,
			POINTLIGHT_PIXEL,
			SPOTLIGHT_VERTEX,
			SPOTLIGHT_PIXEL,
			NOF_BUFFERS
		};
		IBuffer* m_LightBuffers[NOF_BUFFERS];


		enum eLight
		{
			POINTLIGHT,
			SPOTLIGHT,
			NOF_LIGHT_TYPES
		};

		Effect* m_Effect[NOF_LIGHT_TYPES];
	};
};