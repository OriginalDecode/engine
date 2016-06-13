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
			myModel = Snowblind::CAssetsContainer::GetInstance()->GetModel(aFilePath, effect);
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

	void CInstance::Update(float aDeltaTime)
	{
		//myOrientation = CU::Math::Matrix44<float>::CreateRotateAroundY(aRotation) * myOrientation;
	}

	void CInstance::Render(CU::Matrix44f& previousOrientation, CCamera& aCamera)
	{
		//myModel->GetEffect()->SetMatrices(myOrientation, previousOrientation, aCamera.GetProjection());
		//myModel->Render();
	}

	void CInstance::Render()
	{
		//myModel->GetEffect()->SetWorldMatrix(myOrientation);
		//myModel->Render();
	}

	void CInstance::SetPosition(CU::Math::Vector3<float> aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}

	void CInstance::UpdateLight(CU::StaticArray<SDirectionallightData, DIRECTIONAL_SIZE>& someData)
	{
		//myModel->GetEffect()->UpdateLight(someData);
	}

	void CInstance::UpdateLight(CU::StaticArray<SSpotlightData, SPOTLIGHT_SIZE>& someData)
	{
		//myModel->GetEffect()->UpdateLight(someData);
	}

	void CInstance::UpdateLight(CU::StaticArray<SPointlightData, POINTLIGHT_SIZE>& someData)
	{
		//myModel->GetEffect()->UpdateLight(someData);
	}

};