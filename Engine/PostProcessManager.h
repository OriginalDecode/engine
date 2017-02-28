#pragma once

#include "HDRPass.h"
#include "MotionBlurPass.h"
#include "BloomPass.h"

namespace Hex
{
	class Texture;
	class Effect;
	class Engine;
};

#define BITFLAG(x) (1 << x)

class PostProcessManager
{
public:
	enum ePasses
	{
		BLOOM = BITFLAG(0),
		MOTIONBLUR = BITFLAG(1),
		HDR = BITFLAG(2),
	};


	PostProcessManager() = default;
	void Initiate();
	void CleanUp();

	void Process(Hex::Texture* current_frame_texture);

	void SetPassesToProcess(s32 pass_flags);
	void RemovePassToProcess(s32 pass_flag);

	HDRPass& GetHDRPass() { return m_HDRPass; }

private:
	s32				m_PassFlags;
	HDRPass			m_HDRPass;
	MotionBlurPass	m_MotionBlurPass;
	BloomPass		m_BloomPass;

	Hex::Engine* m_Engine = nullptr;
};

