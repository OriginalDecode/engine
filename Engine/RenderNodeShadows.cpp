#include "stdafx.h"
#include "RenderNodeShadows.h"
#include "ModelInstance.h"
#include <Engine/AssetsContainer.h>
#include "Engine/Shadow_Directional.h"

namespace graphics
{
	RenderNodeShadows::RenderNodeShadows()
	{
		const uint64 vtx = AssetsContainer::GetInstance()->LoadShader("render_depth.vs", "instance_main");
		const uint64 fragment = AssetsContainer::GetInstance()->LoadShader("render_depth.ps", "discard_alpha_0");
		m_Shaders[0] = AssetsContainer::GetInstance()->GetShader(vtx);
		m_Shaders[1] = AssetsContainer::GetInstance()->GetShader(fragment);

#ifdef _DEBUG
		m_Shaders[0]->RegisterReload(this);
		m_Shaders[1]->RegisterReload(this);
#endif
	}


	RenderNodeShadows::~RenderNodeShadows()
	{
	}

	void RenderNodeShadows::Init(ShadowDirectional* shadowDirectional)
	{
		m_DirectionalShadow = shadowDirectional; 
	}

	void RenderNodeShadows::Draw(const RenderContext& rc)
	{
		if (m_Processed)
		{
			m_Processed = false;
			return;
		}


		PROFILE_FUNCTION(profiler::colors::Red);
		
		m_DirectionalShadow->SetViewport();
		m_DirectionalShadow->ClearTexture();
		m_DirectionalShadow->SetTargets();
		m_DirectionalShadow->Update();
		m_DirectionalShadow->GetConstBuffer().Bind(0, EShaderTypeFlag_VERTEX | EShaderTypeFlag_DOMAIN, rc);
		
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

		Engine::GetAPI()->ResetViewport();
		m_Processed = true;

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