#pragma once
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../CommonLib/Math/Vector/Vector.h"
#include "../Engine/engine_shared.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;
class btCollisionShape;
class btGhostObject;
class btCollisionObject;
struct btDefaultMotionState;

class RigidBody;

//This should be moved to a different location, this makes 0 sense that the manager holds it.
namespace scientific_constants
{
	namespace pressure
	{
		static constexpr float  air_pressure = 1.293f;
	};
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Add(btRigidBody* aBody);
	void Add(btGhostObject* ghost_object);
	void Add(btCollisionObject* collision_object);

	void Remove(btRigidBody* aBody);
	void Remove(btCollisionObject* collision_object);

	RigidBody* CreateBody();
	float GetGravityForce();
	void Update();
	CU::Vector3f RayCast(const CU::Vector3f& cameraPosition, const CU::Vector3f& target) const;


private:
	Ticket_Mutex m_Mutex;

	btDefaultCollisionConfiguration* myDefaultCollisionConfiguration	= nullptr;
	btCollisionDispatcher* myCollisionDispatcher						= nullptr;
	btBroadphaseInterface* myBroadphaseInterface						= nullptr;
	btSequentialImpulseConstraintSolver* myImpulseSolver				= nullptr;
	btDiscreteDynamicsWorld* myDynamicsWorld							= nullptr;

	RigidBody* myZeroPlane;
	CU::GrowingArray<btCollisionObject*> myBodies;

	float m_Gravity = 0.f;
};

