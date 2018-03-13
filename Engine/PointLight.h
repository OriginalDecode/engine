#pragma once
#include "LightStructs.h"
#include <Engine/LightModel.h>
#include <CommonLib/RefPointer.h>

class Quad;
class PointLight
{
public:
	PointLight();
	~PointLight();


	void SetPosition(const CU::Vector3f& aPosition);
	CU::Vector3f GetPosition();

	void SetColor(const CU::Vector4f& aColor);
	const CU::Vector4f& GetColor();

	void SetRange(float aRange);
	const float& GetRange();

	void Update();
	void Render(const CU::Matrix44f& previousOrientation, const CU::Matrix44f& camera_projection, const graphics::RenderContext& render_context);
	const SPointlightData& GetData() const;

	CU::Matrix44f GetOrientation();
private:
	void RenderSphere();

	RefPointer<Model> m_Model;// = nullptr;

	CU::Matrix44f myOrientation;
	CU::Vector3f myOriginalPosition;
	CU::Vector4f myColor;
	SPointlightData myData;
	float myRange;

#if !defined(_PROFILE) && !defined(_FINAL)
	IBuffer* m_QuadBuffer = nullptr;
	struct quadbuffer
	{
		CU::Matrix44f camera_orientation; //non inverted, we want the rotation
	} m_LightQuadBuffer;

	Quad* m_LightQuad = nullptr;
#endif

};
