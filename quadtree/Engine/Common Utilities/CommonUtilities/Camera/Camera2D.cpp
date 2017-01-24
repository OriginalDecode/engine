#include "Camera2D.h"

namespace CommonUtilities
{
	Camera2D::Camera2D()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		myScreenResolution.x = float(desktop.right);
		myScreenResolution.y = float(desktop.bottom);
	}


	void Camera2D::InitProjection(float aHeight, float aWidth, Vector2f aPosition)
	{

		myProjection.myMatrix[0] = 2.f / aWidth;
		myProjection.myMatrix[4] = 2.f / aHeight;
		myOrientation.SetTranslation(aPosition.x, aPosition.y, 1);

	}

	const CU::Math::Vector2<int> Camera2D::GetResolution()
	{
		return myScreenResolution;
	}

	void Camera2D::Move(CameraMovement aMovement, float aDelta, float aSpeed)
	{
		Vector3f position;
		position = myOrientation.GetTranslation();

		switch (aMovement)
		{
		case CameraMovement::Up:
			position.y -= aSpeed*aDelta;
			break;

		case CameraMovement::Down:
			position.y += aSpeed*aDelta;
			break;

		case CameraMovement::Right:
			position.x += aSpeed*aDelta;
			break;

		case CameraMovement::Left:
			position.x -= aSpeed*aDelta;
			break;

		}

		myOrientation.SetTranslation(position.x, position.y, 1);

	}

	void Camera2D::RotateAroundZ(float aRadian, bool aDegree)
	{
		if (aDegree == false)
			myOrientation.CreateRotateAroundZ(aRadian);
		else
			myOrientation.CreateRotateAroundZ(DEGREE_TO_RADIAN(aRadian));

	}
}