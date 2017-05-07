#pragma once
#include <Engine/snowblind_shared.h>
#include "LightStructs.h"

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

private:

	void SetPosition(const CU::Vector3f& aPosition);
	void SetDirection(const CU::Vector4f& aDirection);

	LightModel* m_Model = nullptr;
	ShadowSpotlight* m_ShadowSpotlight = nullptr;
	SpotlightData myData;
};

