#pragma once
#include "..\Plane\PlaneVolume.h"

namespace Intersection
{
	class Fov90Frustrum
	{
	public:
		Fov90Frustrum(float aNear, float aFar);
		~Fov90Frustrum();

		CommonUtilities::Math::PlaneVolume<float> myPlanes;
	};
}