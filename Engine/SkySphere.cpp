#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
#include <JSON/JSONReader.h>
namespace Snowblind
{

	CSkySphere::CSkySphere(const std::string& aFilePath, const std::string& anEffect)
	{
		myModel = CAssetsContainer::GetInstance()->GetModel(aFilePath, anEffect);
		myModel->SetIsSkysphere();
		std::string skysphereTexture;
		JSONReader reader("Data/Config/game.json");
		reader.ForceReadElement("Skysphere", skysphereTexture);
		mySkysphereTexture = CAssetsContainer::GetInstance()->GetTexture(skysphereTexture);
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Render(CCamera* aCamera)
	{
		CEngine::GetDirectX()->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_NONE);
		CEffect* effect = myModel->GetEffect();

		effect->SetTexture(mySkysphereTexture, "AlbedoTexture");


		effect->SetMatrices(myOrientation, myPrevOrientation, aCamera->GetProjection());
		myPrevOrientation = aCamera->GetOrientation();


		myModel->Render();
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
		CEngine::GetDirectX()->SetDepthBufferState(eDepthStencil::Z_ENABLED);

	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}
};