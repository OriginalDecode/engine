#include "stdafx.h"
#include "RenderNodeGeneral.h"
namespace graphics
{
	u64 RenderNodeGeneral::Type = Hash(STRINGIFY(RenderNodeGeneral));
	RenderNodeGeneral::RenderNodeGeneral()
	{
		auto engine = Engine::GetInstance();
		m_Shaders[VERTEX] = engine->GetAssetsContainer()->GetShader("Data/Shaders/deferred_base.vsmain");
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

		for (const ModelInstance& model_instance : m_Models)
		{
			model_instance.Draw(rc);
		}
	}

	void RenderNodeGeneral::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}


	void RenderNodeGeneral::AddInstance(const ModelInstance instance)
	{
		m_Models.push_back(instance);
	}

};