#include "Intersection.h"
#include "..\Line\Line.h"

#include <cmath>

namespace CU = CommonUtilities;

namespace Intersection
{
	bool PointInsideAABB(AABB aAABB, Vector3f aVector3)
	{
		if (aVector3.x < aAABB.myMinPos.x || aVector3.x > aAABB.myMaxPos.x)
		{
			return false;
		}
		if (aVector3.y < aAABB.myMinPos.y || aVector3.y > aAABB.myMaxPos.y)
		{
			return false;
		}
		if (aVector3.z < aAABB.myMinPos.z || aVector3.z > aAABB.myMaxPos.z)
		{
			return false;
		}

		return true;
	}

	bool PointInsideSphere(Sphere aSphere, Vector3f aVector3)
	{
		Vector3f distance = aSphere.myCenterPosition - aVector3;

		if (CommonUtilities::Math::Length2(distance) < aSphere.myRadiusSquared)
		{
			return true;
		}

		return false;
	}

	bool SphereVsFrustrum(Sphere aSphere, Fov90Frustrum aFrust)
	{
		return aFrust.myPlanes.Inside(aSphere);
	}

	bool LineVsLine(LineSegment2D aLine1, LineSegment2D aLine2, Vector2f& aIntersectionPoint)
	{
		CU::Line<float> line1(aLine1.myStartPos, aLine1.myEndPos);
		CU::Line<float> line2(aLine2.myStartPos, aLine2.myEndPos);

		
		float numX = line2.myData.y * line1.myData.z - line1.myData.y * line2.myData.z;
		float denX = line1.myData.x * line2.myData.y - line2.myData.x * line1.myData.y;
		
		float numY = line1.myData.x * line2.myData.z - line2.myData.x * line1.myData.z;
		float denY = line1.myData.x * line2.myData.y - line2.myData.x * line1.myData.y;

		if (denX == 0 && denY == 0)
		{
			if (line1.myData.z != line2.myData.z)
			{
				return false;
			}

			Vector2f dir1 = aLine1.myEndPos - aLine1.myStartPos;
			Vector2f dir2 = aLine2.myEndPos - aLine2.myStartPos;

			float lengthDir1 = CommonUtilities::Math::Length(dir1);
			dir1 /= lengthDir1;

			float lengthDir2 = CommonUtilities::Math::Length(dir2);
			dir2 /= lengthDir2;

			if (CU::Math::Dot(dir1, aLine2.myEndPos - aLine1.myStartPos) < lengthDir1 &&
				CU::Math::Dot(dir1, aLine2.myEndPos - aLine1.myStartPos) > 0)
			{
				aIntersectionPoint = aLine2.myEndPos;
				return true;
			}

			if (CU::Math::Dot(dir1, aLine2.myStartPos - aLine1.myStartPos) < lengthDir1 &&
				CU::Math::Dot(dir1, aLine2.myStartPos - aLine1.myStartPos) > 0)
			{
				aIntersectionPoint = aLine2.myStartPos;
				return true;
			}

			if (CU::Math::Dot(dir2, aLine1.myEndPos - aLine2.myStartPos) < lengthDir2 &&
				CU::Math::Dot(dir2, aLine1.myEndPos - aLine2.myStartPos) > 0)
			{
				aIntersectionPoint = aLine1.myEndPos;
				return true;
			}

			if (CU::Math::Dot(dir2, aLine1.myStartPos - aLine2.myStartPos) < lengthDir2 &&
				CU::Math::Dot(dir2, aLine1.myStartPos - aLine2.myStartPos) > 0)
			{
				aIntersectionPoint = aLine1.myStartPos;
				return true;
			}

			return false;
		}

		aIntersectionPoint = Vector2f(numX / denX, numY / denY);


		Vector2f v1(aLine1.myEndPos - aLine1.myStartPos);

		Vector2f v1intersect(aIntersectionPoint - aLine1.myStartPos);

		if (CU::Math::Dot(v1, v1intersect) < 0 || CU::Math::Length2(v1intersect) > CU::Math::Length2(v1))
		{
			return false;
		}

		Vector2f v2(aLine2.myEndPos - aLine2.myStartPos);

		Vector2f v2intersect(aIntersectionPoint - aLine2.myStartPos);

		if (CU::Math::Dot(v2, v2intersect) < 0 || CU::Math::Length2(v2intersect) > CU::Math::Length2(v2))
		{
			return false;
		}

		return true;
	}

