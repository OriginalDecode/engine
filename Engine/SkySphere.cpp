#include "stdafx.h"
#include "SkySphere.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
namespace Snowblind
{
	CSkySphere::CSkySphere(const CCamera& aCamera)
		: myCamera(aCamera)
	{
		myModel = CAssetsContainer::GetInstance()->GetModel("", "");
	}

	CSkySphere::~CSkySphere()
	{
	}

	void CSkySphere::Update(float aDeltaTime)
	{

	}

	void CSkySphere::Render()
	{
		
	}

};