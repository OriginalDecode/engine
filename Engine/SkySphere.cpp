#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
#include <JSON/JSONReader.h>
#include "Camera.h"

namespace Snowblind
{

	CSkySphere::CSkySphere(const std::string& aFilePath, const std::string& anEffect, CCamera* aCamera)
		: myCamera(aCamera)
	{
		CEngine::GetInstance()->LoadModel(aFilePath, anEffect);
		myModel = CEngine::GetInstance()->GetModel(aFilePath);
//		myModel->SetIsSkysphere();
		std::string skysphereTexture;
		JSONReader reader("Data/Config/game.json");
		reader.ForceReadElement("Skysphere", skysphereTexture);
		mySkysphereTexture = CEngine::GetInstance()->GetTexture(skysphereTexture);
		myDirectX = CEngine::GetDirectX();
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Render(CU::Matrix44f& anOrientation, CTexture* aDepthTexture)
	{
		myDirectX->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		myDirectX->SetRasterizer(eRasterizer::CULL_NONE);


		ID3D11ShaderResourceView* srv[2];
		srv[0] = mySkysphereTexture->GetShaderView();
		srv[1] = aDepthTexture->GetDepthStencilView();

		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, 2, &srv[0]);

		myModel->Render(anOrientation, myCamera->GetProjection());

		srv[0] = nullptr;
		srv[1] = nullptr;
		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, 2, &srv[0]);

	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		myModel->SetPosition(aPosition);
	}
};