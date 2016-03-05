#pragma once
#include <assert.h>
#include "Vector.h"

namespace CommonUtilities
{
	namespace Math
	{
		template<class TYPE>
		class Vector2
		{
		public:
			Vector2(TYPE anX = 0, TYPE anY = 0);
			~Vector2();

			union
			{
				TYPE X;
				TYPE x;
				TYPE myX;
				TYPE u;
				TYPE myU;
			};
			union
			{
				TYPE Y;
				TYPE y;
				TYPE myY;
				TYPE v;
				TYPE myV;
			};

		};

	};
};
namespace CU = CommonUtilities;
