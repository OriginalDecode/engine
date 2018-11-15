#pragma once
#include "Vector4decl.h"
namespace CommonUtilities
{
	typedef Math::Vector4<float> Vector4f;
	typedef Math::Vector4<int> Vector4i;

	namespace Math
	{
		// Normal Operators
		template<typename T>
		inline const Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return { (lhs.x + rhs.x), (lhs.y + rhs.y), (lhs.z + rhs.z), (lhs.w + rhs.w) };
		}

		template<typename T>
		inline const Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return { (lhs.x - rhs.x), (lhs.y - rhs.y), (lhs.z - rhs.z), (lhs.w - rhs.w) };
		}
			
		template<typename T>
		inline 	const Vector4<T> operator*(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return { (lhs.x * rhs.x), (lhs.y * rhs.y), (lhs.z * rhs.z), (lhs.w * rhs.w) };
		}
	
		template<typename T>
		inline const Vector4<T> operator/(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			assert(aSecondVector.x >= 0 && aSecondVector.y >= 0 && aSecondVector.z >= 0 && aSecondVector.w >= 0 && "Can't divide by Zero");
			return { (lhs.x / rhs.x), (lhs.y / rhs.y), (lhs.z / rhs.z), (lhs.w / rhs.w) };
		}

		// Compound Operators
		template<typename T>
		inline Vector4<T>& operator+=(Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			lhs.z += rhs.z;
			lhs.w += rhs.w;
			return lhs;
		}

		template<typename T>
		inline Vector4<T>& operator-=(Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			lhs.z -= rhs.z;
			lhs.w -= rhs.w;
			return lhs;
		}
		template<typename T>
		inline Vector4<T>& operator*=(Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;
			lhs.z *= rhs.z;
			lhs.w *= rhs.w;
			return lhs;
		}
		template<typename T>
		inline Vector4<T>& operator/=(Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			assert(aSecondVector.x != 0 && aSecondVector.y != 0 && aSecondVector.z != 0 && aSecondVector.w != 0 && "Can't divide by Zero");
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;
			lhs.z /= rhs.z;
			lhs.w /= rhs.w;
			return lhs;
		}
		
		// Normal operators Scalar
		template<typename T>
		inline Vector4<T> operator+(const Vector4<T>& lhs, const T scale)
		{
			return { lhs.x + scale, lhs.y + scale, lhs.z + scale, lhs.w + scale };
		}
		
		template<typename T>
		inline Vector4<T> operator-(const Vector4<T>& lhs, const T scale)
		{
			return { lhs.x - scale, lhs.y - scale, lhs.z - scale, lhs.w - scale };
		}
		
		template<typename T>
		inline Vector4<T> operator*(const Vector4<T>& lhs, const T scale)
		{
			return { lhs.x * scale, lhs.y * scale, lhs.z * scale, lhs.w * scale };
		}

		template<typename T>
		inline const Vector4<T> operator/(const Vector4<T>& lhs, const T scale)
		{
			assert(scale != 0 && "Can't divide by Zero");
			return { lhs.x / scale, lhs.y / scale, lhs.z / scale, lhs.w / scale };
		}
		
		// Compund scale
		template<typename T>
		inline Vector4<T>& operator+=(Vector4<T>& lhs, const T scale)
		{
			lhs.x += scale;
			lhs.y += scale;
			lhs.z += scale;
			lhs.w += scale;
			return lhs;
		}

		template<typename T>
		inline Vector4<T>& operator-=(Vector4<T>& lhs, const T scale)
		{
			lhs.x -= aScale;
			lhs.y -= aScale;
			lhs.z -= aScale;
			lhs.w -= aScale;
			return lhs;
		}
		
		template<typename T>
		inline Vector4<T>& operator*=(Vector4<T>& lhs, const T scale)
		{
			lhs.x *= scale;
			lhs.y *= scale;
			lhs.z *= scale;
			lhs.w *= scale;
			return lhs;
		}
		
		template<typename T>
		inline const Vector4<T> operator/=(Vector4<T>& lhs, const T scale)
		{
			assert(scale != 0 && "Can't divide by Zero");
			lhs.x /= scale;
			lhs.y /= scale;
			lhs.z /= scale;
			lhs.w /= scale;
			return lhs;
		}
		
		// Utilities
		template <typename T>
		const T Length(const Vector4<T>& vec)
		{
			return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
		};

		template <typename T>
		const T Length2(const Vector4<T>& vec)
		{
			return (Length(vec) * Length(vec));
		};

		template <typename T>
		void Normalize(Vector4<T>& vec)
		{
			vec /= Length(vec);
		}

		template <typename T>
		const Vector4<T> GetNormalized(const Vector4<T>& vec)
		{
			return vec / Length(vec);
		}

		template <typename T>
		T Dot(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return ( (lhs.x*rhs.x) + (lhs.y*rhs.y) + (lhs.z*rhs.z) + (lhs.w*rhs.w) );
		}

		template<typename T>
		const Vector4<T> Cross(const Vector4<T>& lhs, const Vector4<T>& rhs)
		{
			return { ( lhs.y * rhs.z ) - ( lhs.z * rhs.y ), ( lhs.z * rhs.x ) - ( lhs.x * rhs.z ), ( lhs.x * rhs.y ) - ( lhs.y * rhs.x ), 1 };
		
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
			if (first.x >= scalar)
				return false;
			if (first.y >= scalar)
				return false;
			if (first.z >= scalar)
				return false;
			if (first.w >= scalar)
				return false;

			return true;
		}

		template<typename T>
		bool operator<=(const Vector4<T>& first, float scalar)
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

		template<typename T>
		bool operator>(const Vector4<T>& first, float scalar)
		{
			if (first.x <= scalar)
				return false;
			if (first.y <= scalar)
				return false;
			if (first.z <= scalar)
				return false;
			if (first.w <= scalar)
				return false;
			return true;
		}

		template<typename T>
		bool operator>=(const Vector4<T>& first, float scalar)
		{
			if (first.x < scalar)
				return false;
			if (first.y < scalar)
				return false;
			if (first.z < scalar)
				return false;
			if (first.w < scalar)
				return false;
			return true;
		}




	};
};
namespace CU = CommonUtilities;
