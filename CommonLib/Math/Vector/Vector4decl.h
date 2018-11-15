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
			Vector4(T X = 0, T Y = 0, T Z = 0, T W = 0)
				: x(X)
				, y(Y)
				, z(Z)
				, w(W)
			{
			}
			
			Vector4(const Vector3<T>& vec, T W)
				: x(vec.x)
				, y(vec.y)
				, z(vec.z)
				, w(W)
			{
			}

			Vector4(const Vector3<T>& vec)
				: x(vec.x)
				, y(vec.y)
				, z(vec.z)
			{
			}

			~Vector4() { }


			T x = 0;
			T y = 0;
			T z = 0;
			T w = 0;

		};
	};
}
namespace CU = CommonUtilities;