	bool SwepthSphereVsSphere(LineSegment3D aLine, float aRadius, Sphere aSphere)
	{
		aSphere.myRadius += aRadius;
		aSphere.myRadiusSquared = aSphere.myRadius * aSphere.myRadius;

		return LineVsSphere(aLine, aSphere, Vector3f());
	}

	bool LineVsAABB(LineSegment3D aLine, AABB aAABB, Vector3f aVector3)
	{
		Vector3f rayDirection = aLine.myEndPos - aLine.myStartPos;
		CU::Math::Normalize(rayDirection);

		float distance = CU::Math::Dot(aAABB.myCenterPos - aLine.myStartPos, rayDirection);

		if (distance < 0)
		{
			if (PointInsideAABB(aAABB, aLine.myStartPos) == false)
			{
				return false;
			}
		}
		else if (PointInsideAABB(aAABB, aLine.myStartPos + rayDirection * distance) == false)
		{
			return false;
		}
		else
		{
			float f = sqrt(CU::Math::Dot(aAABB.myExtents, aAABB.myExtents) - (CU::Math::Dot(aAABB.myCenterPos - aLine.myStartPos, aAABB.myCenterPos - aLine.myStartPos)) + pow(distance, 2));
			float t = distance - f;

			aVector3 = rayDirection * t;
		}

		rayDirection = aLine.myStartPos - aLine.myEndPos;
		CU::Math::Normalize(rayDirection);

		distance = CU::Math::Dot(aAABB.myCenterPos - aLine.myEndPos, rayDirection);

		if (distance < 0)
		{
			if (PointInsideAABB(aAABB, aLine.myEndPos) == false)
			{
				return false;
			}
		}
		else if (PointInsideAABB(aAABB, aLine.myEndPos + rayDirection * distance) == false)
		{
			return false;
		}
		else
		{
			float f = sqrt(CU::Math::Dot(aAABB.myExtents, aAABB.myExtents) - (CU::Math::Dot(aAABB.myCenterPos - aLine.myEndPos, aAABB.myCenterPos - aLine.myEndPos)) + pow(distance, 2));
			float t = distance - f;

			aVector3 = rayDirection * t;
		}

		return true;
	}

	bool SwepthSphereVsAABB(LineSegment3D aLine, float aRadius, AABB aAABB)
	{
		aAABB.myMaxPos += aRadius;
		aAABB.myMinPos -= aRadius;

		return LineVsAABB(aLine, aAABB, Vector3f());
	}

	bool LineVsSphere(LineSegment3D aLine, Sphere aSphere, Vector3f aVector3)
	{
		Vector3f rayDirection = aLine.myEndPos - aLine.myStartPos;
		CU::Math::Normalize(rayDirection);

		float distance = CU::Math::Dot(aSphere.myCenterPosition - aLine.myStartPos, rayDirection);

		if (distance < 0)
		{
			if (PointInsideSphere(aSphere, aLine.myStartPos) == false)
			{
				return false;
			}
		}
		else if (PointInsideSphere(aSphere, aLine.myStartPos + rayDirection * distance) == false)
		{
			return false;
		}
		else
		{
			float f = sqrt(pow(aSphere.myRadius, 2) - (CU::Math::Dot(aSphere.myCenterPosition - aLine.myStartPos, aSphere.myCenterPosition - aLine.myStartPos)) + pow(distance, 2));
			float t = distance - f;

			aVector3 = rayDirection * t;
		}

		rayDirection = aLine.myStartPos - aLine.myEndPos;
		CU::Math::Normalize(rayDirection);

		distance = CU::Math::Dot(aSphere.myCenterPosition - aLine.myEndPos, rayDirection);

		if (distance < 0)
		{
			if (PointInsideSphere(aSphere, aLine.myEndPos) == false)
			{
				return false;
			}
		}
		else if (PointInsideSphere(aSphere, aLine.myEndPos + rayDirection * distance) == false)
		{
			return false;
		}
		else
		{
			float f = sqrt(pow(aSphere.myRadius, 2) - (CU::Math::Dot(aSphere.myCenterPosition - aLine.myEndPos, aSphere.myCenterPosition - aLine.myEndPos)) + pow(distance, 2));
			float t = distance - f;

			aVector3 = rayDirection * t;
		}

		return true;
	}
}

const Vector2f GetNormal(const Intersection::LineSegment2D& aLineSegment)
{
	CU::Line<float> line(aLineSegment.myStartPos, aLineSegment.myEndPos);

	return Vector2f(line.myData.x, line.myData.y);
}