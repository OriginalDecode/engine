#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
#include <JSON/JSONReader.h>
#include "Camera.h"

namespace Snowblind
{
	bool CSkySphere::Initiate(const std::string& model_filepath, const std::string& skysphere_shader, CCamera* camera)
	{
		myCamera = camera;
		if (!myCamera)
			return false;

		CEngine::GetInstance()->LoadModel(model_filepath, skysphere_shader);

		SkysphereLayer layer;
		layer.m_Model = CEngine::GetInstance()->GetModel(model_filepath);
		layer.m_Texture = nullptr;

		m_Layers.Add(layer);
		if (m_Layers.Size() <= 0)
			return false;

		myAPI = CEngine::GetAPI();
		if (!myAPI)
			return false;



		return true;
	}

	bool CSkySphere::AddLayer(const std::string& layer_filepath, const std::string& layer_shader)
	{
		CEngine::GetInstance()->LoadModel(layer_filepath, layer_shader);

		SkysphereLayer layer;
		layer.m_Model = CEngine::GetInstance()->GetModel(layer_filepath);
		JSONReader reader("Data/Config/game.json");
		layer.m_Texture = CEngine::GetInstance()->GetTexture(reader.ReadElement("Skysphere"));
		layer.m_ShouldRotate = true;
		m_Layers.Add(layer);
		if (m_Layers.Size() <= 0)
			return false;

		myAPI = CEngine::GetAPI();
		if (!myAPI)
			return false;

		return true;
	}

	bool CSkySphere::CleanUp()
	{
		m_Layers.RemoveAll();
		
		if (m_Layers.Size() > 0)
			return false;

		return true;
	}

	void CSkySphere::Render(CU::Matrix44f& anOrientation, Texture* aDepthTexture)
	{
		SetPosition(anOrientation.GetPosition());

		myAPI->SetBlendState(eBlendStates::LIGHT_BLEND);
		myAPI->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		myAPI->SetRasterizer(eRasterizer::CULL_NONE);

		for (const SkysphereLayer& layer : m_Layers )
		{
			ID3D11ShaderResourceView* srv[2];
			srv[0] = layer.m_Texture ? layer.m_Texture->GetShaderView() : nullptr;
			srv[1] = aDepthTexture->GetDepthStencilView();
			myAPI->GetContext()->PSSetShaderResources(0, 2, &srv[0]);

			layer.m_Model->Render(anOrientation, myCamera->GetProjection());

			srv[0] = nullptr;
			srv[1] = nullptr;
			myAPI->GetContext()->PSSetShaderResources(0, 2, &srv[0]);
		}
	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		for (const SkysphereLayer& layer : m_Layers)
		{
			layer.m_Model->SetPosition(aPosition);
		}
	}

	void CSkySphere::Update(float dt)
	{
		for (const SkysphereLayer& layer : m_Layers)
		{
			if (layer.m_ShouldRotate)
			{
				layer.m_Model->Update(dt);
			}
		}
	}
};