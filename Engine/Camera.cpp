#include "stdafx.h"
#include "Camera.h"
#include <SimpleMath.h>
namespace Snowblind
{
	CCamera::CCamera(float aWidth, float aHeight)
	{

		myFOV *= 3.1415926535f / 180.f;
		myProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(0.01f, 1000.f, aHeight / aWidth, myFOV);
	}

	CCamera::CCamera(float aWidth, float aHeight, const CU::Vector3f& aPosition)
	{
		my2DOrientation.myMatrix[3] = aPosition.x;
		my2DOrientation.myMatrix[7] = aPosition.y;
		my2DOrientation.myMatrix[11] = aPosition.z;

		myOrthogonalMatrix = CU::Matrix44f::CreateOrthogonalMatrixLH(aWidth, aHeight, 0.01f, 100.f);

	}

	CU::Matrix44f& CCamera::GetOrthogonalMatrix()
	{
		return myOrthogonalMatrix;
	}

	void CCamera::SetPosition(const CU::Vector3f& position)
	{
		myOrientation.SetPosition(position);
	}

	void CCamera::SetTranslation(const CU::Vector4f& translation)
	{
		myOrientation.SetTranslation(translation);
	}

	//void CCamera::AddOrientation(CU::Matrix44f* anOrientation)
	//{
	//	myOrientation = anOrientation;
	//}

	/*void CCamera::Move(eDirection aDirection, float aSpeed)
	{
		CU::Math::Vector4<float> position;
		position = myOrientation->GetTranslation();
		switch (aDirection)
		{
		case eDirection::FORWARD:
			MoveForwardAndBack(position, aSpeed);
			break;
		case eDirection::BACK:
			MoveForwardAndBack(position, aSpeed);
			break;
		case eDirection::UP:
			MoveUpAndDown(position, aSpeed);
			break;
		case eDirection::DOWN:
			MoveUpAndDown(position, aSpeed);
			break;
		case eDirection::LEFT:
			MoveLeftAndRight(position, aSpeed);
			break;
		case eDirection::RIGHT:
			MoveLeftAndRight(position, aSpeed);
			break;
		}
		myOrientation->SetTranslation(position);
	}



	void CCamera::MoveForwardAndBack(CU::Vector4f& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> forward = myOrientation->GetForward();
		aPosition += forward * aSpeed;
	}

	void CCamera::MoveUpAndDown(CU::Vector4f& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> up = myOrientation->GetUp();
		aPosition += up * aSpeed;
	}

	void CCamera::MoveLeftAndRight(CU::Vector4f& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> right = myOrientation->GetRight();
		aPosition += right * aSpeed;
	}*/

}