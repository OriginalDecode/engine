#pragma once
#include "Vector4decl.h"
namespace CommonUtilities
{
	typedef Math::Vector4<float> Vector4f;
	typedef Math::Vector4<int> Vector4i;

	namespace Math
	{
		template<typename T>
		Vector4<T>::Vector4(const T& anX, const T& anY, const T& anZ, const T& anW)
		{
			x = anX;
			y = anY;
			z = anZ;
			w = anW;
		};

		template<typename T>
		Vector4<T>::Vector4(const Vector3<T>& aVector, T anW)
		{
			x = aVector.x;
			y = aVector.y;
			z = aVector.z;
			w = anW;
		}

		template<typename T>
		Vector4<T>::Vector4(const Vector2<T>& aVector, T anZ, T anW)
		{
			x = aVector.x;
			y = aVector.y;
			z = anZ;
			w = anW;
		}

		template<typename T>
		Vector4<T>::Vector4(T anX, T anY, const Vector2<T>& aVector)
		{
			x = anX;
			y = anY;
			z = aVector.X;
			w = aVector.Y;
		}
		template<typename T> 
		Vector4<T>::Vector4(const Vector2<T>& aVector, const Vector2<T>& aSecondVector)
		{
			x = aVector.x;
			y = aVector.y;
			z = aSecondVector.x;
			w = aSecondVector.y;
		}


		template<typename T>
		Vector4<T>::~Vector4()
		{
		};

		/****************************\
		|		Normal Operators	 |
		\****************************/
		template<typename T>
		inline const Vector4<T> operator+(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x + aSecondVector.x),
				(aFirstVector.y + aSecondVector.y),
				(aFirstVector.z + aSecondVector.z),
				(aFirstVector.w + aSecondVector.w));
		}
		template<typename T>
		inline const Vector4<T> operator-(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x - aSecondVector.x),
				(aFirstVector.y - aSecondVector.y),
				(aFirstVector.z - aSecondVector.z),
				(aFirstVector.w - aSecondVector.w));
		}
		template<typename T>
		inline 	const Vector4<T> operator*(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>((aFirstVector.x * aSecondVector.x),
				(aFirstVector.y * aSecondVector.y),
				(aFirstVector.z * aSecondVector.z),
				(aFirstVector.w * aSecondVector.w));
		}
		template<typename T>
		inline const Vector4<T> operator/(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
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
		inline Vector4<T> operator+=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x += aSecondVector.x;
			aFirstVector.y += aSecondVector.y;
			aFirstVector.z += aSecondVector.z;
			aFirstVector.w += aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		inline Vector4<T> operator-=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x -= aSecondVector.x;
			aFirstVector.y -= aSecondVector.y;
			aFirstVector.z -= aSecondVector.z;
			aFirstVector.w -= aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		inline Vector4<T> operator*=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			aFirstVector.x *= aSecondVector.x;
			aFirstVector.y *= aSecondVector.y;
			aFirstVector.z *= aSecondVector.z;
			aFirstVector.w *= aSecondVector.w;
			return aFirstVector;
		}
		template<typename T>
		inline Vector4<T> operator/=(Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
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
		inline const Vector4<T> operator+(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>
				((aFirstVector.x + aScale),
				(aFirstVector.y + aScale),
				(aFirstVector.z + aScale),
				(aFirstVector.w + aScale));
		}
		template<typename T>
		inline const Vector4<T> operator-(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>
				((aFirstVector.x - aScale),
				(aFirstVector.y - aScale),
				(aFirstVector.z - aScale),
				(aFirstVector.w - aScale));
		}
		template<typename T>
		inline const Vector4<T> operator*(const Vector4<T>& aFirstVector, const T aScale)
		{
			return Vector4<T>((aFirstVector.x *aScale),
				(aFirstVector.y *aScale),
				(aFirstVector.z *aScale),
				(aFirstVector.w *aScale));
		}
		template<typename T>
		inline const Vector4<T> operator/(const Vector4<T>& aFirstVector, const T aScale)
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
		inline const Vector4<T> operator+=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x += aScale;
			aFirstVector.y += aScale;
			aFirstVector.z += aScale;
			aFirstVector.w += aScale;
			return aFirstVector;
		}
		template<typename T>
		inline const Vector4<T> operator-=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x -= aScale;
			aFirstVector.y -= aScale;
			aFirstVector.z -= aScale;
			aFirstVector.w -= aScale;
			return aFirstVector;
		}
		template<typename T>
		inline const Vector4<T> operator*=(Vector4<T>& aFirstVector, const T aScale)
		{
			aFirstVector.x *= aScale;
			aFirstVector.y *= aScale;
			aFirstVector.z *= aScale;
			aFirstVector.w *= aScale;
			return aFirstVector;
		}
		template<typename T>
		inline const Vector4<T> operator/=(Vector4<T>& aFirstVector, const T aScale)
		{
			if (aScale <= 0.f)
				return CU::Vector4f();
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
			return sqrtf((aVector.x * aVector.x) +
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

		template<typename T>
		const Vector4<T> Cross(const Vector4<T>& aFirstVector, const Vector4<T>& aSecondVector)
		{
			return Vector4<T>(
				( aFirstVector.y*aSecondVector.z ) - ( aFirstVector.z*aSecondVector.y ),
				( aFirstVector.z*aSecondVector.x ) - ( aFirstVector.x*aSecondVector.z ),
				( aFirstVector.x*aSecondVector.y ) - ( aFirstVector.y*aSecondVector.x ),
				1);
		}


		template<typename T>
		bool operator==(const Vector4<T>& aFirst, const Vector4<T>& aSecond)
		{
			if (aFirst.x != aSecond.x)
			{
				return false;
			}

			if (aFirst.y != aSecond.y)
			{
				return false;
			}

			if (aFirst.z != aSecond.z)
			{
				return false;
			}

			if (aFirst.w != aSecond.w)
			{
				return false;
			}

			return true;
		}

		template<typename T>
		bool operator!=(const Vector4<T>& aFirst, const Vector4<T>& aSecond)
		{
			return !(aFirst == aSecond);
		}

		template<typename T>
		bool operator>(const Vector4<T>& aFirst, const Vector4<T>& aSecond)
		{
			if (aFirst.x < aSecond.x)
			{
				return false;
			}

			if (aFirst.y < aSecond.y)
			{
				return false;
			}

			if (aFirst.z < aSecond.z)
			{
				return false;
			}

			return true;
		}

		template<typename T>
		bool operator>(const Vector4<T>& aFirst, const Vector3<T>& aSecond)
		{
			if (aFirst.x < aSecond.x)
			{
				return false;
			}

			if (aFirst.y < aSecond.y)
			{
				return false;
			}

			if (aFirst.z < aSecond.z)
			{
				return false;
			}

			return true;
		}

		template<typename T>
		bool operator<(const Vector4<T>& aFirst, const Vector3<T>& aSecond)
		{
			if (aFirst > aSecond)
				return false;

			return true;
		}

		template<typename T>
		bool operator<(const Vector4<T>& aFirst, const Vector4<T>& aSecond)
		{
			if (aFirst > aSecond)
				return false;

			return true;
		}

		template<typename T>
		bool operator<(const Vector4<T>& first, float scalar)
		{
			if (first.x > scalar)
				return false;
			if (first.y > scalar)
				return false;
			if (first.z > scalar)
				return false;
			if (first.w > scalar)
				return false;
			return true;


		}



	};
};
namespace CU = CommonUtilities;
