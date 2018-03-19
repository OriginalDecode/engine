#pragma once

#include <Engine/engine_shared.h>

class Renderer;
class Effect;
class ShadowDirectional;
class ShadowSpotlight;

class ShadowPass
{
public:

	bool Initiate(Renderer* renderer);
	bool CleanUp();
	
	void ProcessShadows(ShadowSpotlight* shadow_spotlight);
	void ProcessShadows(ShadowDirectional* shadow_directional, const graphics::RenderContext& rc);

	void Activate();
	void DeActivate();



private:
	void ProcessShadows(Camera* camera);


	Renderer* m_Renderer = nullptr;
	Effect* m_RenderToDepth = nullptr;
	Effect* m_RenderToDepthInstanced = nullptr;
	Texture* m_DepthTexture = nullptr;


};
