#include "LightPass.h"

#include <Engine/Engine.h>
#include <Engine/IGraphicsContext.h>

namespace graphics
{
	LightPass::LightPass(IRenderTargetView* rtv, IDepthStencilView* dsv)
		: RenderPass(eRenderPass::LIGHTPASS, &Engine::GetAPI()->GetContext())
	{
		m_RenderTarget = rtv;
		m_DepthView = dsv;


		

		SetBlendState(ALPHA_BLEND);

		SetRasterState(CULL_NONE);

		SetDepthStencilState(Z_ENABLED, 1);

		AddSamplerState(LINEAR_WRAP, SamplerState::FRAGMENT_SHADER);


	}

	LightPass::~LightPass()
	{
	}

	void LightPass::Update()
	{
		m_Context->OMSetRenderTargets(1, m_RenderTarget, &m_DepthStencilState);


		for (const SamplerState& state : m_SamplerStates)
		{
			SetSamplerState(state, 0 /* static variable for each type that has to be reset */, 0 /*how many samplers are we adding? */);
		}

		//Fetch all directional lights

		//Fetch all pointlights

		//Fetch all spotlights
	}


};