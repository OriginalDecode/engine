#pragma once

#include "HDRPass.h"
#include "MotionBlurPass.h"
#include "BloomPass.h"
#include "SSAOPass.h"

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
		BLOOM = BITFLAG(0),
		MOTIONBLUR = BITFLAG(1),
		HDR = BITFLAG(2),
		SSAO = BITFLAG(3),
	};

	PostProcessManager();
	~PostProcessManager();

	s32 GetFlags() const { return m_PassFlags; }

	void Process(Texture* current_frame_texture, const graphics::RenderContext& render_context);

	void SetPassesToProcess(s32 pass_flags);
	void RemovePassToProcess(s32 pass_flag);

	HDRPass& GetHDRPass() { return m_HDRPass; }

private:
	s32				m_PassFlags;
	HDRPass			m_HDRPass;
	MotionBlurPass	m_MotionBlurPass;
	BloomPass		m_BloomPass;
	SSAOPass		m_SSAOPass;
};

