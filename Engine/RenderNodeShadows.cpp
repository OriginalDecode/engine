#include "stdafx.h"
#include "RenderNodeShadows.h"
#include "ModelInstance.h"
#include <Engine/AssetsContainer.h>

namespace graphics
{
	RenderNodeShadows::RenderNodeShadows()
	{
		m_DirectionalShadow.Initiate(2048.f);

		const uint64 vtx = AssetsContainer::GetInstance()->LoadShader("deferred_base_instanced.vs", "main");
		const uint64 fragment = AssetsContainer::GetInstance()->LoadShader("render_depth.ps", "main");
		m_Shaders[0] = AssetsContainer::GetInstance()->GetShader(vtx);
		m_Shaders[1] = AssetsContainer::GetInstance()->GetShader(fragment);

		m_Shaders[0]->RegisterReload(this);
		m_Shaders[1]->RegisterReload(this);

	}


	RenderNodeShadows::~RenderNodeShadows()
	{
		m_DirectionalShadow.CleanUp();
	}

	void RenderNodeShadows::Draw(const RenderContext& rc)
	{
		PROFILE_FUNCTION(profiler::colors::Red);
		auto& ctx = rc.GetContext();

		ctx.SetVertexShader(m_Shaders[0]);
		ctx.SetPixelShader(m_Shaders[1]);



		ctx.SetRasterState(graphics::CULL_NONE);
		ctx.SetBlendState(graphics::BLEND_FALSE);
		ctx.PSSetSamplerState(0, 1, graphics::MSAA_x1);
		ctx.VSSetSamplerState(0, 1, graphics::MSAA_x1);

		Model * model = nullptr;
		for (auto& object : m_Models)
		{
			std::vector<ModelInstance>& list = object.second;

			for (int i = 0; i < list.size(); i++)
			{
				ModelInstance& instance = list[i];
				model = static_cast<Model*>(instance.GetModel());
				model->AddOrientation(instance.GetOrientation());
			}

			model->ShadowRenderInstanced(rc);
			model = nullptr;
		}
	}	

	void RenderNodeShadows::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}

	void RenderNodeShadows::AddInstance(const ModelInstance instance)
	{
		const uint64 key = cl::Hash(instance.GetModelName());
		auto it = m_Models.find(key);
		if (it == m_Models.end())
		{
			m_Models.insert(std::make_pair(key, std::vector<ModelInstance>()));
		}

		it = m_Models.find(key);
		if (it != m_Models.end())
		{
			it->second.push_back(instance);
		}
	}

};