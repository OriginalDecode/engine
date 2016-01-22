#include "Instance.h"
#include "Effect.h"
#include "Model.h"
#include "Camera.h"
#include <Math/Matrix/Matrix.h>
namespace Snowblind
{
	CInstance::CInstance(CModel* aModel) 
		: myModel(*aModel)
	{
	}


	CInstance::~CInstance()
	{
	}

	void CInstance::Update(float aRotation)
	{
		myOrientation *= CU::Math::Matrix44<float>::CreateRotateAroundY(aRotation);
	}

	void CInstance::Render(CCamera& aCamera)
	{
		myModel.GetEffect()->SetMatrices(myOrientation, aCamera.GetOrientation(), aCamera.GetProjection());
		myModel.Render();
	}

};