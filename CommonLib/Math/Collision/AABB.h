#pragma once
#include "..\..\Misc\Global.h"
namespace Intersection
{
	struct AABB
	{
		Vector3f myCenterPos;
		Vector3f myExtents;
		Vector3f myMinPos;
		Vector3f myMaxPos;
	};
}