#pragma once

#include <Engine/IPostprocessPass.h>

class SSAOPass : public IPostprocessPass
{
public:
	SSAOPass();
	~SSAOPass() override;

	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;
private:


	Texture*	m_SSAOTexture = nullptr;
	Texture*	m_DebugTexture	= nullptr;
	Texture*	m_Test = nullptr;

	IBuffer* m_cbSSAO = nullptr;
	struct cbSSAO
	{
		CU::Matrix44f m_Projection;
		CU::Matrix44f m_View;
	} m_SSAOStruct;

};
