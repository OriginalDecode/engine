#pragma once

#include "HDRPass.h"
#include "MotionBlurPass.h"
#include "BloomPass.h"
#include "SSAOPass.h"
#include "EdgeDetectionPass.h"
#include "SSRPass.h"
#include "DownsamplePass.h"

#include <Engine/engine_shared.h>

class Texture;
namespace graphics
{
	class RenderContext;
}

class PostProcessManager
{
public:
	enum ePasses
	{
		BLOOM			= 1 << 0,
		MOTIONBLUR		= 1 << 1,
		HDR				= 1 << 2,
		SSAO			= 1 << 3,
		EDGE_DETECTION	= 1 << 4,
		SSR				= 1 << 5,
		DOWNSAMPLE		= 1 << 6,
	};

	PostProcessManager();
	~PostProcessManager();

	s32 GetFlags() const { return m_PassFlags; }

	void Process(Texture* current_frame_texture, const graphics::RenderContext& render_context);
	void Process(Texture* pTexture, s32 process_flag, const graphics::RenderContext& rc);

	void Initiate();

	void SetPassesToProcess(s32 pass_flags);
	void RemovePassToProcess(s32 pass_flag);

	HDRPass& GetHDRPass() { return m_HDRPass; }

private:
	s32					m_PassFlags;
	HDRPass				m_HDRPass;
	MotionBlurPass		m_MotionBlurPass;
	BloomPass			m_BloomPass;
	SSAOPass			m_SSAOPass;
	EdgeDetectionPass	m_EdgeDetectionPass;
	SSRPass				m_SSRPass;

};

