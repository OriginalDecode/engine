#pragma once

#include "../engine/IGraphicsContext.h"
#include "../engine/engine_shared.h"
#include <CommonLib/DataStructures/GrowingArray.h>
#include "graphics_flags.h"

enum eRenderPass
{
	GBUFFER,
	
	ATMOSPHERE,

	AMBIENT, // IBL

	LIGHTPASS, // Real light

	POSTPROCESS,
	NOF_RENDERPASS
};

namespace graphics
{
	class IGraphicsContext;
	class RenderPass
	{
	public:
		RenderPass(eRenderPass pass, IGraphicsContext* ctx) 
			: m_Context(ctx)
			, m_RenderPass(pass) 
		{ 
		};
		
		virtual ~RenderPass() 
		{
		};

		
		virtual void Update() = 0;

	protected:

		virtual void SetDepthStencilState(eDepthStencilState depth_state, unsigned int max_depth);
		virtual void SetRasterState(eRasterizer raster_state);
		virtual void SetBlendState(eBlendStates blend_state, const float blend_color[4] = blendcolor::black, unsigned int mask = 0xFFFFFFFF);
		virtual void AddSamplerState(eSamplerStates sampler_state, SamplerState::ShaderBinding shader_binding);
		virtual void SetSamplerState(const SamplerState& samplerstate, s32 index, s32 count);

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

		CU::StaticArray<CU::GrowingArray<SamplerState>, 6> m_SamplerStates;


		IRenderTargetView* m_RenderTarget = nullptr;
		IDepthStencilView* m_DepthView = nullptr;


	};

	void RenderPass::SetDepthStencilState(eDepthStencilState depth_state, unsigned int max_depth)
	{
		m_DepthStencilState = depth_state;
		m_MaxDepth = max_depth;
	}

	void RenderPass::SetRasterState(eRasterizer raster_state)
	{
		m_RasterState = raster_state;
	}

	void RenderPass::SetBlendState(eBlendStates blend_state, const float blend_color[4] /*= blendcolor::black*/, unsigned int mask /*= 0xFFFFFFFF*/)
	{
		m_BlendState = blend_state;
		m_BlendMask = mask;

		m_BlendColor[0] = blend_color[0];
		m_BlendColor[1] = blend_color[1];
		m_BlendColor[2] = blend_color[2];
		m_BlendColor[3] = blend_color[3];
	}

	void RenderPass::AddSamplerState(eSamplerStates sampler_state, SamplerState::ShaderBinding shader_binding)
	{
		m_SamplerStates.Add(SamplerState(sampler_state, shader_binding));
	}

	void RenderPass::SetSamplerState(const SamplerState& samplerstate, s32 index, s32 count)
	{
		switch (samplerstate.m_ShaderBinding)
		{
			case SamplerState::VERTEX_SHADER: 
			{
				m_Context->VSSetSamplerState(index, count, samplerstate.m_SamplerState);
			} break;

			case SamplerState::FRAGMENT_SHADER: 
			{
				m_Context->PSSetSamplerState(index, count, samplerstate.m_SamplerState);
			} break;

			case SamplerState::GEOMETRY_SHADER: 
			{
				m_Context->GSSetSamplerState(index, count, samplerstate.m_SamplerState);

			} break;

			case SamplerState::HULL_SHADER: 
			{
				m_Context->HSSetSamplerState(index, count, samplerstate.m_SamplerState);

			} break;

			case SamplerState::DOMAIN_SHADER: 
			{
				m_Context->DSSetSamplerState(index, count, samplerstate.m_SamplerState);

			} break;

			case SamplerState::COMPUTE_SHADER: 
			{
				m_Context->CSSetSamplerState(index, count, samplerstate.m_SamplerState);
			} break;

			default:
				break;
		}
	}
};
