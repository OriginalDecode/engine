#include "stdafx.h"
#include "RenderNodeGeneral.h"
#include <engine/Model.h>
#include <engine/Engine.h>
#include <engine/AssetsContainer.h>
namespace graphics
{
	u64 RenderNodeGeneral::Type = cl::Hash(STRINGIFY(RenderNodeGeneral));
	RenderNodeGeneral::RenderNodeGeneral()
	{
		Engine* engine = Engine::GetInstance();
		m_Shaders[VERTEX] = engine->GetAssetsContainer()->GetShader("Data/Shaders/deferred_base_instanced.vsmain");
		m_Shaders[PIXEL] = engine->GetAssetsContainer()->GetShader("Data/Shaders/pbl_debug.psmain");

#ifdef _DEBUG
		m_Shaders[VERTEX]->RegisterReload(this);
		m_Shaders[PIXEL]->RegisterReload(this);
#endif
	}

	RenderNodeGeneral::~RenderNodeGeneral()
	{
	}

	void RenderNodeGeneral::Draw(const RenderContext& rc)
	{
		rc.GetContext().SetVertexShader(m_Shaders[VERTEX]);
		rc.GetContext().SetPixelShader(m_Shaders[PIXEL]);


		rc.GetContext().SetDepthState(graphics::Z_ENABLED, 1);
		rc.GetContext().SetRasterState(graphics::CULL_NONE);
		rc.GetContext().SetBlendState(graphics::BLEND_FALSE);
		rc.GetContext().PSSetSamplerState(0, 1, graphics::MSAA_x1);
		rc.GetContext().VSSetSamplerState(0, 1, graphics::MSAA_x1);

		Model * model = nullptr;
		for (auto& object : m_Models)
		{
			std::vector<ModelInstance>& list = object.second;
			
			for (ModelInstance& instance : list)
			{
				model = static_cast<Model*>(instance.GetModel());
				model->AddOrientation(instance.GetOrientation());
				instance.UpdateMaterial();
			}

			//if(Surface* s = model->GetSurface())
			//	s->Activate(rc);

			model->Render(rc);
			model = nullptr;
		}
	}

	void RenderNodeGeneral::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}


	void RenderNodeGeneral::AddInstance(const ModelInstance instance)
	{
		const u64 key = instance.GetMaterialKey();
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