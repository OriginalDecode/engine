#pragma once
#include "IPostprocessPass.h"
class DownsamplePass : public IPostprocessPass
{
public:
	DownsamplePass(s32 sample_count, s32 width, s32 height);
	~DownsamplePass() override;


	void Process(s32 sample_count, Texture* pTexture, const graphics::RenderContext& rc);
	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;

private:

	Effect*	m_Downsampler = nullptr;

};

