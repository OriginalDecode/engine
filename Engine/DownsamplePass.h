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

	void Initiate(int32 sample_count, int32 width, int32 height, TextureDesc& desc, bool register_textures = false);


	void Process(Texture* scene, const graphics::RenderContext& rc) override;
	void OnResize() override;

	Texture* GetSample(int32 index) const;

	int32 SampleCount() const { return m_Downsamples.Size(); }

private:

	void Downsample(IRenderTargetView* render_target, IShaderResourceView* source);

	int32 m_Width = 0;
	int32 m_Height = 0;
	Effect*	m_Downsampler = nullptr;
	graphics::Viewport* m_Viewport;

	CU::GrowingArray<Texture*> m_Downsamples;

};

