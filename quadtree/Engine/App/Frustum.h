#pragma once
#include "Renderer.h"
#include "AABB.h"
#include "Math\Line\LineVolume.h"
class Synchronizer;
class TreeDweller;

class Frustum
{
public:
	Frustum();
	~Frustum();

	void Initiate();

	const bool IsInside(TreeDweller* anObject) const;
	void ViewDirection(const Vector2f& aPosition);
	void MoveTo(const Vector2f& aPosition);

	inline CU::LineVolume<float> GetFrustum();
	inline CU::LineVolume<float> GetLineBox();
	inline AABB& GetAABB();
	void Render(Synchronizer& aSynchronizer);
private:


	void Update();
	void UpdateAABB();

	float myAngle;
	float myFarPlane;
	AABB myBoundingBox;
	CU::LineVolume<float> myFrustum;
	CU::LineVolume<float> myLineBox;
	Vector2f myPosition;
	float myFoV;


};

CU::LineVolume<float> Frustum::GetFrustum()
{
	return myFrustum;
}

CU::LineVolume<float> Frustum::GetLineBox()
{
	return myLineBox;
}

AABB& Frustum::GetAABB()
{
	return myBoundingBox;
}