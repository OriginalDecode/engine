#include "stdafx.h"

#include "IGraphicsContext.h"

#include <Engine/Engine.h>
#include <Engine/IGraphicsAPI.h>

namespace graphics
{

	IGraphicsContext::~IGraphicsContext()
	{
		IGraphicsAPI* api = Engine::GetAPI();
		for (ISamplerState* sampler_state : m_SamplerStates)
		{
			api->ReleasePtr(sampler_state);
			sampler_state = nullptr;
		}

		for (IDepthStencilState* depthstencil_state : m_DepthStencilStates)
		{
			api->ReleasePtr(depthstencil_state);
			depthstencil_state = nullptr;
		}

		for (IRasterizerState* raster_state : m_RasterizerStates)
		{
			api->ReleasePtr(raster_state);
			raster_state = nullptr;
		}

		for (IBlendState* blend_state : m_BlendStates)
		{
			api->ReleasePtr(blend_state);
			blend_state = nullptr;
		}
	}

};