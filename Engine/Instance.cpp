#include "stdafx.h"
#include "Instance.h"
#include "Effect.h"
#include "Model.h"
#include "Camera.h"
#include "AssetsContainer.h"
#include "Engine.h"


void CInstance::Initiate(const char* filepath, const std::string& effect, const eModelType& aModelType)
{
	switch (aModelType)
	{
		case eModelType::STATIC:
		Engine::GetInstance()->LoadModel<Model>(filepath, effect, true);
		break;
		case eModelType::ANIMATED:
		DL_ASSERT("Not implemented!");
		break;
		default:
		break;
	}
}

void CInstance::Initiate(Model* aModel)
{
	myModel = aModel;
}

void CInstance::Render(const CU::Matrix44f&, Camera&)
{
	//myModel->Render(previousOrientation, aCamera.GetProjection());
}

void CInstance::SetPosition(CU::Math::Vector3<float> aPosition)
{
	myOrientation.SetPosition(aPosition);
	myModel->SetPosition(aPosition);
}
