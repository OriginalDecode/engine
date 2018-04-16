#pragma once
#include "IPostprocessPass.h"

namespace graphics
{
	class Viewport;
}

class DownsamplePass : public IPostprocessPass
{
public:

	DownsamplePass() = default;
	~DownsamplePass() override;

	void Initiate(s32 sample_count, s32 width, s32 height, TextureDesc& desc);


	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;

	Texture* GetSample(s32 index) const;

	s32 SampleCount() const { return m_Downsamples.Size(); }

private:

	void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);

	s32 m_Width = 0;
	s32 m_Height = 0;
	Effect*	m_Downsampler = nullptr;
	graphics::Viewport* m_Viewport;

	CU::GrowingArray<Texture*> m_Downsamples;

};

