#pragma once
#include "../Vector/Vector.h"

namespace CommonUtilities
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const CU::Vector3f& aNormal, float anAngle);

		Quaternion Inverted() const;

		Quaternion operator*(const Quaternion& aQuaternion) const;
		
		CU::Vector3f operator*(const CU::Vector3f& aVector) const;
		Quaternion operator^(float aT) const;

		Quaternion Slerp(const Quaternion& other, float aT) const;


	private:
		void ToAxisAngle(CU::Vector3f& aVectorAxisOut, float& anAngleOut) const;
		CU::Vector3f v;
		float w;
	};

}