#pragma once
#include <assert.h>
#include "Vector.h"

namespace CommonUtilities
{
	namespace Math
	{
		template<typename T>
		class Vector4
		{
		public:
			Vector4(const T& anX = 0, const T& anY = 0, const T& anZ = 0, const T& anW = 0);
			Vector4(const Vector2<T>& aVector, T anZ = 0, T anW = 0);
			Vector4(T anX, T anY, const Vector2<T>& aVector);
			Vector4(const Vector2<T>& aVector, const Vector2<T>& aSecondVector);
			Vector4(const Vector3<T>& aVector, T anW = 0);
			~Vector4();
			T x;
			T y;
			T z;
			T w;
		};
	};
}
namespace CU = CommonUtilities;