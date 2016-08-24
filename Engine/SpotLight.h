#pragma once
#include "LightStructs.h"
namespace Snowblind
{
	class CInstance;
	class CCamera;

	class CSpotLight
	{
	public:
		CSpotLight();
		~CSpotLight();

		void Render(const CU::Matrix44f& previousOrientation, CCamera* aCamera);
		void SetColor(const CU::Vector4f& aColor);
		void SetRange(float aRange);
		void SetAngle(float anAngle);
		void SetPosition(const CU::Vector3f& aPosition);
		void SetDirection(const CU::Vector4f& aDirection);
		SSpotlightData& GetData();
		CU::Matrix44f myBaseMatrix;
	private:
		SSpotlightData myData;
		CInstance* myLightMesh = nullptr;
	};

};