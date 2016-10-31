#include "stdafx.h"
#include "Camera.h"
#include <SimpleMath.h>
#include "../Input/ControllerInput.h"
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

	/*void CCamera::Update(const ControllerState& controller_state)
	{
		m_CenterPoint.y -= ((float)controller_state.m_ThumbRY / SHRT_MAX) * 0.01f;
		m_CenterPoint.x += ((float)controller_state.m_ThumbRX / SHRT_MAX) * 0.01f;

		m_CenterPoint.y = fmaxf(fminf(1.57f, m_CenterPoint.y), -1.57f);

		myPitch = CU::Quaternion(CU::Vector3f(1.f, 0, 0), m_CenterPoint.y);
		myYaw = CU::Quaternion(CU::Vector3f(0, 1.f, 0), m_CenterPoint.x);

		CU::Vector3f axisX(1.f, 0, 0);
		CU::Vector3f axisY(0, 1.f, 0);
		CU::Vector3f axisZ(0, 0, 1.f);

		axisX = myYaw * myPitch * axisX;
		axisY = myYaw * myPitch * axisY;
		axisZ = myYaw * myPitch * axisZ;

		myOrientation[0] = axisX.x;
		myOrientation[1] = axisX.y;
		myOrientation[2] = axisX.z;
		myOrientation[4] = axisY.x;
		myOrientation[5] = axisY.y;
		myOrientation[6] = axisY.z;
		myOrientation[8] = axisZ.x;
		myOrientation[9] = axisZ.y;
		myOrientation[10] = axisZ.z;
	}*/

	void CCamera::SetOrientation(const CU::Matrix44f& matrix)
	{
		myOrientation = matrix;
	}

}