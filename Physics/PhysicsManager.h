#pragma once
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/Math/Vector/Vector.h"
#include "../Engine/snowblind_shared.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;

class CRigidBody;

class CPhysicsManager
{
public:
	CPhysicsManager();
	~CPhysicsManager();
	void Add(btRigidBody* aBody);
	void Remove(btRigidBody* aBody);
	CRigidBody* CreateBody();
	float GetGravityForce();
	void Update(double& additionalTime);
	CU::Vector3f RayCast(const CU::Vector3f& cameraPosition, const CU::Vector3f& target) const;


private:
	Ticket_Mutex m_Mutex;

	btDefaultCollisionConfiguration* myDefaultCollisionConfiguration = nullptr;
	btCollisionDispatcher* myCollisionDispatcher = nullptr;
	btBroadphaseInterface* myBroadphaseInterface = nullptr;
	btSequentialImpulseConstraintSolver* myImpulseSolver = nullptr;
	btDiscreteDynamicsWorld* myDynamicsWorld = nullptr;

	CRigidBody* myZeroPlane;
	CU::GrowingArray<btRigidBody*> myBodies;

	float myGravity = 0.f;

};

