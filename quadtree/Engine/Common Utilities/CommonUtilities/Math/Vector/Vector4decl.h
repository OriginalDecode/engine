#pragma once
#include <assert.h>
#include <math.h>
#include "Vector.h"

namespace CommonUtilities
{
	namespace Math
	{
		template<typename T>
		class Vector4
		{
		public:
			Vector4(T anX = 0, T anY = 0, T anZ = 0, T anW = 0);
			Vector4(const Vector2<T>& aVector, T anZ = 0, T anW = 0);
			Vector4(T anX, T anY, const Vector2<T>& aVector);
			Vector4(const Vector2<T>& aVector, const Vector2<T>& aSecondVector);
			Vector4(const Vector3<T>& aVector, T anW = 0);
			~Vector4();
			union
			{
				T X;
				T x;
				T myX;
				T r;
				T myR;
			};
			union
			{
				T Y;
				T y;
				T myY;
				T g;
				T myG;
			};
			union
			{
				T Z;
				T z;
				T myZ;
				T b;
				T myB;
			};
			union
			{
				T W;
				T w;
				T myW;
				T a;
				T myA;
			};
		};
	};
}
namespace CU = CommonUtilities;