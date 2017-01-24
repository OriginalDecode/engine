#include "Fov90Frustrum.h"

namespace Intersection
{
	Fov90Frustrum::Fov90Frustrum(float aNear, float aFar) : myPlanes(std::vector<CU::Math::Plane<float>>())
	{
		myPlanes.AddPlane(CU::Math::Plane<float>({ 0, 0, aFar }, { 0, 0, 1 }));
		myPlanes.AddPlane(CU::Math::Plane<float>({ 0, 0, aNear }, { 0, 0, -1 }));

		Vector4f normal(1, 0, 0, 1);
		normal = normal * Matrix44f::CreateRotateAroundY(0.785398163f);
		Vector3f n(normal.x, normal.y, normal.z);
		Vector3f p(0, 0, 0);

		myPlanes.AddPlane(CU::Math::Plane<float>(p, n));

		normal = { -1, 0, 0, 1 };
		normal = normal * Matrix44f::CreateRotateAroundY(-0.785398163f);
		n = { normal.x, normal.y, normal.z };
		myPlanes.AddPlane(CU::Math::Plane<float>(p, n));

		normal = { 0, 1, 0, 1 };
		normal = normal * Matrix44f::CreateRotateAroundX(-0.785398163f);
		n = { normal.x, normal.y, normal.z };
		myPlanes.AddPlane(CU::Math::Plane<float>(p, n));

		normal = { 0, -1, 0, 1 };
		normal = normal * Matrix44f::CreateRotateAroundX(0.785398163f); 
		n = { normal.x, normal.y, normal.z };
		myPlanes.AddPlane(CU::Math::Plane<float>(p, n));
	}

	Fov90Frustrum::~Fov90Frustrum()
	{
	}
}