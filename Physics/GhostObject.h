#pragma once
#include "CollisionObject.h"
class btGhostObject;

class GhostObject : public CollisionObject
{
public:
	GhostObject() = default;
	~GhostObject() override;
	btGhostObject* InitAsBox(float width, float height, float depth, CU::Vector3f position);
	btGhostObject* InitAsBox(CU::Vector3f size, CU::Vector3f position);
	btGhostObject* InitAsSphere(float radius, CU::Vector3f position);

	void SetPosition(const CU::Vector3f& position);
	const CU::Matrix44f& GetOrientation() const;

private:
	btGhostObject* m_GhostObject = nullptr;
};

