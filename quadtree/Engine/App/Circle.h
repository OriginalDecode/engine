#pragma once
#include "Misc\Global.h"
#include "Math\Line\LineSegment2D.h"



class Circle
{
public:

	Circle();
	~Circle();


	void SetCenterPosition(const Vector2f& aPosition);
	void SetRadius(const float& aRadius);

	const float& GetRadius() const;
	const float& GetRadiusSquared() const;
	const Vector2f& GetCenterPosition() const;



private:

	Vector2f myCenterPosition;
	float myRadius;
	float myRadiusSquared;
};


namespace CommonUtilities
{
	bool LineVsCircle(Intersection::LineSegment2D aLine, Circle aCircle, Vector2f anIntersectionPoint);
	bool CircleVsCircle(Circle aFirstCircle, Circle aSecondCircle);
}
