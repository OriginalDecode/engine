#include "Circle.h"

Circle::Circle()
{

}

Circle::~Circle()
{

}


void Circle::SetCenterPosition(const Vector2f& aPosition)
{
	myCenterPosition = aPosition;
}

void Circle::SetRadius(const float& aRadius)
{
	myRadius = aRadius;
	myRadiusSquared = aRadius*aRadius;
}

const float& Circle::GetRadius() const
{
	return myRadius;
}

const float& Circle::GetRadiusSquared() const
{
	return myRadiusSquared;
}

const Vector2f& Circle::GetCenterPosition() const
{
	return myCenterPosition;
}


namespace CommonUtilities
{
	bool LineVsCircle(Intersection::LineSegment2D aLine, Circle aCircle, Vector2f anIntersectionPoint)
	{
		Vector2f d = aLine.myEndPos - aLine.myStartPos;
		CU::Math::Normalize(d);
		Vector2f e = aCircle.GetCenterPosition() - aLine.myStartPos;
		float a = CU::Math::Dot(e, d);

		float fSquared = aCircle.GetRadiusSquared() - CU::Math::Dot(e, e) + a * a;
		if (fSquared < 0)
		{
			return false;
		}

		float lSqrd = CU::Math::Length(aLine.myStartPos - aLine.myEndPos);
		if (a > lSqrd + aCircle.GetRadius())
		{
			return false;
		}

		if (a < -aCircle.GetRadius())
		{
			return false;
		}

		float t = a - sqrt(fSquared);
		anIntersectionPoint = aLine.myStartPos + d * t;

		return true;
	}
	bool CircleVsCircle(Circle aFirstCircle, Circle aSecondCircle)
	{
		Vector2f distanceVector = aFirstCircle.GetCenterPosition() - aSecondCircle.GetCenterPosition();
		float distance = Length2(distanceVector);

		if (distance > aFirstCircle.GetRadiusSquared() + aSecondCircle.GetRadiusSquared())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

}
