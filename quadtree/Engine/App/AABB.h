#pragma once
#include "Misc\Global.h"
class AABB;
namespace CommonUtilities
{
	bool PointInsideAABB(AABB aAABB, Vector3f& aPoint);
	bool PointInsideAABB(AABB aAABB, Vector3f& aPoint, float aRadius);
};

class AABB
{
public:
	AABB();
	~AABB();

	void SetCenterPosition(const Vector3f& aPosition);
	void SetSize(const Vector3f& aSize);
	void SetMinPosition(const Vector3f& aMinPosition);
	void SetMaxPosition(const Vector3f& aMaxPosition);

	inline const Vector3f& GetCenterPosition() const;
	inline const Vector3f& GetSize() const;
	inline const Vector3f& GetMaxPosition() const;
	inline const Vector3f& GetMinPosition() const;



private:

	Vector3f mySize;
	Vector3f myMinPosition;
	Vector3f myMaxPosition;
	Vector3f myCenterPosition; 

};

inline const Vector3f& AABB::GetCenterPosition() const
{
	return myCenterPosition;
}
inline const Vector3f& AABB::GetSize() const
{
	return mySize;
}
inline const Vector3f& AABB::GetMaxPosition() const
{
	return myMaxPosition;
}
inline const Vector3f& AABB::GetMinPosition() const
{
	return myMinPosition;
}

namespace CU = CommonUtilities;