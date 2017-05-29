#pragma once
#include <Engine/Quad.h>
class SSAOPass
{
public:
	SSAOPass() = default;

	void Initiate();
	void CleanUp(); 
	void Process(Texture* scene_texture);
private:

	Quad		m_ScreenQuad;
	WindowSize	m_WindowSize;

	Texture*	m_SSAOTexture = nullptr;
	Texture*	m_DebugTexture	= nullptr;
	Texture*	m_Test = nullptr;


	Effect*		m_SSAOShader	= nullptr;
	Effect*		m_BlurShader[2];


	IBuffer* m_cbSSAO = nullptr;
	struct cbSSAO
	{
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
	} m_SSAOStruct;


	struct cbBlur
	{
		float m_Size;
		CU::Vector3f dummy;
	} m_BlurStruct;

	IBuffer* m_cbBlur = nullptr;


};

