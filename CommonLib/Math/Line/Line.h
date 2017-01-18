#pragma once
#include "../Vector/Vector.h"

namespace CommonUtilities
{
	class Line
	{
	public:
		Line(Vector3f  aFirstPoint, Vector3f  aSecondPoint);

		void InitWith2Points(Vector3f aFirstPoint, Vector3f aSecondPoint);
		void InitWithPointAndDirection(Vector3f aPoint, Vector3f aDirection);

		bool Inside(Vector3f aPosition);

		Vector3f GetNormal() const;
		Vector3f  GetPoint() const;

	private:

		Vector3f myOld;
		Vector3f myPoint;
		Vector3f myNorm;
	};

	Line::Line(Vector3f aFirstPoint, Vector3f aSecondPoint)
	{
		Vector3f result = 
			( (aSecondPoint.x - aFirstPoint.x)
			, (aSecondPoint.y - aFirstPoint.y)
			, (aSecondPoint.z - aFirstPoint.z) );
		Normalize(result);

		myOld.x = result.x;

		result.x = (-aFirstPoint.y);
		result.y = myOld.x;
		
	}
	
	void Line::InitWith2Points(Vector3f aFirstPoint, Vector3f aSecondPoint)
	{
		CommonUtilities::Line(aFirstPoint, aSecondPoint);
	}

	void Line::InitWithPointAndDirection(Vector3f aPoint, Vector3f aDirection)
	{
		myPoint = aPoint;
		myNorm = GetNormalized(aDirection);

		myOld.x = myNorm.x;

		myNorm.x = (-myNorm.y);
		myNorm.y = myOld.x;
	}

	bool Line::Inside(Vector3f aPosition)
	{
		if (Dot(aPosition - myPoint, myNorm) > 0)
			return false;
		else
			return true;
	}
	
	Vector3f Line::GetPoint() const
	{
		return myPoint;
	}

	Vector3f Line::GetNormal() const
	{
		return myNorm;
	}
};