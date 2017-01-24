#pragma once

#include "../Misc/Global.h"
#include "CameraMove.h"

namespace CommonUtilities
{
	class Camera3D
	{
	public:
		friend class Megaton;

		void InitPerspective(float aFieldOfView, float aHeight, float aWidth, float aFar, float aNear = 0.00001f);

		void Move(CameraMovement aMovement, float aDeltaTime);

		void RotateAroundX(float aRadian, bool aDegree = false);
		void RotateAroundY(float aRadian, bool aDegree = false);
		void RotateAroundZ(float aRadian, bool aDegree = false);

		Matrix44f GetInverse();
		Matrix44f GetPerspective();
		CU::Math::Vector2<int> GetResolution();
	private:
		Camera3D();

		Matrix44f myOrientation;
		Matrix44f myPerspective;

		CU::Math::Vector2<int> myScreenResolution;

		float myNear;

	};

}