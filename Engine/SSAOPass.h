#pragma once
#include <Engine/Quad.h>
class SSAOPass
{
public:
	SSAOPass();
	~SSAOPass();

	void Process(Texture* scene_texture, const graphics::RenderContext& render_context);
	void OnResize();
private:

	WindowSize	m_WindowSize;

	Quad*		m_ScreenQuad = nullptr;

	Texture*	m_SSAOTexture = nullptr;
	Texture*	m_DebugTexture	= nullptr;
	Texture*	m_Test = nullptr;

	Effect*		m_SSAOShader	= nullptr;
	
	IBuffer* m_cbSSAO = nullptr;
	struct cbSSAO
	{
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
	} m_SSAOStruct;

};
