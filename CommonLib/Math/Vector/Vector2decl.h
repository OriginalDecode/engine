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
			TYPE x;
			TYPE y;

		};

	};
};
namespace CU = CommonUtilities;
