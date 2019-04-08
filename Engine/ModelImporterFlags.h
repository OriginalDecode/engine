#pragma once
#include <Engine/engine_shared.h>

namespace ModelImportUtil
{
	enum eLoadOptions
	{
		FLIP_VERTEX = BIT(1),
		FLIP_NORMAL = BIT(2),
		FLIP_BINORM = BIT(3),
		FLIP_TANGENT = BIT(4),
		IGNORE_FILL = BIT(5),
	};

};