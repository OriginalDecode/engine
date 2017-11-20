#include "stdafx.h"
#include "PostProcessManager.h"
#include <Engine/profile_defines.h>


PostProcessManager::PostProcessManager()
{
	m_HDRPass.Initiate();
}

PostProcessManager::~PostProcessManager()
{
	m_HDRPass.CleanUp();
}

void PostProcessManager::Process(Texture* current_frame_texture, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Green);


	if (m_PassFlags & SSAO)
		m_SSAOPass.Process(current_frame_texture, render_context);

	if (m_PassFlags & HDR)
		m_HDRPass.Process(current_frame_texture, render_context);

	if (m_PassFlags & BLOOM)
		m_BloomPass.Process(current_frame_texture, render_context);

	if (m_PassFlags & MOTIONBLUR)
		m_MotionBlurPass.Process(current_frame_texture, render_context);
}

void PostProcessManager::Process(Texture* pTexture, s32 process_flag, const graphics::RenderContext& rc)
{

	if (process_flag & SSAO)
		m_SSAOPass.Process(pTexture, rc);

	if (process_flag & HDR)
		m_HDRPass.Process(pTexture, rc);

	if (process_flag & BLOOM)
		m_BloomPass.Process(pTexture, rc);

	if (process_flag & MOTIONBLUR)
		m_MotionBlurPass.Process(pTexture, rc);

	if (process_flag & EDGE_DETECTION)
		m_EdgeDetectionPass.Process(pTexture, rc);
	

}

void PostProcessManager::SetPassesToProcess(s32 pass_flags)
{
	m_PassFlags |= pass_flags;
}

void PostProcessManager::RemovePassToProcess(s32 pass_flag)
{
	m_PassFlags &= ~pass_flag;
}
