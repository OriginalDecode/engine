#include "Quaternion.h"
#include <cmath>
namespace CommonUtilities
{

	Quaternion::Quaternion()
		: v(0, 0, 0)
		, w(1.f)
	{
	}

	Quaternion::Quaternion(const CU::Vector3f& aNormal, float anAngle)
		: v(aNormal * sinf(anAngle * 0.5f))
		, w(cosf(anAngle * 0.5f))
	{
	}

	Quaternion Quaternion::Inverted() const
	{
		Quaternion q;

		q.w = w;
		q.v.x = -v.x;
		q.v.y = -v.y;
		q.v.z = -v.z;

		return q;
	}

	Quaternion Quaternion::operator*(const Quaternion& aQuaternion) const
	{
		Quaternion r;
		r.w = w * aQuaternion.w - Dot(v, aQuaternion.v);
		r.v = v * aQuaternion.w + aQuaternion.v * w + Cross(v, aQuaternion.v);

		return r;
	}

	CU::Vector3f Quaternion::operator*(const CU::Vector3f& aVector) const
	{
		CU::Vector3f vcV = Cross(v, aVector);
		return aVector + vcV * (2.f * w) + Cross(v, vcV) * 2.f;
	}

	Quaternion Quaternion::operator^(float aT) const
	{
		float a;
		CU::Vector3f n;

		ToAxisAngle(n, a);

		float aTimesT = a * aT;

		return Quaternion(n, aTimesT);
	}

	Quaternion Quaternion::Slerp(const Quaternion& other, float aT) const
	{
		return ((other * Inverted()) ^ aT) * (*this);
	}

	void Quaternion::ToAxisAngle(CU::Vector3f& aVectorAxisOut, float& anAngleOut) const
	{
		if (Length2(v) < 0.0001f)
		{
			aVectorAxisOut = CU::Vector3f(1.f, 0, 0);
		}
		else
		{
			aVectorAxisOut = GetNormalized(v);
		}

		anAngleOut = acos(w) * 2.f;
	}
}
namespace CU = CommonUtilities;