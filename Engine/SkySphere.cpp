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
	{
		//myCamera = aCamera;
		//myModel = CAssetsContainer::GetInstance()->GetModel(aFilePath, anEffect);
		//myModel->SetIsSkysphere();
		//std::wstring skysphereTexture;
		//JSONReader reader("Data/Config/game.json");
		//reader.ForceReadElement("Skysphere", skysphereTexture);
		//mySkysphereTexture = CAssetsContainer::GetInstance()->GetTexture(skysphereTexture);
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Render(CU::Matrix44f& anOrientation)
	{
		CEngine::GetDirectX()->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_NONE);
		CEffect* effect = myModel->GetEffect();

		effect->SetTexture(mySkysphereTexture, "AlbedoTexture");


		effect->SetMatrices(myOrientation, anOrientation, myCamera->GetProjection());

		myModel->Render();
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
		CEngine::GetDirectX()->SetDepthBufferState(eDepthStencil::Z_ENABLED);
	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}
};