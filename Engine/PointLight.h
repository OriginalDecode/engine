#pragma once
#include "LightStructs.h"
#include <Engine/LightModel.h>
class PointLight
{
public:
	PointLight();

	void SetPosition(const CU::Vector3f& aPosition);
	CU::Vector3f GetPosition();

	void SetColor(const CU::Vector4f& aColor);
	const CU::Vector4f& GetColor();

	void SetRange(float aRange);
	const float& GetRange();

	void Update();
	void Render(const CU::Matrix44f& previousOrientation, Camera* camera, const RenderContext& render_context);
	const SPointlightData& GetData() const;

	CU::Matrix44f GetOrientation();
private:
	LightModel* m_Model = nullptr;

	CU::Matrix44f myOrientation;
	CU::Vector3f myOriginalPosition;
	CU::Vector4f myColor;
	SPointlightData myData;
	float myRange;

};
