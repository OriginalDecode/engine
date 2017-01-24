#include "AABB.h"


AABB::AABB()
{
}


AABB::~AABB()
{
}


void AABB::SetSize(const Vector3f& aSize)
{
	mySize = aSize;
}

void AABB::SetMinPosition(const Vector3f& aMinPosition)
{
	myMinPosition = aMinPosition;
}

void AABB::SetMaxPosition(const Vector3f& aMaxPosition)
{
	myMaxPosition = aMaxPosition;
}

void AABB::SetCenterPosition(const Vector3f& aPosition)
{
	myCenterPosition = aPosition;
}



namespace CommonUtilities
{
	bool PointInsideAABB(AABB aAABB, Vector3f aPoint)
	{
		if (aPoint.x > aAABB.GetMaxPosition().x) return false;
		if (aPoint.x < aAABB.GetMinPosition().x) return false;
		if (aPoint.y > aAABB.GetMaxPosition().y) return false;
		if (aPoint.y < aAABB.GetMinPosition().y) return false;
		if (aPoint.z > aAABB.GetMaxPosition().z) return false;
		if (aPoint.z < aAABB.GetMinPosition().z) return false;

		return true;
	}

	bool PointInsideAABB(AABB aAABB,Vector3f aPoint, float aRadius)
	{
		float boxLeft = aAABB.GetCenterPosition().x - aAABB.GetSize().x;
		float boxRight = aAABB.GetCenterPosition().x + aAABB.GetSize().x;
		float boxTop = aAABB.GetCenterPosition().y - aAABB.GetSize().y;
		float boxBot = aAABB.GetCenterPosition().y + aAABB.GetSize().y;


		float circleLeft = aPoint.x - aRadius;
		float circleRight = aPoint.x + aRadius;
		float circleTop = aPoint.y - aRadius;
		float circleBot = aPoint.y + aRadius;

		if ((circleLeft > boxLeft) && (circleRight < boxRight) && (circleTop > boxTop) && (circleBot < boxBot))
			return true;

		return false;

	}
}