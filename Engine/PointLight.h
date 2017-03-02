#pragma once
#include "LightStructs.h"





class CModel;
class CPointLight
{
public:
	CPointLight();

	void Initiate();

	void SetPosition(const CU::Vector3f& aPosition);
	CU::Vector3f GetPosition();

	void SetColor(const CU::Vector4f& aColor);
	const CU::Vector4f& GetColor();

	void SetRange(float aRange);
	const float& GetRange();

	void Update();
	void Render(const CU::Matrix44f& previousOrientation, Camera* camera);
	const SPointlightData& GetData() const;

	CU::Matrix44f GetOrientation();
private:
	CModel* m_Model;
	CU::Matrix44f myOrientation;
	CU::Vector3f myOriginalPosition;
	CU::Vector4f myColor;
	SPointlightData myData;
	float myRange;

};
