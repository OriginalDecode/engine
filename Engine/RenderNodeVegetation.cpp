#include "stdafx.h"
#include "RenderNodeVegetation.h"
#include <engine/Engine.h>
#include <engine/AssetsContainer.h>
#include <engine/Model.h>
#include "DebugRenderer.h"

namespace graphics
{
	RenderNodeVegetation::RenderNodeVegetation()
	{

		Engine* engine = Engine::GetInstance();
		AssetsContainer* ac = AssetsContainer::GetInstance();
		const uint64 vtx = ac->LoadShader("deferred_base_instanced.vs", "main");
		const uint64 fragment = ac->LoadShader("pbl_debug.ps", "main");
		const uint64 depth_frag = ac->LoadShader("depth_prepass.ps", "main");

		m_Shaders[EShaderType_VERTEX] = ac->GetShader(vtx);
		m_Shaders[EShaderType_PIXEL] = ac->GetShader(fragment);
		m_DepthShader = ac->GetShader(depth_frag);

#ifdef _DEBUG
		m_Shaders[EShaderType_VERTEX]->RegisterReload(this);
		m_Shaders[EShaderType_PIXEL]->RegisterReload(this);
		m_DepthShader->RegisterReload(this);
#endif
	}


	RenderNodeVegetation::~RenderNodeVegetation()
	{
	}

	void RenderNodeVegetation::Draw(const RenderContext& rc)
	{
		PROFILE_FUNCTION(profiler::colors::Red);
		auto& ctx = rc.GetContext();
		ctx.SetVertexShader(m_Shaders[EShaderType_VERTEX]);

		if (!m_DrawDepth)
		{
			ctx.SetPixelShader(m_Shaders[EShaderType_PIXEL]);
			ctx.SetDepthState(graphics::Z_EQUAL, 1);
		}
		else
		{
			ctx.SetPixelShader(m_DepthShader);
			ctx.SetDepthState(graphics::Z_ENABLED, 1);
		}

		ctx.SetRasterState(graphics::CULL_BACK);
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
				if (i == 0)
				{
					instance.UpdateMaterial();
				}

				model = static_cast<Model*>(instance.GetModel());
				model->AddOrientation(instance.GetOrientation());
				const CU::Vector3f& pos = instance.GetOrientation().GetPosition();

				DebugRenderer::GetInstance()->DrawPosition(pos);
			}

			model->Render(rc);
			model = nullptr;
		}

		m_DrawDepth = !m_DrawDepth;
	}

	
	void RenderNodeVegetation::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}

	void RenderNodeVegetation::AddInstance(const ModelInstance instance)
	{
		const uint64 key = instance.GetMaterialKey();
		auto it = m_Models.find(key);
		if (it == m_Models.end())
		{
			m_Models.emplace(key, std::vector<ModelInstance>());
		}

		it = m_Models.find(key);
		if (it != m_Models.end())
		{
			it->second.push_back(instance);
		}

	}

};