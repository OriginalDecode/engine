#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
namespace Snowblind
{

	CSkySphere::CSkySphere(const std::string& aFilePath, const std::string& anEffect)
	{
		myModel = CAssetsContainer::GetInstance()->GetModel(aFilePath, anEffect);
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Render(CCamera* aCamera)
	{
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_NONE);
		myModel->GetEffect()->SetMatrices(myOrientation, aCamera->GetOrientation(), aCamera->GetProjection());
		myModel->Render();
		CEngine::GetDirectX()->SetRasterizer(eRasterizer::CULL_BACK);
	}

	void CSkySphere::SetPosition(const CU::Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}
};