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
		CCamera(float aWidth, float aHeight);
		CCamera(float aWidth, float aHeight, const CU::Vector3f& aPosition);
		~CCamera();

		const CU::Vector3f GetPosition() const;
		CU::Matrix44f& GetOrientation();
		CU::Matrix44f& Get2DOrientation();

		CU::Matrix44f& GetProjection();

		CU::Matrix44f& GetOrthogonalMatrix();
		void AddOrientation(CU::Matrix44f* anOrientation);

		void Move(eDirection aDirection, float aSpeed);
	private:
		void operator=(CCamera&) = delete;
		void MoveForwardAndBack(CU::Vector4f& aPosition, float aSpeed);
		void MoveUpAndDown(CU::Vector4f& aPosition, float aSpeed);
		void MoveLeftAndRight(CU::Vector4f& aPosition, float aSpeed);

		CU::Matrix44f myOrientation;
		CU::Matrix44f my2DOrientation;

		CU::Matrix44f myProjectionMatrix;
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

	inline CU::Matrix44f& CCamera::Get2DOrientation()
	{
		return my2DOrientation;
	}

	inline CU::Matrix44f& CCamera::GetProjection()
	{
		return myProjectionMatrix;
	}
}