#include "stdafx.h"
#include "Instance.h"
#include "Effect.h"
#include "Model.h"
#include "Camera.h"
namespace Snowblind
{
	CInstance::CInstance() 
	{
	}


	CInstance::~CInstance()
	{
	}

	void CInstance::Initiate(CModel* aModel)
	{
		myModel = aModel;
	}

	void CInstance::Update(float aRotation)
	{
		myOrientation = CU::Math::Matrix44<float>::CreateRotateAroundY(aRotation) * myOrientation;
	}

	void CInstance::Render(CCamera& aCamera)
	{
		myModel->GetEffect()->SetMatrices(myOrientation, aCamera.GetOrientation(), aCamera.GetProjection());
		myModel->Render();
	}

	void CInstance::SetPosition(CU::Math::Vector3<float> aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}




};