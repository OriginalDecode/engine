#include "stdafx.h"
#include "RenderNodeShadows.h"
#include "ModelInstance.h"


namespace graphics
{
	RenderNodeShadows::RenderNodeShadows()
	{
	}


	RenderNodeShadows::~RenderNodeShadows()
	{
	}

	void RenderNodeShadows::Draw(const RenderContext& rc)
	{
		PROFILE_FUNCTION(profiler::colors::Red);
		auto& ctx = rc.GetContext();
		/*ctx.SetVertexShader(m_Shaders[EShaderType_VERTEX]);

		if (!m_DrawDepth)
		{
			ctx.SetPixelShader(m_Shaders[EShaderType_PIXEL]);
			ctx.SetDepthState(graphics::Z_EQUAL, 1);
		}
		else
		{
			ctx.SetPixelShader(m_DepthShader);
			ctx.SetDepthState(graphics::Z_ENABLED, 1);
		}*/

		ctx.SetRasterState(graphics::CULL_NONE);
		ctx.SetBlendState(graphics::BLEND_FALSE);
		ctx.PSSetSamplerState(0, 1, graphics::MSAA_x1);
		ctx.VSSetSamplerState(0, 1, graphics::MSAA_x1);

		for (auto& instance : m_Models)
		{
			Model* model = static_cast<Model*>(instance.GetModel());
			model->AddOrientation(instance.GetOrientation());
		}

	}	

	void RenderNodeShadows::Reload(CompiledShader* shader)
	{
		/**/
	}

	void RenderNodeShadows::AddInstance(const ModelInstance instance)
	{
		m_Models.push_back(instance);
	}

};