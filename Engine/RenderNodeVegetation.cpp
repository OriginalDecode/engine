#include "stdafx.h"
#include "RenderNodeVegetation.h"
#include <engine/Engine.h>
#include <engine/AssetsContainer.h>
#include <engine/Model.h>

namespace graphics
{
	RenderNodeVegetation::RenderNodeVegetation()
	{
		auto engine = Engine::GetInstance();

		engine->LoadModelA("Data/model/sponza_pbr/building.fbx", "Shaders/deferred_base.json", false);
		m_Model = Engine::GetInstance()->GetModel<Model>("Data/model/sponza_pbr/building.fbx").GetData();

		m_Shaders[VERTEX] = engine->GetAssetsContainer()->GetShader("Data/Shaders/deferred_base.vsmain");
		m_Shaders[PIXEL] = engine->GetAssetsContainer()->GetShader("Data/Shaders/pbl_debug.psmain");

		m_Shaders[VERTEX]->RegisterReload(this);
		m_Shaders[PIXEL]->RegisterReload(this);

	}


	RenderNodeVegetation::~RenderNodeVegetation()
	{
	}

	void RenderNodeVegetation::Draw(const RenderContext& rc)
	{
		rc.GetContext().SetVertexShader(m_Shaders[VERTEX]);
		rc.GetContext().SetPixelShader(m_Shaders[PIXEL]);


		m_Model->Render(rc);

		//render the models

	}

	
	void RenderNodeVegetation::Reload(CompiledShader* shader)
	{
		m_Shaders[shader->m_Type] = shader;
	}

};