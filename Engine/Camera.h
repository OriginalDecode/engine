#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
namespace Snowblind
{
	class CCamera
	{
	public:
		CCamera(float aWidth, float aHeight, Vector3f aPosition);
		~CCamera();

		void SetPosition(const Vector3f& aPosition);
		const Vector3f GetPosition() const;
		Matrix44f& GetOrientation();
		Matrix44f& GetProjection();


	private:

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