#include "stdafx.h"
#include "RenderNodeVegetation.h"
#include <engine/Engine.h>
#include <engine/AssetsContainer.h>
#include <engine/Model.h>

namespace graphics
{
	u64 RenderNodeVegetation::Type = cl::Hash(STRINGIFY(RenderNodeVegetation));
	RenderNodeVegetation::RenderNodeVegetation()
	{

		auto engine = Engine::GetInstance();

		m_Shaders[VERTEX] = engine->GetAssetsContainer()->GetShader("Data/Shaders/deferred_base.vsmain");
		m_Shaders[PIXEL] = engine->GetAssetsContainer()->GetShader("Data/Shaders/pbl_debug.psmain");

#ifdef _DEBUG
		m_Shaders[VERTEX]->RegisterReload(this);
		m_Shaders[PIXEL]->RegisterReload(this);
#endif
	}


	RenderNodeVegetation::~RenderNodeVegetation()
	{
	}

	void RenderNodeVegetation::Draw(const RenderContext& rc)
	{
		rc.GetContext().SetVertexShader(m_Shaders[VERTEX]);
		rc.GetContext().SetPixelShader(m_Shaders[PIXEL]);

		for (const ModelInstance& model_instance : m_Models)
		{
			model_instance.Draw(rc);
		}
	}

	
	void RenderNodeVegetation::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}

	void RenderNodeVegetation::AddInstance(const ModelInstance instance)
	{
		m_Models.push_back(instance);
	}

};