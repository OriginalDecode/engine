#include "stdafx.h"
#include "Camera.h"
#include <SimpleMath.h>
#include "../Input/ControllerInput.h"
namespace Hex
{
	Camera::Camera(float aWidth, float aHeight)
	{

		myFOV *= 3.1415926535f / 180.f;
		myProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(0.01f, 1000.f, aHeight / aWidth, myFOV);
	}

	Camera::Camera(float aWidth, float aHeight, const CU::Vector3f& aPosition)
	{
		my2DOrientation.myMatrix[3] = aPosition.x;
		my2DOrientation.myMatrix[7] = aPosition.y;
		my2DOrientation.myMatrix[11] = aPosition.z;

		myOrthogonalMatrix = CU::Matrix44f::CreateOrthogonalMatrixLH(aWidth, aHeight, 0.01f, 100.f);

	}

	Camera::Camera(float aWidth, float aHeight, float far_plane, float near_plane)
	{
		myFOV *= 3.1415926535f / 180.f;
		myProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(near_plane, far_plane, aHeight / aWidth, myFOV);
	}

	Camera::Camera(float aWidth, float aHeight, float far_plane, float near_plane, float fov)
	{
		myFOV = fov * (3.1415926535f / 180.f);
		myProjectionMatrix = CU::Matrix44f::CreateProjectionMatrixLH(near_plane, far_plane, aHeight / aWidth, myFOV);
	}

	CU::Matrix44f& Camera::GetOrthogonalMatrix()
	{
		return myOrthogonalMatrix;
	}

	void Camera::SetPosition(const CU::Vector3f& position)
	{
		myOrientation.SetPosition(position);
	}

	void Camera::SetTranslation(const CU::Vector4f& translation)
	{
		myOrientation.SetTranslation(translation);
	}

	void Camera::Update(const ControllerState& controller_state)
	{

		float x_value = (float)controller_state.m_ThumbRX;
		float y_value = (float)controller_state.m_ThumbRY;

		float magnitude = sqrt(x_value * x_value + y_value * y_value); //Do something to skip the sqrt?
		float normalized = 0.f;
		const float r_thumb_deadzone = 8689.f;

		if (magnitude > r_thumb_deadzone)
		{
			if (magnitude > SHRT_MAX)
				magnitude = SHRT_MAX;

			magnitude -= r_thumb_deadzone;

			normalized = magnitude / (SHRT_MAX - r_thumb_deadzone);

		}
		else
		{
			x_value = 0.f;
			y_value = 0.f;
		}


		if (normalized < -0.5f || normalized > 0.5f)
		{
			x_value /= 2.f;
			y_value /= 2.f;
		}

		m_CenterPoint.x += (x_value / SHRT_MAX) * m_LookSpeedModifier;
		m_CenterPoint.y -= (y_value / SHRT_MAX) * m_LookSpeedModifier;
		m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

		myPitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), m_CenterPoint.y);
		myYaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), m_CenterPoint.x);

		CU::Vector3f axisX(1.f, 0, 0);
		CU::Vector3f axisY(0, 1.f, 0);
		CU::Vector3f axisZ(0, 0, 1.f);

		axisX = myYaw * myPitch * axisX;
		axisY = myYaw * myPitch * axisY;
		axisZ = myYaw * myPitch * axisZ;

		myOrientation.myMatrix[0] = axisX.x;
		myOrientation.myMatrix[1] = axisX.y;
		myOrientation.myMatrix[2] = axisX.z;
		myOrientation.myMatrix[4] = axisY.x;
		myOrientation.myMatrix[5] = axisY.y;
		myOrientation.myMatrix[6] = axisY.z;
		myOrientation.myMatrix[8] = axisZ.x;
		myOrientation.myMatrix[9] = axisZ.y;
		myOrientation.myMatrix[10] = axisZ.z;
	}

	void Camera::Update(const CU::Vector2f& cursor_pos)
	{
		float x_value = cursor_pos.x;
		float y_value = cursor_pos.y;

		m_CenterPoint.x += x_value * m_LookSpeedModifier;
		m_CenterPoint.y += y_value * m_LookSpeedModifier;
		m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

		myPitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), m_CenterPoint.y);
		myYaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), m_CenterPoint.x);

		CU::Vector3f axisX(1.f, 0, 0);
		CU::Vector3f axisY(0, 1.f, 0);
		CU::Vector3f axisZ(0, 0, 1.f);

		axisX = myYaw * myPitch * axisX;
		axisY = myYaw * myPitch * axisY;
		axisZ = myYaw * myPitch * axisZ;

		myOrientation.myMatrix[0] = axisX.x;
		myOrientation.myMatrix[1] = axisX.y;
		myOrientation.myMatrix[2] = axisX.z;

		myOrientation.myMatrix[4] = axisY.x;
		myOrientation.myMatrix[5] = axisY.y;
		myOrientation.myMatrix[6] = axisY.z;

		myOrientation.myMatrix[8] = axisZ.x;
		myOrientation.myMatrix[9] = axisZ.y;
		myOrientation.myMatrix[10] = axisZ.z;
	}

	void Camera::SetOrientation(const CU::Matrix44f& matrix)
	{
		myOrientation = matrix;
	}

	void Camera::RotateAroundX(float rad)
	{
		myOrientation = CU::Matrix44f::CreateRotateAroundX(rad) * myOrientation;
	}

	void Camera::RotateAroundY(float rad)
	{
		myOrientation = CU::Matrix44f::CreateRotateAroundY(rad) * myOrientation;
	}

	void Camera::RotateAroundZ(float rad)
	{
		myOrientation = CU::Matrix44f::CreateRotateAroundZ(rad) * myOrientation;
	}

	void Camera::Move(eDirection aDirection, float aSpeed)
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

	void Camera::MoveForwardAndBack(CU::Vector4f& aPosition, float aSpeed)
	{
	CU::Math::Vector4<float> forward = myOrientation.GetForward();
	aPosition += forward * aSpeed;
	}

	void Camera::MoveUpAndDown(CU::Vector4f& aPosition, float aSpeed)
	{
	CU::Math::Vector4<float> up = myOrientation.GetUp();
	aPosition += up * aSpeed;
	}

	void Camera::MoveLeftAndRight(CU::Vector4f& aPosition, float aSpeed)
	{
	CU::Math::Vector4<float> right = myOrientation.GetRight();
	aPosition += right * aSpeed;
	}


}