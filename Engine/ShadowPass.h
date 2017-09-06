#pragma once

#include <Engine/engine_shared.h>

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
	
	void ProcessShadows(ShadowSpotlight* shadow_spotlight);
	void ProcessShadows(ShadowDirectional* shadow_directional);

	void Activate();
	void DeActivate();

	const CU::Matrix44f& GetOrientation() const;
	const CU::Matrix44f& GetOrientation();


private:
	void ProcessShadows(Camera* camera);

	CU::Matrix44f m_Orientation;

	Renderer* m_Renderer = nullptr;
	Effect* m_RenderToDepth = nullptr;
	Camera* m_Camera = nullptr;

	Texture* m_DepthTexture = nullptr;


};
