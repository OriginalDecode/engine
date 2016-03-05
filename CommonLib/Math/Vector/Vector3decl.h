#pragma once
#include <assert.h>
#include "Vector.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<typename T>
		class Vector3
		{
		public:
			
			Vector3(T anX = 0, T anY = 0, T anZ = 0);
			Vector3(const Vector2<T>& aVector, T aZ = 0);

			~Vector3();
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
		};
	};
};
namespace CU = CommonUtilities;