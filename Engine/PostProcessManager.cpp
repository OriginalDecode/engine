#include "stdafx.h"
#include "PostProcessManager.h"


void PostProcessManager::Process(Hex::Texture*  current_frame_texture)
{

	if (m_PassFlags & HDR)
		m_HDRPass.Process(current_frame_texture);

	if (m_PassFlags & BLOOM)
		m_BloomPass.Process(current_frame_texture);

	if (m_PassFlags & MOTIONBLUR)
		m_MotionBlurPass.Process(current_frame_texture);
}

void PostProcessManager::SetPassesToProcess(s32 pass_flags)
{
	m_PassFlags |= pass_flags;
}

void PostProcessManager::RemovePassToProcess(s32 pass_flag)
{
	m_PassFlags &= ~pass_flag;
}
