#include "stdafx.h"
#include "Instance.h"
#include "Effect.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
namespace Snowblind
{
	CInstance::CInstance()
	{
	}

	CInstance::~CInstance()
	{
	}

	void CInstance::Initiate(const char* aFilePath, const std::string& effect, const eModelType& aModelType)
	{
		switch (aModelType)
		{
		case eModelType::STATIC:
			Snowblind::CAssetsContainer::GetInstance()->LoadModel(aFilePath, effect);
			myModel = Snowblind::CAssetsContainer::GetInstance()->GetModel(aFilePath);
			break;
		case eModelType::ANIMATED:
			DL_ASSERT("Not implemented!");
			break;
		default:
			break;
		}
	}

	void CInstance::Initiate(CModel* aModel)
	{
		myModel = aModel;
	}

	void CInstance::Render(CU::Matrix44f& previousOrientation, CCamera& aCamera)
	{
		myModel->Render(previousOrientation, aCamera.GetProjection());
	}

	void CInstance::SetPosition(CU::Math::Vector3<float> aPosition)
	{
		myOrientation.SetPosition(aPosition);
		myModel->SetPosition(aPosition);
	}

	void CInstance::SetIsLightmesh()
	{
		myModel->SetIsLightmesh();
	}

};