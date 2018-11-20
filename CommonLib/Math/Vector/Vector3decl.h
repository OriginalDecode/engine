#pragma once
#include <assert.h>
#include "Vector.h"
namespace CommonUtilities
{
	namespace Math
	{
		template<typename T>
		//The alignment will change the size of the vector to 16 from 12, breaking the shaders. Shoul be aligned tho.
		class /*__declspec(align(16))*/ Vector3
		{
		public:
			
			Vector3(T X = 0, T Y = 0, T Z = 0)
				: x(X)
				, y(Y)
				, z(Z)
			{
			}

			Vector3(const Vector2<T>& vec, T Z = 0)
				: x(vec.x)
				, y(vec.y)
				, z(Z)
			{
			}

			Vector3(const Vector3<T>& vector)
				: x(vector.x)
				, y(vector.y)
				, z(vector.z)
			{
			}

			Vector3(T in[3])
				: x(in[0])
				, y(in[1])
				, z(in[2])
			{
			}

			~Vector3() { }

			union
			{
				struct 
				{
					T x;
					T y;
					T z;
				};
				T vector[3];
			};
		} ;
	};
};
namespace CU = CommonUtilities;
