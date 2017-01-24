#pragma once
#include "..\Vector\Vector.h"
namespace CommonUtilities
{
	namespace Math
	{

		template<typename TYPE>
		class Plane
		{
		public:
			Plane();
			Plane(Vector3<TYPE>aFirstPoint, Vector3<TYPE> aSecondPoint, Vector3<TYPE> aThirdPoint);
			Plane(Vector3<TYPE> aPoint, Vector3<TYPE> aNormal);
			~Plane();

			bool operator==(Plane<TYPE> aPlane);
			void InitWith3Points(Vector3<TYPE> aFirstPoint, Vector3<TYPE> aSecondPoint, Vector3<TYPE> aThirdPoint);
			void InitWithPointAndNormal(Vector3<TYPE> aPoint, Vector3<TYPE> aNormal);
			bool Inside(Vector3<TYPE> aPosition);
			void IncreaseDistance(TYPE aDistance);

		private:

			Vector4<TYPE> myData;
		};

		template<typename TYPE>
		Plane<TYPE>::Plane()
		{
		}

		template<typename TYPE>
		Plane<TYPE>::~Plane()
		{
		}

		template<typename TYPE>
		Plane<TYPE>::Plane(Vector3<TYPE> aFirstPoint, Vector3<TYPE> aSecondPoint, Vector3<TYPE> aThirdPoint)
		{
			InitWith3Points(aFirstPoint, aSecondPoint, aThirdPoint);
		}

		template<typename TYPE>
		Plane<TYPE>::Plane(Vector3<TYPE> aPoint, Vector3<TYPE> aNormal)
		{
			InitWithPointAndNormal(aPoint, aNormal);
		}

		template<typename TYPE>
		void Plane<TYPE>::InitWith3Points(Vector3<TYPE> aFirstPoint, Vector3<TYPE> aSecondPoint, Vector3<TYPE> aThirdPoint)
		{
			myPoint = aFirstPoint;

			Vector3<TYPE> temp(Cross(aFirstPoint - aSecondPoint, aThirdPoint - aSecondPoint));

			myData = temp;
			myData.w = Dot(temp, aFirstPoint);
		}

		template<typename TYPE>
		void Plane<TYPE>::InitWithPointAndNormal(Vector3<TYPE> aPoint, Vector3<TYPE> aNormal)
		{
			myData = aNormal;
			myData.w = Dot(aNormal, aPoint);
		}

		template<typename TYPE>
		bool Plane<TYPE>::Inside(Vector3<TYPE> aPosition)
		{
			Vector3<TYPE> normal(myData.x, myData.y, myData.z);
			return Dot(aPosition, normal) - myData.w <= 0;
		}

		template <typename TYPE>
		void Plane<TYPE>::IncreaseDistance(TYPE aDistance)
		{
			myData.w += aDistance;
		}

		template<typename TYPE>
		bool Plane<TYPE>::operator==(Plane<TYPE> aPlane)
		{
			if (myData == aPlane)
			{
				return true;
			}

			return false;
		}

	}

};
namespace CU = CommonUtilities;