#include "LightPass.h"

#include <Engine/Engine.h>
#include <Engine/IGraphicsContext.h>

namespace graphics
{
	LightPass::LightPass(IRenderTargetView* rtv, IDepthStencilView* dsv)
		: IRenderPass(eRenderPass::LIGHTPASS, &Engine::GetAPI()->GetContext())
	{
		m_RenderTarget = rtv;
		m_DepthView = dsv;


		

		SetSamplerState(LINEAR_WRAP);
		SetBlendState(LIGHT_BLEND);
		SetRasterState(CULL_NONE);



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