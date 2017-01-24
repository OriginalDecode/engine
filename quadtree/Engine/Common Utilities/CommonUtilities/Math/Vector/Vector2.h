#pragma once
#include "Vector2decl.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<class TYPE>
		Vector2<TYPE>::Vector2(TYPE anX, TYPE anY)
		{
			myX = anX;
			myY = anY;
		};

		template<class TYPE>
		Vector2<TYPE>::~Vector2()
		{
		};



		/****************************\
		|		Normal Operators	 |
		\****************************/
		template<class TYPE>
		const Vector2<TYPE> operator+(const Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			return Vector2<TYPE>(aFirstVector.x + aSecondVector.x, aFirstVector.y + aSecondVector.y);
		}

		template<class TYPE>
		const Vector2<TYPE> operator-(const Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			return Vector2<TYPE>(aFirstVector.x - aSecondVector.x, aFirstVector.y - aSecondVector.y);
		}

		template<class TYPE>
		const Vector2<TYPE> operator*(const Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			return Vector2<TYPE>(aFirstVector.x * aSecondVector.x, aFirstVector.y * aSecondVector.y);
		}

		template<class TYPE>
		const Vector2<TYPE> operator/(const Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && "Can't divide by Zero");
			return Vector2<TYPE>(aFirstVector.x / aSecondVector.x, aFirstVector.y / aSecondVector.y);
		}



		/****************************\
		|		Compound Operators	 |
		\****************************/

		template<class TYPE>
		Vector2<TYPE> operator+=(Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			aFirstVector.x += aSecondVector.x;
			aFirstVector.y += aSecondVector.y;
			return aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator-=(Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			aFirstVector.x -= aSecondVector.x;
			aFirstVector.y -= aSecondVector.y;
			return aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator*=(Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			aFirstVector.x *= aSecondVector.x;
			aFirstVector.y *= aSecondVector.y;
			return aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator/=(Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && "Can't divide by Zero");
			aFirstVector.x /= aSecondVector.x;
			aFirstVector.y /= aSecondVector.y;
			return aFirstVector;
		}




		/****************************\
		|	Normal Scalar Operators	 |
		\****************************/

		template<class TYPE>
		const Vector2<TYPE> operator+(const Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			return Vector2<TYPE>(aFirstVector.x + aScale, aFirstVector.y + aScale);
		}

		template<class TYPE>
		const Vector2<TYPE> operator-(const Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			return Vector2<TYPE>(aFirstVector.x - aScale, aFirstVector.y - aScale);
		}

		template<class TYPE>
		const Vector2<TYPE> operator*(const Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{

			return Vector2<TYPE>(aFirstVector.x *aScale, aFirstVector.y *aScale);
		}

		template<class TYPE>
		const Vector2<TYPE> operator/(const Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			assert(aScale >= 0 && "Can't divide by Zero");
			return Vector2<TYPE>(aFirstVector.x / aScale, aFirstVector.y / aScale);
		}



		/****************************\
		|	   Normal Compound       |
		|	   Scalar Operators	     |
		\****************************/

		template<class TYPE>
		Vector2<TYPE> operator+=(Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			aFirstVector.x += aScale;
			aFirstVector.y += aScale;
			return aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator-=(Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			aFirstVector.x -= aScale;
			aFirstVector.y -= aScale;
			return aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator*=(Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			aFirstVector.x *= aScale;
			aFirstVector.y *= aScale;
			return  aFirstVector;
		}

		template<class TYPE>
		Vector2<TYPE> operator/=(Vector2<TYPE>& aFirstVector, const TYPE aScale)
		{
			assert(aScale >= 0 && "Can't divide by Zero");
			aFirstVector.x /= aScale;
			aFirstVector.y /= aScale;
			return aFirstVector;
		}




		/****************************\
		|		Other Operations     |
		\****************************/

		template <class TYPE>
		const TYPE Length(const Vector2<TYPE>& aVector)
		{
			return sqrt((aVector.x * aVector.x) + (aVector.y*aVector.y));
		};

		template <class TYPE>
		const TYPE Length2(const Vector2<TYPE>& aVector)
		{
			return ((aVector.x * aVector.x) + (aVector.y*aVector.y));
		};

		template <class TYPE>
		void Normalize(Vector2<TYPE>& aVector)
		{
			aVector /= Length(aVector);
		}

		template <class TYPE>
		Vector2<TYPE> GetNormalized(const Vector2<TYPE>& aVector)
		{
			return aVector / Length(aVector);
		}

		template <class TYPE>
		TYPE Dot(const Vector2<TYPE>& aFirstVector, const Vector2<TYPE>& aSecondVector)
		{
			return ((aFirstVector.x*aSecondVector.x) + (aFirstVector.y*aSecondVector.y));
		}

	};
};
namespace CU = CommonUtilities;
