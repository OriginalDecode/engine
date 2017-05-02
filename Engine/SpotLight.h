#pragma once
#include "LightStructs.h"

class Camera;
class LightModel;
class SpotLight
{
public:
	SpotLight();

	void Render(const CU::Matrix44f& previousOrientation, Camera* aCamera, const RenderContext& render_context);
	void SetColor(const CU::Vector4f& aColor);
	void SetRange(float aRange);
	void SetAngle(float anAngle);
	void SetPosition(const CU::Vector3f& aPosition);
	void SetDirection(const CU::Vector4f& aDirection);
	SSpotlightData& GetData();

	void DoTranslation(const CU::Matrix44f& translationMatrix);
	CU::Matrix44f GetOrientatino();
private:
	LightModel* m_Model = nullptr;
	CU::Matrix44f myBaseMatrix;
	SSpotlightData myData;
};

