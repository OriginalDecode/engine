#include "stdafx.h"
#include "Camera.h"
#include <Windows.h>
#include <xnamath.h>
namespace Snowblind
{
	CCamera::CCamera(float aWidth, float aHeight, const Vector3f& aPosition)
	{
		XMMATRIX projection;
		projection = XMMatrixPerspectiveFovLH(XM_PI*0.3f, aWidth / FLOAT(aHeight), 0.1f, 100.f);

		XMFLOAT4X4 projMiddleHand;
		XMStoreFloat4x4(&projMiddleHand, projection);
		myProjectionMatrix.Init(reinterpret_cast<float*>(projMiddleHand.m));

		myOrientation.myMatrix[3] = aPosition.x;
		myOrientation.myMatrix[7] = aPosition.y;
		myOrientation.myMatrix[11] = aPosition.z;


		XMMATRIX orthogonal;
		orthogonal = XMMatrixOrthographicLH(aWidth, aHeight, 0.f, 1.f);
		XMFLOAT4X4 orthMiddleHand;
		XMStoreFloat4x4(&orthMiddleHand, orthogonal);
		myOrthogonalMatrix.Init(reinterpret_cast<float*>(orthMiddleHand.m));

	}

	CCamera::~CCamera()
	{
	}

	void CCamera::SetPosition(const Vector3f& aPosition)
	{
		myOrientation.SetPosition(aPosition);
	}

	Matrix44f& CCamera::GetOrthogonalMatrix()
	{
		return myOrthogonalMatrix;
	}

	void CCamera::Move(eDirection aDirection, float aSpeed)
	{
		CU::Math::Vector4<float> position;
		position = myOrientation.GetTranslation();
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
		myOrientation.SetTranslation(position);
	}

	void CCamera::Rotate(eRotation anAxis, float aSpeed)
	{
		switch (anAxis)
		{
		case eRotation::X_AXIS:
			myOrientation = CU::Math::Matrix44<float>::CreateRotateAroundX(aSpeed) * myOrientation;
			break;
		case eRotation::Y_AXIS:
			myOrientation = CU::Math::Matrix44<float>::CreateRotateAroundY(aSpeed) * myOrientation;
			break;
		case eRotation::Z_AXIS:
			myOrientation = CU::Math::Matrix44<float>::CreateRotateAroundZ(aSpeed) * myOrientation;
			break;
		}
	}

	void CCamera::MoveForwardAndBack(CU::Math::Vector4<float>& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> forward = myOrientation.GetForward();
		aPosition += forward * aSpeed;
	}

	void CCamera::MoveUpAndDown(CU::Math::Vector4<float>& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> up = myOrientation.GetUp();
		aPosition += up * aSpeed;
	}

	void CCamera::MoveLeftAndRight(CU::Math::Vector4<float>& aPosition, float aSpeed)
	{
		CU::Math::Vector4<float> right = myOrientation.GetRight();
		aPosition += right * aSpeed;
	}

}