#include "Camera3D.h"

namespace CommonUtilities
{
	Camera3D::Camera3D()
	{

		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		myScreenResolution.x = float(desktop.right);
		myScreenResolution.y = float(desktop.bottom);

	}

	void Camera3D::InitPerspective(float aFoV, float aHeight, float aWidth, float aFar, float aNear)
	{

		myPerspective = Matrix44f();

		myNear = aNear;
		float height = cos(0.5f*aFoV) / sin(0.5f*aFoV);
		float aspect = aWidth / aHeight;
		float scale = aFar / (aFar - aNear);

		myPerspective.myMatrix[0] = height / aspect;
		myPerspective.myMatrix[5] = height;
		myPerspective.myMatrix[10] = scale;
		myPerspective.myMatrix[11] = 1;
		myPerspective.myMatrix[14] = -scale / aNear;

	}

	void Camera3D::Move(CameraMovement aMovement, float aDeltaTime)
	{
		Vector4f position;
		position = myOrientation.GetTranslation();

		Vector4f forwardVector = myOrientation.GetForward();
		Vector4f rightVector = myOrientation.GetRight();
		Vector4f upVector = myOrientation.GetUp();

		float speed = 25;


		switch (aMovement)
		{
		case CameraMovement::Forward:
			position += (forwardVector * (speed * aDeltaTime));
			break;
		case CameraMovement::Backward:
			position -= (forwardVector * (speed * aDeltaTime));
			break;
		case CameraMovement::Right:
			position += (rightVector * (speed * aDeltaTime));
			break;
		case CameraMovement::Left:
			position -= (rightVector * (speed * aDeltaTime));
			break;
		case CameraMovement::Up:
			position += (upVector *(speed*aDeltaTime));
			break;
		case CameraMovement::Down:
			position -= (upVector *(speed*aDeltaTime));
			break;


		}

		myOrientation.SetTranslation(position.x, position.y, position.z, 1);
	}



	void Camera3D::RotateAroundX(float aRadian, bool aDegree)
	{
		if (aDegree == false)
			myOrientation.Rotate3DX(aRadian);
		else
			myOrientation.Rotate3DX(DEGREE_TO_RADIAN(aRadian));
	}

	void Camera3D::RotateAroundY(float aRadian, bool aDegree)
	{
		if (aDegree == false)
			myOrientation.Rotate3DY(aRadian);
		else
			myOrientation.Rotate3DY(DEGREE_TO_RADIAN(aRadian));
	}

	void Camera3D::RotateAroundZ(float aRadian, bool aDegree)
	{
		if (aDegree == false)
			myOrientation.Rotate3DZ(aRadian);
		else
			myOrientation.Rotate3DZ(DEGREE_TO_RADIAN(aRadian));
	}

	CU::Math::Vector2<int> Camera3D::GetResolution()
	{
		return myScreenResolution;
	}

	Matrix44f Camera3D::GetInverse()
	{
		return CU::Math::Inverse(myOrientation);
	}

	Matrix44f Camera3D::GetPerspective()
	{
		return myPerspective;
	}
}