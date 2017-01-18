#pragma once
#include "../Vector/Vector.h"
namespace CommonUtilities
{
	template<typename TYPE>
	class Plane
	{
	public:
		Plane() = default;
		Plane(Math::Vector4 <TYPE>aFirstPoint, Math::Vector4 <TYPE> aSecondPoint, Math::Vector4 <TYPE> aThirdPoint);
		Plane(Math::Vector4 <TYPE> aPoint, Math::Vector4 <TYPE> aNormal);


		void InitWith3Points(Math::Vector4 <TYPE> aFirstPoint, Math::Vector4 <TYPE> aSecondPoint, Math::Vector4 <TYPE> aThirdPoint);
		void InitWithPointAndNormal(Math::Vector4 <TYPE> aPoint, Math::Vector4 <TYPE> aNormal);

		bool Inside(Math::Vector4 <TYPE> aPosition);

		Math::Vector4 <TYPE> GetNormal() const;
		Math::Vector4 <TYPE> GetPoint() const;

		void SetNormal(Math::Vector4 <TYPE> normal);
		void SetPoint(Math::Vector4 <TYPE> position);

	private:

		Math::Vector4 <TYPE> myNorm;
		Math::Vector4 <TYPE> myPoint;
	};

	template<typename TYPE>
	void CommonUtilities::Plane<TYPE>::SetPoint(Math::Vector4 <TYPE> position)
	{
		myPoint = position;
	}

	template<typename TYPE>
	void CommonUtilities::Plane<TYPE>::SetNormal(Math::Vector4 <TYPE> normal)
	{
		myNorm = normal;
	}

	template<typename TYPE>
	Plane<TYPE>::Plane(Math::Vector4 <TYPE> aFirstPoint, Math::Vector4 <TYPE> aSecondPoint, Math::Vector4 <TYPE> aThirdPoint)
	{
		InitWith3Points(aFirstPoint, aSecondPoint, aThirdPoint);
	}

	template<typename TYPE>
	Plane<TYPE>::Plane(Math::Vector4 <TYPE> aPoint, Math::Vector4 <TYPE> aNormal)
	{
		InitWithPointAndNormal(aPoint, aNormal);
	}

	template<typename TYPE>
	void Plane<TYPE>::InitWith3Points(Math::Vector4 <TYPE> aFirstPoint, Math::Vector4 <TYPE> aSecondPoint, Math::Vector4 <TYPE> aThirdPoint)
	{
		myPoint = (aSecondPoint + aThirdPoint) / 2.f;

		myNorm = Cross(aSecondPoint - aFirstPoint, aThirdPoint - aFirstPoint);

		Normalize(myNorm);
	}

	template<typename TYPE>
	void Plane<TYPE>::InitWithPointAndNormal(Math::Vector4 <TYPE> aPoint, Math::Vector4 <TYPE> aNormal)
	{
		myPoint = aPoint;
		myNorm = aNormal;
		Normalize(myNorm);
	}

	template<typename TYPE>
	bool Plane<TYPE>::Inside(Math::Vector4 <TYPE> aPosition)
	{
		if (Math::Dot<float>(aPosition - myPoint, myNorm) > 0.f)
			return false;
		else
			return true;
	}

	template<typename TYPE>
	Math::Vector4 <TYPE> Plane<TYPE>::GetPoint() const
	{
		return myPoint;
	}

	template<typename TYPE>
	Math::Vector4 <TYPE> Plane<TYPE>::GetNormal() const
	{
		return myNorm;
	}
};