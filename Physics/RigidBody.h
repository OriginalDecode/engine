#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class btVector3;
class CRigidBody
{
public:
	CRigidBody();
	~CRigidBody();

	btRigidBody* InitAsPlane(const btVector3& aNormal);
	btRigidBody* InitAsCube();
	btRigidBody* InitAsSphere();

	btRigidBody* GetBody();
	const CU::Matrix44f& GetPosition();

private:

	btRigidBody* myBody = nullptr;
	btCollisionShape* myShape = nullptr;
	btDefaultMotionState* myMotionState = nullptr;

	CU::Matrix44f* myOrientation;


};

