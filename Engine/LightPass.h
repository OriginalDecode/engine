#pragma once
#include "VertexStructs.h"
#include "snowblind_shared.h"
namespace Snowblind
{
	class GBuffer;
	class CSpotLight;
	class LightPass
	{
	public:
		LightPass() = default;

		bool Initiate(GBuffer* aGBuffer);
		bool CleanUp();
		void RenderPointlight(CPointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation);
		void RenderSpotlight(CSpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation);
		Effect* GetPointlightEffect();
		Effect* GetSpotlightEffect();
	private:
		void UpdatePointlightBuffers(CPointLight* pointlight, Camera* aCamera, const CU::Matrix44f& previousOrientation);
		void UpdateSpotlightBuffers(CSpotLight* spotlight, Camera* aCamera, const CU::Matrix44f& previousOrientation);

		void CreateSpotlightBuffers();
		void CreatePointlightBuffers();
		struct SPointlightConstantBuffer : public SVertexBaseStruct
		{
		} myPointlightVertexConstantData; //Longest name.

		struct SSpotlightConstantBuffer : public SVertexBaseStruct
		{
		} mySpotlightVertexConstantData;

		struct SPixelConstantBuffer
		{
			CU::Matrix44f myInvertedProjection;
			CU::Matrix44f myView;
			CU::Vector4f myColor;
			CU::Vector4f myPosition;
			CU::Vector4f myCameraPosition;

		} myPixelConstantStruct;

		struct SSpotPixelConstantBuffer : public SPixelConstantBuffer
		{
			CU::Vector4f myDirection;
		} mySpotPixelConstantStruct;


		enum class eBuffer
		{
			POINT_LIGHT_VERTEX,
			POINT_LIGHT_PIXEL,
			SPOT_LIGHT_VERTEX,
			SPOT_LIGHT_PIXEL,
			_COUNT
		};
		ID3D11Buffer* myConstantBuffers[u32(eBuffer::_COUNT)];
		ID3D11DeviceContext* myContext;

		enum class eLight
		{
			POINT_LIGHT,
			SPOT_LIGHT,
			_COUNT
		};

		Effect* myEffect[u32(eLight::_COUNT)];
		Engine* myEngine;
		const GBuffer* myGBuffer;

	public:
		bool HasInitiated() {
			return (myGBuffer 
				&& myEngine 
				&& myEffect[u32(eLight::POINT_LIGHT)] 
				&& myEffect[u32(eLight::SPOT_LIGHT)] 
				&& myConstantBuffers[u32(eBuffer::POINT_LIGHT_VERTEX)]
				&& myConstantBuffers[u32(eBuffer::POINT_LIGHT_PIXEL)]
				&& myConstantBuffers[u32(eBuffer::SPOT_LIGHT_VERTEX)]
				&& myConstantBuffers[u32(eBuffer::SPOT_LIGHT_PIXEL)]);
		}

	};
};