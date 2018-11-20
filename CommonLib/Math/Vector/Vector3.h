#pragma once
#include "Vector3decl.h"
namespace CommonUtilities
{
	typedef Math::Vector3<float> Vector3f;
	typedef Math::Vector3<int> Vector3i;

	namespace Math
	{
		// Normal operators
		template<typename T>
		inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
		}

		template<typename T>
		inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
		}

		template<typename T>
		inline Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T> rhs)
		{
			return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
		}

		template<typename T>
		inline Vector3<T> operator/(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			assert(rhs.x >= 0 && rhs.y >= 0 && rhs.z >= 0 && "Can't divide by Zero");
			return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
		}
		
		// Compound operators
		template<typename T>
		inline Vector3<T>& operator+=(Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			lhs.z += rhs.z;
			return lhs;
		}

		template<typename T>
		inline Vector3<T>& operator-=(Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			lhs.z -= rhs.z;
			return lhs;
		}

		template<typename T>
		inline Vector3<T>& operator*=(Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;
			lhs.z *= rhs.z;
			return lhs;
		}

		template<typename T>
		inline Vector3<T>& operator/=(Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			assert(rhs.x >= 0 && rhs.y >= 0 && rhs.z >= 0 && "Can't divide by Zero");
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;
			lhs.z /= rhs.z;
			return lhs;
		}

		// Normal scalar operators
		template<typename T>
		inline Vector3<T> operator+(const Vector3<T>& lhs, const T scale)
		{
			return { lhs.x + scale, lhs.y + scale, lhs.z + scale };
		}

		template<typename T>
		inline Vector3<T> operator-(const Vector3<T>& lhs, const T scale)
		{
			return { lhs.x - scale, lhs.y - scale, lhs.z - scale };
		}

		template<typename T>
		inline Vector3<T> operator*(const Vector3<T>& lhs, const T scale)
		{
			return { lhs.x * scale, lhs.y * scale, lhs.z * scale };
		}

		template<typename T>
		inline Vector3<T> operator/(const Vector3<T>& lhs, const T scale)
		{
			assert(scale >= 0 && "Can't divide by Zero");
			return { lhs.x / scale, lhs.y / scale, lhs.z / scale };
		}

		// Compound Scalard Operators
		template<typename T>
		inline Vector3<T>& operator+=(Vector3<T>& lhs, const T scale)
		{
			lhs.x += scale;
			lhs.y += scale;
			lhs.z += scale;
			return lhs;
		}

		template<typename T>
		inline Vector3<T>& operator-=(Vector3<T>& lhs, const T scale)
		{
			lhs.x -= scale;
			lhs.y -= scale;
			lhs.z -= scale;
			return lhs;
		}

		template<typename T>
		inline Vector3<T>& operator*=(Vector3<T>& lhs, const T scale)
		{
			lhs.x *= scale;
			lhs.y *= scale;
			lhs.z *= scale;
			return lhs;
		}


		template<typename T>
		inline Vector3<T>& operator/=(Vector3<T>& lhs, const T scale)
		{
			assert(scale >= 0 && "Can't divide by Zero");
			lhs.x /= scale;
			lhs.y /= scale;
			lhs.z /= scale;
			return lhs;
		}
	
		// Utilities
		template <typename T>
		const T Length(const Vector3<T>& aVector)
		{
			return sqrtf((aVector.x * aVector.x) +
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
			if (Length(aVector) > 0.f)
				aVector /= Length(aVector);
		}

		template <typename T>
		Vector3<T> GetNormalized(const Vector3<T>& aVector)
		{
			T sqrt = Length(aVector);
			if (sqrt > 0)
				return aVector / sqrt;

			return Vector3<T>(0, 0, 0);
		}

		template <typename T>
		T Dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			return ((lhs.x*rhs.x) + (lhs.y*rhs.y) + (lhs.z*rhs.z));
		}

		template<typename T>
		const Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			return { (lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z), (lhs.x * rhs.y) - (lhs.y * rhs.x) };
		}



		template<typename T>
		bool operator==(const Vector3<T>& aFirst, const Vector3<T>& aSecond)
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

			return true;
		}

		template<typename T>
		bool operator!=(const Vector3<T>& aFirst, const Vector3<T>& aSecond)
		{
			return !(aFirst == aSecond);
		}

		template<typename T>
		bool operator<(const Vector3<T>& aFirst, const Vector3<T>& aSecond)
		{
			if (aFirst.x > aSecond.x)
			{
				return false;
			}

			if (aFirst.y > aSecond.y)
			{
				return false;
			}

			if (aFirst.z > aSecond.z)
			{
				return false;
			}

			return true;
		}

		template<typename T>
		bool operator>(const Vector3<T>& aFirst, const Vector3<T>& aSecond)
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



	};
};
namespace CU = CommonUtilities;
