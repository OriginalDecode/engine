#pragma once

#include <Engine/snowblind_shared.h>

#include <Engine/Camera.h>
#include <Engine/ShadowSpotlight.h>

#include <functional>

class Renderer;
class Effect;
class ShadowPass
{
public:

	bool Initiate(Renderer* renderer);
	bool CleanUp();
	
	void ProcessShadows(Camera* camera, const RenderContext& render_context);
	ShadowSpotlight* GetShadowSpotlight() const { return m_ShadowSpotlight; }

	const CU::Matrix44f& GetOrientation() const;
	const CU::Matrix44f& GetOrientation();


private:

	CU::Matrix44f m_Orientation;

	Renderer* m_Renderer = nullptr;
	Effect* m_RenderToDepthTechnique = nullptr;
	Texture* m_DepthTexture = nullptr;
	ShadowSpotlight* m_ShadowSpotlight = nullptr;


};
