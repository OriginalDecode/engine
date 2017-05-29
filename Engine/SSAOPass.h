#pragma once
#include <Engine/Quad.h>
class SSAOPass
{
public:
	SSAOPass() = default;

	void Initiate();
	void CleanUp(); 
	void Process();
private:
	Quad		m_ScreenQuad;
	WindowSize	m_WindowSize;
	Texture*	m_SSAOTexture	= nullptr;
	Effect*		m_SSAOShader	= nullptr;
	Effect*		m_BlurShader	= nullptr;

};

