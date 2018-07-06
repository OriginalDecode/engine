#pragma once

#include "../engine/IGraphicsContext.h"
#include "../engine/engine_shared.h"
enum eRenderPass
{
	GBUFFER,
	ATMOSPHERE,
	AMBIENT,
	DIRECTIONALLIGHT,
	SPOTLIGHT,
	POINTLIGHT,
	POSTPROCESS,
	NOF_RENDERPASS
};

namespace graphics
{
	class IGraphicsContext;
	class IRenderPass
	{
	public:
		IRenderPass(eRenderPass pass, IGraphicsContext* ctx) 
			: m_Context(ctx)
			, m_RenderPass(pass) 
		{ 
		};
		
		virtual ~IRenderPass() 
		{
		};

		virtual void SetDepthStencilState(eDepthStencilState depth_state, unsigned int max_depth) 
		{
			m_DepthStencilState = depth_state;  
			m_MaxDepth = max_depth;
		}

		virtual void SetRasterState(eRasterizer raster_state)
		{
			m_RasterState = raster_state;
		}

		virtual void SetBlendState(eBlendStates blend_state, const float blend_color[4], unsigned int mask)
		{
			m_BlendState = blend_state;
			m_BlendMask = mask;

			m_BlendColor[0] = blend_color[0];
			m_BlendColor[1] = blend_color[1];
			m_BlendColor[2] = blend_color[2];
			m_BlendColor[3] = blend_color[3];
		}

		virtual void SetSamplerState(eSamplerStates sampler_state)
		{
			m_SamplerState = sampler_state;
		}

	protected:
		eRenderPass m_RenderPass = NOF_RENDERPASS;
		IGraphicsContext* m_Context = nullptr;


		//DepthState
		eDepthStencilState m_DepthStencilState = Z_ENABLED;
		unsigned int m_MaxDepth = 0;


		//RasterState
		eRasterizer m_RasterState = CULL_BACK;



		//BlendState
		eBlendStates m_BlendState = NO_BLEND;
		float m_BlendColor[4];
		unsigned int m_BlendMask = 0;

		eSamplerStates m_SamplerState = LINEAR_CLAMP;
		

		IRenderTargetView* m_RenderTarget = nullptr;
		IDepthStencilView* m_Depthview = nullptr;


	};
};
