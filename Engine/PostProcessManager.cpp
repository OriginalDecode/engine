#include "stdafx.h"
#include "PostProcessManager.h"


PostProcessManager::PostProcessManager()
{
	m_HDRPass.Initiate();
}

PostProcessManager::~PostProcessManager()
{
	m_HDRPass.CleanUp();
}

void PostProcessManager::Process(Texture*  current_frame_texture, const graphics::RenderContext& render_context)
{
#ifdef _PROFILE
	EASY_FUNCTION(profiler::colors::Green);
#endif


#ifdef _PROFILE
	EASY_BLOCK("ProcessSSAO");
#endif
	if(m_PassFlags & SSAO )
		m_SSAOPass.Process(current_frame_texture, render_context);
#ifdef _PROFILE
	EASY_END_BLOCK;
	EASY_BLOCK("ProcessHDR");
#endif
	if (m_PassFlags & HDR)
		m_HDRPass.Process(current_frame_texture, render_context);
#ifdef _PROFILE
	EASY_END_BLOCK;
	EASY_BLOCK("ProcessBloom");
#endif
	if (m_PassFlags & BLOOM)
		m_BloomPass.Process(current_frame_texture, render_context);
#ifdef _PROFILE
	EASY_END_BLOCK;
	EASY_BLOCK("ProcessMotionBlur");
#endif
	if (m_PassFlags & MOTIONBLUR)
		m_MotionBlurPass.Process(current_frame_texture, render_context);
#ifdef _PROFILE
	EASY_END_BLOCK;
#endif
}

void PostProcessManager::SetPassesToProcess(s32 pass_flags)
{
	m_PassFlags |= pass_flags;
}

void PostProcessManager::RemovePassToProcess(s32 pass_flag)
{
	m_PassFlags &= ~pass_flag;
}
