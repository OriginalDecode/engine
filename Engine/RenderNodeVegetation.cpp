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

		m_VertexShader = engine->GetAssetsContainer()->GetShader("deferred_base.vs");
		m_PixelShader = engine->GetAssetsContainer()->GetShader("pbl_debug.ps");

		engine->LoadModelA("Data/model/sponza_pbr/building.fbx", "Shaders/deferred_base.json", false);
		m_Model = Engine::GetInstance()->GetModel<Model>("Data/model/sponza_pbr/building.fbx").GetData();
	}


	RenderNodeVegetation::~RenderNodeVegetation()
	{
	}

	void RenderNodeVegetation::Draw(const RenderContext& rc)
	{
		rc.GetContext().SetVertexShader(m_VertexShader);
		rc.GetContext().SetPixelShader(m_PixelShader);


		m_Model->Render(rc);

		//render the models


	}

};