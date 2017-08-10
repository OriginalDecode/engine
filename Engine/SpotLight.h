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
	SpotLight() = default;

	void Initiate();
	void CleanUp();

	void Render(const CU::Matrix44f& previousOrientation, Camera* aCamera, const RenderContext& render_context);

	void SetData(const SpotlightData& data);
	const SpotlightData& GetData() const;

	ShadowSpotlight* GetShadowSpotlight() { return m_ShadowSpotlight; }

	bool ShadowCasting() { return ( m_ShadowSpotlight ? true : false ); }

private:

	void SetPosition(const CU::Vector3f& aPosition);
	void SetDirection(const CU::Vector4f& aDirection);
	
	IInputLayout* m_InputLayout = nullptr;

	VertexDataWrapper m_VertexData;
	VertexBufferWrapper m_VertexBuffer;


	struct gsbuffer
	{
		CU::Matrix44f view_projection;
		CU::Vector4f direction;
	} m_gsBuffer;
	IBuffer* m_gsCBuffer = nullptr;


	struct spotlight
	{
		CU::Vector4f m_Position;
		CU::Vector4f m_Range;
	} m_Data;

	LightModel* m_Model = nullptr;
	ShadowSpotlight* m_ShadowSpotlight = nullptr;
	SpotlightData myData;
};

