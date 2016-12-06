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




class RigidBody;


static constexpr float  air_preassure = 1.293f;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	void Add(btRigidBody* aBody);
	void Remove(btRigidBody* aBody);
	RigidBody* CreateBody();
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

	RigidBody* myZeroPlane;
	CU::GrowingArray<btRigidBody*> myBodies;

	float myGravity = 0.f;

};

