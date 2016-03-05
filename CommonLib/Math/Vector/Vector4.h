#pragma once
#include "Vector4decl.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<typename T>
		Vector4<T>::Vector4(const T& anX, const T& anY, const T& anZ, const T& anW)
		{
			myX = anX;
			myY = anY;
			myZ = anZ;
			myW = anW;
		};

		//template<typename T>
		//Vector4<T>::Vector4(const Vector3<T>& aVector, T anW)
		//{
		//	myX = aVector.X;
		//	myY = aVector.Y;
		//	myZ = aVector.Z;
		//	myW = anW;

		//}

		template<typename T>
		Vector4<T>::Vector4(const Vector2<T>& aVector, T anZ, T anW)
		{
			myX = aVector.X;
			myY = aVector.Y;
			myZ = anZ;
			myW = anW;
		}

		template<typename T>
		Vector4<T>::Vector4(T anX, T anY, const Vector2<T>& aVector)
		{
			myX = anX;
			myY = anY;
			myZ = aVector.X;
			myW = aVector.Y;
		}
		template<typename T> 
		Vector4<T>::Vector4(const Vector2<T>& aVector, const Vector2<T>& aSecondVector)
		{
			myX = aVector.x;
			myY = aVector.y;
			myZ = aSecondVector.x;
			myW = aSecondVector.y;
		}


		template<typename T>
		Vector4<T>::~Vector4()
		{
		};

		/****************************\
		|		Normal Operators	 |
		\****************************/
		template<typename T>
		__forceinline const Vector4<T> operator+(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x + aSecondVector.x),
				(aFirstVector.y + aSecondVector.y),
				(aFirstVector.z + aSecondVector.z),
				(aFirstVector.w + aSecondVector.w));
		}
		template<typename T>
		__forceinline const Vector4<T> operator-(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x - aSecondVector.x),
				(aFirstVector.y - aSecondVector.y),
				(aFirstVector.z - aSecondVector.z),
				(aFirstVector.w - aSecondVector.w));
		}
		template<typename T>
		__forceinline 	const Vector4<T> operator*(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x * aSecondVector.x),
				(aFirstVector.y * aSecondVector.y),
				(aFirstVector.z * aSecondVector.z),
				(aFirstVector.w * aSecondVector.w));
		}
		template<typename T>
		__forceinline const Vector4<T> operator/(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && aSecondVector.z >= 0 && aSecondVector.w >= 0 && "Can't divide by Zero");
			return Vector4<T>((aFirstVector.x / aSecondVector.x),
				(aFirstVector.y / aSecondVector.y),
				(aFirstVector.z / aSecondVector.z),
				(aFirstVector.w / aSecondVector.w));
		}
		/****************************\
		|		Compound Operators	 |
		\****************************/
		template<typename T>
		__forceinline Vector4<T> operator+=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x += aSecondVector.x;
			aFirstVector.y += aSecondVector.y;
			aFirstVector.z += aSecondVector.z;
			aFirstVector.w += aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		__forceinline Vector4<T> operator-=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x -= aSecondVector.x;
			aFirstVector.y -= aSecondVector.y;
			aFirstVector.z -= aSecondVector.z;
			aFirstVector.w -= aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		__forceinline Vector4<T> operator*=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x *= aSecondVector.x;
			aFirstVector.y *= aSecondVector.y;
			aFirstVector.z *= aSecondVector.z;
			aFirstVector.w *= aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		__forceinline Vector4<T> operator/=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			assert(aSecondVector.x != 0 && aSecondVector.y != 0 && aSecondVector.z != 0 && aSecondVector.w != 0 && "Can't divide by Zero");
			aFirstVector.x /= aSecondVector.x;
			aFirstVector.y /= aSecondVector.y;
			aFirstVector.z /= aSecondVector.z;
			aFirstVector.w /= aSecondVector.w;
			return aFirstVector;
		}
		/****************************\
		|	Normal Scalar Operators	 |
		\****************************/
		template<typename T>
		__forceinline const Vector4<T> operator+(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>
				((aFirstVector.x + aScale),
				(aFirstVector.y + aScale),
				(aFirstVector.z + aScale),
				(aFirstVector.w + aScale));
		}
		template<typename T>
		__forceinline const Vector4<T> operator-(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>
				((aFirstVector.x - aScale),
				(aFirstVector.y - aScale),
				(aFirstVector.z - aScale),
				(aFirstVector.w - aScale));
		}
		template<typename T>
		__forceinline const Vector4<T> operator*(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>((aFirstVector.x *aScale),
				(aFirstVector.y *aScale),
				(aFirstVector.z *aScale),
				(aFirstVector.w *aScale));
		}
		template<typename T>
		__forceinline const Vector4<T> operator/(const Vector4<T>& aFirstVector, const T aScale)
		{
			assert(aScale != 0 && "Can't divide by Zero");
			return Vector4<T>((aFirstVector.x / aScale),
				(aFirstVector.y / aScale),
				(aFirstVector.z / aScale),
				(aFirstVector.w / aScale));
		}
		/****************************\
		|	   Normal Compound       |
		|	   Scalar Operators	     |
		\****************************/
		template<typename T>
		__forceinline const Vector4<T> operator+=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x += aScale;
			aFirstVector.y += aScale;
			aFirstVector.z += aScale;
			aFirstVector.w += aScale;
			return aFirstVector;
		}
		template<typename T>
		__forceinline const Vector4<T> operator-=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x -= aScale;
			aFirstVector.y -= aScale;
			aFirstVector.z -= aScale;
			aFirstVector.w -= aScale;
			return aFirstVector;
		}
		template<typename T>
		__forceinline const Vector4<T> operator*=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x *= aScale;
			aFirstVector.y *= aScale;
			aFirstVector.z *= aScale;
			aFirstVector.w *= aScale;
			return aFirstVector;
		}
		template<typename T>
		__forceinline const Vector4<T> operator/=(Vector4<T>& aFirstVector, const T aScale)
		{
			assert(aScale != 0 && "Can't divide by Zero");
			aFirstVector.x /= aScale;
			aFirstVector.y /= aScale;
			aFirstVector.z /= aScale;
			aFirstVector.w /= aScale;
			return aFirstVector;
		}
		/****************************\
		|		Other Operations     |
		\****************************/

		template <typename T>
		const T Length(const Vector4<T>& aVector)
		{
			return sqrt((aVector.x * aVector.x) +
				(aVector.y * aVector.y) +
				(aVector.z * aVector.z) +
				(aVector.w * aVector.w));
		};

		template <typename T>
		const T Length2(const Vector4<T>& aVector)
		{
			return (Length(aVector)*Length(aVector));
		};

		template <typename T>
		void Normalize(Vector4<T>& aVector)
		{
			aVector /= Length(aVector);
		}

		template <typename T>
		const Vector4<T> GetNormalized(const Vector4<T>& aVector)
		{
			return aVector / Length(aVector);
		}

		template <typename T>
		T Dot(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return ((aFirstVector.x*aSecondVector.x) +
				(aFirstVector.y*aSecondVector.y) +
				(aFirstVector.z*aSecondVector.z) +
				(aFirstVector.w*aSecondVector.w));
		}


	};
};
namespace CU = CommonUtilities;
typedef CU::Math::Vector4<float> Vector4f;
