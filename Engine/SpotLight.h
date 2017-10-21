#pragma once
#include <Engine/engine_shared.h>
#include "LightStructs.h"
#include "VertexWrapper.h"
class Camera;
class LightModel;
class ShadowSpotlight;
class SpotLight
{
public:
	SpotLight();
	~SpotLight();

	void Render(const graphics::RenderContext& render_context);

	void SetData(const SpotlightData& data);
	const SpotlightData& GetData() const;

	ShadowSpotlight* GetShadowSpotlight() { return m_ShadowSpotlight; }

	bool ShadowCasting() { return ( m_ShadowSpotlight ? true : false ); }

private:

	void SetPosition(const CU::Vector3f& aPosition);
	void SetDirection(const CU::Vector4f& aDirection);


	//VertexWrapper m_VertexWrapper;


	struct gsbuffer
	{
		CU::Matrix44f view_projection;
	} m_gsBuffer;
	IBuffer* m_gsCBuffer = nullptr;

	struct psbuffer
	{
		CU::Vector4f color;
		CU::Vector4f eye_pos;
		CU::Vector4f light_pos;
	} m_psBuffer;
	IBuffer* m_psCBuffer = nullptr;

	struct vsbuffer
	{
		CU::Matrix44f world;
	} m_vsBuffer;

	struct spotlight
	{
		CU::Vector3f m_Position;
		float m_Angle = 0.f;
		float m_Range = 0.f;
		CU::Vector3f m_Direction;
	} m_Data;

	LightModel* m_Model = nullptr;
	ShadowSpotlight* m_ShadowSpotlight = nullptr;
	SpotlightData myData;






#if !defined(_PROFILE) && !defined(_FINAL)
	IBuffer* m_QuadBuffer = nullptr;
	struct quadbuffer
	{
		CU::Matrix44f camera_orientation; //non inverted, we want the rotation
	} m_LightQuadBuffer;

	Quad* m_LightQuad = nullptr;
#endif
};

