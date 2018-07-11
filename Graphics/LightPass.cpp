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

		//AddSamplerState(LINEAR_WRAP, SamplerState::FRAGMENT_SHADER);
		//AddSamplerState(MSAA_x16, SamplerState::FRAGMENT_SHADER);


	}

	LightPass::~LightPass()
	{
	}

	void LightPass::Update()
	{
		m_Context->OMSetRenderTargets(1, m_RenderTarget, &m_DepthStencilState);


		//Fetch all directional lights

		//Fetch all pointlights

		//Fetch all spotlights
	}


};