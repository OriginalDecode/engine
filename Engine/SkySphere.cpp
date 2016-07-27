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
		myModel->SetIsSkysphere();
		std::string skysphereTexture;
		JSONReader reader("Data/Config/game.json");
		reader.ForceReadElement("Skysphere", skysphereTexture);
		mySkysphereTexture = CEngine::GetInstance()->GetTexture(skysphereTexture);
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Render(CU::Matrix44f& anOrientation)
	{


		ID3D11ShaderResourceView* srv = mySkysphereTexture->GetShaderView();
		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, 1, &srv);

		myModel->Render(anOrientation, myCamera->GetProjection());

		srv = nullptr;
		CEngine::GetDirectX()->GetContext()->PSSetShaderResources(0, 1, &srv);


	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		myModel->SetPosition(aPosition);
	}
};