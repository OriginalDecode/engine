#include "stdafx.h"
#include "PostProcessManager.h"


void PostProcessManager::Initiate()
{
	m_Engine = Engine::GetInstance();
	m_HDRPass.Initiate();
}

void PostProcessManager::CleanUp()
{
	m_HDRPass.CleanUp();
}

void PostProcessManager::Process(Texture*  current_frame_texture)
{
	m_Engine->DisableZ();

	if (m_PassFlags & HDR)
		m_HDRPass.Process(current_frame_texture);

	if (m_PassFlags & BLOOM)
		m_BloomPass.Process(current_frame_texture);

	if (m_PassFlags & MOTIONBLUR)
		m_MotionBlurPass.Process(current_frame_texture);

	m_Engine->EnableZ();
}

void PostProcessManager::SetPassesToProcess(s32 pass_flags)
{
	m_PassFlags |= pass_flags;
}

void PostProcessManager::RemovePassToProcess(s32 pass_flag)
{
	m_PassFlags &= ~pass_flag;
}
