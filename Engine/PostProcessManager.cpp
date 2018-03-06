#include "stdafx.h"
#include "PostProcessManager.h"
#include <Engine/profile_defines.h>


PostProcessManager::PostProcessManager()
{
}

PostProcessManager::~PostProcessManager()
{
	m_HDRPass.CleanUp();
}

void PostProcessManager::Initiate()
{
	m_HDRPass.Initiate();
	m_EdgeDetectionPass.Initiate();
}


void PostProcessManager::Process(Texture* current_frame_texture, const graphics::RenderContext& render_context)
{
	PROFILE_FUNCTION(profiler::colors::Green);
	Process(current_frame_texture, m_PassFlags, render_context);
}

void PostProcessManager::Process(Texture* pTexture, s32 process_flag, const graphics::RenderContext& rc) //if you want to run a specific pass
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
	
	if (process_flag & SSR)
		m_SSRPass.Process(pTexture, rc);
}

void PostProcessManager::SetPassesToProcess(s32 pass_flags)
{
	m_PassFlags |= pass_flags;
}

void PostProcessManager::RemovePassToProcess(s32 pass_flag)
{
	m_PassFlags &= ~pass_flag;
}
