#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
namespace Snowblind
{
	enum class eDirection
	{
		FORWARD,
		BACK,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};


	class CCamera
	{
	public:
		CCamera(float aWidth, float aHeight, Vector3f aPosition);
		~CCamera();

		void SetPosition(const Vector3f& aPosition);
		const Vector3f GetPosition() const;
		Matrix44f& GetOrientation();
		Matrix44f& GetProjection();

		void Move(eDirection aDirection, float aSpeed);

	private:

		void MoveForwardAndBack(CU::Math::Vector4<float>& aPosition, float aSpeed);
		void MoveUpAndDown(CU::Math::Vector4<float>& aPosition, float aSpeed);
		void MoveLeftAndRight(CU::Math::Vector4<float>& aPosition, float aSpeed);

		Matrix44f myProjectionMatrix;
		Matrix44f myOrientation;
	};

	inline const Vector3f CCamera::GetPosition() const 
	{
		return myOrientation.GetPosition();
	}

	inline Matrix44f& CCamera::GetOrientation()
	{
		return myOrientation;
	}

	inline Matrix44f& CCamera::GetProjection()
	{
		return myProjectionMatrix;
	}
}