#pragma once
#include "../Misc/Global.h"
#include "CameraMove.h"

namespace CommonUtilities
{
	class Camera2D
	{
	public:
		friend class Megaton;

		void InitProjection(float aHeight, float aWidth, Vector2f aPosition);

		void Move(CameraMovement aMove, float aDelta, float aSpeed);
		void RotateAroundZ(float aRadian, bool aDegree = false);

		const Matrix33f GetInverse();
		const Matrix33f GetProjection();

		const CU::Math::Vector2<int> GetResolution();


	private:
		Camera2D();

		Matrix33f myOrientation;
		Matrix33f myProjection;

		CU::Math::Vector2<int> myScreenResolution;

		float myNear;

	};

}