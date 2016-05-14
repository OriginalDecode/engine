#pragma once
#include "Vector3decl.h"
namespace CommonUtilities
{
	typedef Math::Vector3<float> Vector3f;
	typedef Math::Vector3<int> Vector3i;

	namespace Math
	{
		template<typename T>
		Vector3<T>::Vector3(T anX, T anY, T anZ)
		{
			myX = anX;
			myY = anY;
			myZ = anZ;
		};


		template<typename T>
		Vector3<T>::Vector3(const Vector2<T>& aVector, T aZ)
		{
			myX = aVector.x;
			myY = aVector.y;
			myZ = aZ;
		}

		template<typename T>
		Vector3<T>::~Vector3()
		{
		};

		/****************************\
		|		Normal Operators	 |
		\****************************/

		template<typename T>
		__forceinline Vector3<T> operator+(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			return Vector3<T>((aFirstVector.x + aSecondVector.x),
				(aFirstVector.y + aSecondVector.y),
				(aFirstVector.z + aSecondVector.z));
		}

		template<typename T>
		__forceinline Vector3<T> operator-(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			return Vector3<T>((aFirstVector.x - aSecondVector.x),
				(aFirstVector.y - aSecondVector.y),
				(aFirstVector.z - aSecondVector.z));
		}

		template<typename T>
		__forceinline Vector3<T> operator*(const Vector3<T>& aFirstVector, const Vector3<T> aSecondVector)
		{
			return Vector3<T>((aFirstVector.x * aSecondVector.x),
				(aFirstVector.y * aSecondVector.y),
				(aFirstVector.z * aSecondVector.z));
		}

		template<typename T>
		__forceinline Vector3<T> operator/(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && aSecondVector.z >= 0 && "Can't divide by Zero");
			return Vector3<T>((aFirstVector.x / aSecondVector.x),
				(aFirstVector.y / aSecondVector.y),
				(aFirstVector.z / aSecondVector.z));
		}
		/****************************\
		|		Compound Operators	 |
		\****************************/
		template<typename T>
		__forceinline void operator+=(Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			aFirstVector.x += aSecondVector.x;
			aFirstVector.y += aSecondVector.y;
			aFirstVector.z += aSecondVector.z;
		}

		template<typename T>
		__forceinline void operator-=(Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			aFirstVector.x -= aSecondVector.x;
			aFirstVector.y -= aSecondVector.y;
			aFirstVector.z -= aSecondVector.z;
		}

		template<typename T>
		__forceinline void operator*=(Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			aFirstVector.x *= aSecondVector.x;
			aFirstVector.y *= aSecondVector.y;
			aFirstVector.z *= aSecondVector.z;
		}

		template<typename T>
		__forceinline void operator/=(Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && aSecondVector.z >= 0 && "Can't divide by Zero");
			aFirstVector.x /= aSecondVector.x;
			aFirstVector.y /= aSecondVector.y;
			aFirstVector.z /= aSecondVector.z;
		}

		/****************************\
		|	Normal Scalar Operators	 |
		\****************************/
		template<typename T>
		__forceinline Vector3<T> operator+(const Vector3<T>& aFirstVector, const T aScale)
		{
			return Vector3<T>((aFirstVector.x + aScale),
				(aFirstVector.y + aScale),
				(aFirstVector.z + aScale));
		}

		template<typename T>
		__forceinline Vector3<T> operator-(const Vector3<T>& aFirstVector, const T aScale)
		{
			return Vector3<T>((aFirstVector.x - aScale),
				(aFirstVector.y - aScale),
				(aFirstVector.z - aScale));
		}

		template<typename T>
		__forceinline Vector3<T> operator*(const Vector3<T>& aFirstVector, const T aScale)
		{
			return Vector3<T>((aFirstVector.x*aScale),
				(aFirstVector.y*aScale),
				(aFirstVector.z*aScale));
		}

		template<typename T>
		__forceinline Vector3<T> operator/(const Vector3<T>& aFirstVector, const T aScale)
		{
			assert(aScale >= 0 && "Can't divide by Zero");
			return Vector3<T>((aFirstVector.x / aScale),
				(aFirstVector.y / aScale),
				(aFirstVector.z / aScale));
		}

		/****************************\
		|	   Normal Compound       |
		|	   Scalar Operators	     |
		\****************************/

		template<typename T>
		__forceinline void operator+=(Vector3<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x += aScale;
			aFirstVector.y += aScale;
			aFirstVector.z += aScale;
		}

		template<typename T>
		__forceinline void operator-=(Vector3<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x -= aScale;
			aFirstVector.y -= aScale;
			aFirstVector.z -= aScale;
		}

		template<typename T>
		__forceinline void operator*=(Vector3<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x *= aScale;
			aFirstVector.y *= aScale;
			aFirstVector.z *= aScale;
		}


		template<typename T>
		__forceinline  void operator/=(Vector3<T>& aFirstVector, const T aScale)
		{
			assert(aScale >= 0 && "Can't divide by Zero");
			aFirstVector.x /= aScale;
			aFirstVector.y /= aScale;
			aFirstVector.z /= aScale;
		}
		/****************************\
		|		Other Operations     |
		\****************************/

		template <typename T>
		const T Length(const Vector3<T>& aVector)
		{
			return sqrt((aVector.x * aVector.x) +
				(aVector.y*aVector.y) +
				(aVector.z*aVector.z));
		};

		template <typename T>
		const T Length2(const Vector3<T>& aVector)
		{
			return (Length(aVector)*Length(aVector));
		};

		template <typename T>
		void Normalize(Vector3<T>& aVector)
		{
			aVector /= Length(aVector);
		}

		template <typename T>
		Vector3<T> GetNormalized(const Vector3<T>& aVector)
		{
			return aVector / Length(aVector);
		}

		template <typename T>
		T Dot(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			return ((aFirstVector.x*aSecondVector.x) + (aFirstVector.y*aSecondVector.y) + (aFirstVector.z*aSecondVector.z));
		}

		template<typename T>
		const Vector3<T> Cross(const Vector3<T>& aFirstVector, const Vector3<T>& aSecondVector)
		{
			return Vector3<T>((aFirstVector.y*aSecondVector.z) - (aFirstVector.z*aSecondVector.y),
				(aFirstVector.z*aSecondVector.x) - (aFirstVector.x*aSecondVector.z),
				(aFirstVector.x*aSecondVector.y) - (aFirstVector.y*aSecondVector.x));
		}
	};
};
namespace CU = CommonUtilities;
