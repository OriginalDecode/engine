#pragma once
#include "..\..\Misc\Global.h"
#include "AABB.h"
#include "Sphere.h"
#include "Fov90Frustrum.h"
#include "..\Line\LineSegment3D.h"
#include "..\Line\LineSegment2D.h"

namespace Intersection
{
	bool PointInsideAABB(AABB aAABB, Vector3f aVector3);
	
	bool PointInsideSphere(Sphere aSphere, Vector3f aVector3);

	bool SphereVsFrustrum(Sphere aSphere, Fov90Frustrum aFrust);
	
	bool LineVsLine(LineSegment2D aLine, LineSegment2D aSecondLine, Vector2f& aIntersectionPoint);

	bool LineVsSphere(LineSegment3D aLine, Sphere aSphere, Vector3f  aVector3);

	bool SwepthSphereVsSphere(LineSegment3D aLine, float aRadius, Sphere aSphere);

	bool LineVsAABB(LineSegment3D aLine, AABB aAABB, Vector3f  aVector3);

	bool SwepthSphereVsAABB(LineSegment3D aLine, float aRadius, AABB aAABB);
}

const Vector2f GetNormal(const Intersection::LineSegment2D& aLineSegment);