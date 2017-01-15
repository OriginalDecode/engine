#pragma once
#include "../Vector/Vector.h"
namespace CommonUtilities
{
	template<typename TYPE>
	class Plane
	{
	public:
		Plane() = default;
		Plane(Math::Vector3<TYPE>aFirstPoint, Math::Vector3<TYPE> aSecondPoint, Math::Vector3<TYPE> aThirdPoint);
		Plane(Math::Vector3<TYPE> aPoint, Math::Vector3<TYPE> aNormal);


		void InitWith3Points(Math::Vector3<TYPE> aFirstPoint, Math::Vector3<TYPE> aSecondPoint, Math::Vector3<TYPE> aThirdPoint);
		void InitWithPointAndNormal(Math::Vector3<TYPE> aPoint, Math::Vector3<TYPE> aNormal);

		bool Inside(Math::Vector3<TYPE> aPosition);

		Math::Vector3<TYPE> GetNormal() const;
		Math::Vector3<TYPE> GetPoint() const;

	private:

		Math::Vector3<TYPE> myNorm;
		Math::Vector3<TYPE> myPoint;
	};

	template<typename TYPE>
	Plane<TYPE>::Plane(Math::Vector3<TYPE> aFirstPoint, Math::Vector3<TYPE> aSecondPoint, Math::Vector3<TYPE> aThirdPoint)
	{
		myPoint = aFirstPoint;
		myNorm = Cross(aSecondPoint - aFirstPoint, aThirdPoint - aFirstPoint);

		Normalize(myNorm);
	}

	template<typename TYPE>
	Plane<TYPE>::Plane(Math::Vector3<TYPE> aPoint, Math::Vector3<TYPE> aNormal)
	{
		InitWithPointAndNormal(aPoint, aNormal);
	}

	template<typename TYPE>
	void Plane<TYPE>::InitWith3Points(Math::Vector3<TYPE> aFirstPoint, Math::Vector3<TYPE> aSecondPoint, Math::Vector3<TYPE> aThirdPoint)
	{
		CommonUtilities::Plane(aFirstPoint, aSecondPoint, aThirdPoint);
	}

	template<typename TYPE>
	void Plane<TYPE>::InitWithPointAndNormal(Math::Vector3<TYPE> aPoint, Math::Vector3<TYPE> aNormal)
	{
		myPoint = aPoint;
		myNorm = aNormal;
		Normalize(myNorm);
	}

	template<typename TYPE>
	bool Plane<TYPE>::Inside(Math::Vector3<TYPE> aPosition)
	{
		if (Math::Dot<float>(aPosition - myPoint, myNorm) > 0.f)
			return false;
		else
			return true;
	}

	template<typename TYPE>
	Math::Vector3<TYPE> Plane<TYPE>::GetPoint() const
	{
		return myPoint;
	}

	template<typename TYPE>
	Math::Vector3<TYPE> Plane<TYPE>::GetNormal() const
	{
		return myNorm;
	}
};