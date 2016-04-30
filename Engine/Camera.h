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

	enum class eRotation
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS
	};


	class CCamera
	{
	public:
		CCamera(float aWidth, float aHeight, const CU::Vector3f& aPosition);
		CCamera(float aWidth, float aHeight);
		~CCamera();

		void SetPosition(const CU::Vector3f& aPosition);
		const CU::Vector3f GetPosition() const;
		CU::Matrix44f& GetOrientation();
		CU::Matrix44f& GetProjection();

		CU::Matrix44f& GetOrthogonalMatrix();


		void Move(eDirection aDirection, float aSpeed);
		void Rotate(eRotation anAxis, float aSpeed);
	private:

		void MoveForwardAndBack(CU::Math::Vector4<float>& aPosition, float aSpeed);
		void MoveUpAndDown(CU::Math::Vector4<float>& aPosition, float aSpeed);
		void MoveLeftAndRight(CU::Math::Vector4<float>& aPosition, float aSpeed);

		CU::Matrix44f myProjectionMatrix;
		CU::Matrix44f myOrientation;
		CU::Matrix44f myOrthogonalMatrix;
	};

	inline const CU::Vector3f CCamera::GetPosition() const
	{
		return myOrientation.GetPosition();
	}

	inline CU::Matrix44f& CCamera::GetOrientation()
	{
		return myOrientation;
	}

	inline CU::Matrix44f& CCamera::GetProjection()
	{
		return myProjectionMatrix;
	}
}