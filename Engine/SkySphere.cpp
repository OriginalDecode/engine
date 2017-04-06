#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
#include <JSON/JSONReader.h>
#include "Camera.h"


bool SkySphere::Initiate(const std::string& model_filepath, const std::string& skysphere_shader, Camera* camera)
{
	myCamera = camera;
	if (!myCamera)
		return false;

	Engine::GetInstance()->LoadModel(model_filepath, skysphere_shader, false);

	SkysphereLayer layer;
	layer.m_Model = Engine::GetInstance()->GetModel(model_filepath); 
	layer.m_Model->SetIsSkysphere(true);

	JSONReader reader("Data/Config/game.json");
	layer.m_Texture = Engine::GetInstance()->GetTexture("Data/Textures/Skysphere/T_skybox_level04 - Copy.dds");

	m_Layers.Add(layer);
	if ( m_Layers.Size() <= 0 )
		return false;

	myAPI = Engine::GetAPI();
	if (!myAPI)
		return false;
	m_cbPixelShader = myAPI->CreateConstantBuffer(sizeof(cbPixelShader));
	m_cbVertexShader = myAPI->CreateConstantBuffer(sizeof(cbVertexShader));
	return true;
}

bool SkySphere::AddLayer(const std::string& layer_filepath, const std::string& layer_shader)
{
	return true;
	Engine::GetInstance()->LoadModel(layer_filepath, layer_shader, false);

	SkysphereLayer layer;
	layer.m_Model = Engine::GetInstance()->GetModel(layer_filepath);
	JSONReader reader("Data/Config/game.json");
	layer.m_Texture = Engine::GetInstance()->GetTexture(reader.ReadElement("Skysphere"));
	layer.m_ShouldRotate = true;
	m_Layers.Add(layer);
	if (m_Layers.Size() <= 0)
		return false;

	myAPI = Engine::GetAPI();
	if (!myAPI)
		return false;


	return true;
}

bool SkySphere::CleanUp()
{
	m_Layers.RemoveAll();

	if (m_Layers.Size() > 0)
		return false;

	SAFE_RELEASE(m_cbPixelShader);
	SAFE_RELEASE(m_cbVertexShader);
	return true;
}

void SkySphere::Render(CU::Matrix44f& anOrientation, Texture* aDepthTexture)
{
	SetPosition(anOrientation.GetPosition());
	m_PixelShaderStruct.m_CameraPos = myCamera->GetPosition();
	myAPI->SetBlendState(eBlendStates::LIGHT_BLEND);
	myAPI->SetDepthStencilState(eDepthStencilState::Z_DISABLED, 1);
	myAPI->SetRasterizer(eRasterizer::CULL_NONE);

	myAPI->UpdateConstantBuffer(m_cbPixelShader, &m_PixelShaderStruct);
	myAPI->GetContext()->VSSetConstantBuffers()
	myAPI->GetContext()->PSSetConstantBuffers(0, 1, &m_cbPixelShader);
	for (const SkysphereLayer& layer : m_Layers)
	{
		ID3D11ShaderResourceView* srv[2];
		srv[0] = layer.m_Texture ? layer.m_Texture->GetShaderView() : nullptr;
		srv[1] = aDepthTexture->GetDepthStencilView();
		myAPI->GetContext()->PSSetShaderResources(0, 2, &srv[0]);
		//(#LINUS) Fix scale.
		layer.m_Model->Render(anOrientation, myCamera->GetPerspective(), CU::Vector4f(1, 1, 1, 1));

		srv[0] = nullptr;
		srv[1] = nullptr;
		myAPI->GetContext()->PSSetShaderResources(0, 2, &srv[0]);
	}
}

void SkySphere::SetPosition(const CU::Vector3f& aPosition)
{
	for (const SkysphereLayer& layer : m_Layers)
	{
		layer.m_Model->SetPosition(aPosition);
	}
}

void SkySphere::SetLightPos(const CU::Vector4f& light_position)
{
	m_PixelShaderStruct.m_LightPos = light_position;
}

/*void SkySphere::Update(float dt)
{
	for (const SkysphereLayer& layer : m_Layers)
	{
		if (layer.m_ShouldRotate)
		{
			layer.m_Model->Update(dt);
		}
	}
}*/
