#pragma once

#include <Engine/engine_shared.h>

class Camera;
class Texture;
namespace graphics
{
	class Viewport;
};

class ShadowDirectional
{
public:
	ShadowDirectional() = default;
	void Initiate(float buffer_size);
	void CleanUp();


	Camera* GetCamera() { return m_Camera; }
	Camera* GetCamera() const { return m_Camera; }

	void SetViewport();
	void ClearTexture();
	void SetTargets();

	void SetOrientation(const CU::Matrix44f& orientation);
	const CU::Matrix44f& GetOrientation() { return m_Camera->GetOrientation(); }



	CU::Matrix44f GetMVP();
	

	Texture* GetDepthTexture() { return m_ShadowDepth; }
	Texture* GetDepthStencilTexture() { return m_ShadowDepthStencil; }

private:

	graphics::Viewport*	m_Viewport				= nullptr;
	Camera*		m_Camera				= nullptr;
	Texture*	m_ShadowDepth			= nullptr;
	Texture*	m_ShadowDepthStencil	= nullptr;
};

