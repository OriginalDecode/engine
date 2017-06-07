#pragma once

#include <Engine/engine_shared.h>

class ShadowDirectional
{
public:
	ShadowDirectional() = default;
	void Initiate(float buffer_size);
	void CleanUp();


	Camera* GetCamera() { return m_Camera; }
	Camera* GetCamera() const { return m_Camera; }

	void SetViewport();
	void ClearTexture(const RenderContext& render_context);
	void SetTargets(const RenderContext& render_context);

	void SetOrientation(const CU::Matrix44f& orientation);
	const CU::Matrix44f& GetOrientation() { return m_Camera->GetOrientation(); }



	const CU::Matrix44f& GetMVP();
	

private:
	const float m_Clear[4] = { 0.f, 0.f, 0.f, 0.f };

	Viewport*	m_Viewport				= nullptr;
	Camera*		m_Camera				= nullptr;
	Texture*	m_ShadowDepth			= nullptr;
	Texture*	m_ShadowDepthStencil	= nullptr;
};

