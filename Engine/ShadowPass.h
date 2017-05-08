#pragma once

#include <Engine/snowblind_shared.h>

#include <Engine/Camera.h>
#include <functional>

class Renderer;
class Effect;
class ShadowDirectional;
class ShadowSpotlight;
class ShadowPass
{
public:

	bool Initiate(Renderer* renderer);
	bool CleanUp();
	
	void ProcessShadows(ShadowSpotlight* shadow_spotlight, const RenderContext& render_context);
	void ProcessShadows(ShadowDirectional* shadow_directional, const RenderContext& render_context);

	void Activate();
	void DeActivate();

	const CU::Matrix44f& GetOrientation() const;
	const CU::Matrix44f& GetOrientation();


private:
	void ProcessShadows(Camera* camera, const RenderContext& render_context);

	CU::Matrix44f m_Orientation;

	Renderer* m_Renderer = nullptr;
	Effect* m_RenderToDepthTechnique = nullptr;
	Texture* m_DepthTexture = nullptr;


};
