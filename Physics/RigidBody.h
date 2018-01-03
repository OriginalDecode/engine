#pragma once
#include "CollisionObject.h"
#include "CommonLib/Math/Quaternion/Quaternion.h"
#include "CommonLib/DataStructures/GrowingArray.h"
#include "Engine/VertexStructs.h"
#include <vector>
class btVector3;
class btRigidBody;
class btTransform;
class btTriangleIndexVertexArray;


//struct ControllerState;

class RigidBody : public CollisionObject
{
public:
	RigidBody();
	~RigidBody() override;

	btRigidBody* InitAsPlane(const btVector3& aNormal);
	btRigidBody* InitAsTerrain(std::vector<float> vertices, std::vector<s32> indices);
	btRigidBody* InitAsSphere(float aRadius, float aMass, float aGravityForce, float anInitialResistance, const CU::Vector3f& aPosition);
	btRigidBody* InitWithMeshCollision(s8* const vertices, s8* const indices, const s32 const idx_count, const s32 const vtx_count);
	btRigidBody* InitAsBox(float width, float height, float depth, CU::Vector3f position);

	void SetResistanceDensity(float aDensity);
	void SetPosition(const CU::Vector3f& aPosition);

	void Update(float deltaTime);
	btRigidBody* GetBody();
	const CU::Matrix44f& GetOrientation();
	void Impulse(const CU::Vector3f& anImpulseVector);
	CU::Vector3f GetLinearVelocity();
	//void UpdateOrientation(const ControllerState& controller_state);
	float GetMass() { return myMass; }
	float GetGravity() { return myGravity; }
	bool IsEnabled() { return m_IsEnabled; }
	void ToggleBody() { m_IsEnabled = !m_IsEnabled; }
	btCollisionShape* GetShape() const { return m_Shape; }

	void SetStatic(bool is_static);
	bool IsStatic() const { return m_IsStatic; }

	template<typename Writer>
	void Serialize(Writer& writer);

	template<typename Value>
	void Deserialize(const Value& value);

	void SerializePhysicsData(unsigned char*& buffer_pointer, int& buffer_size);
private:

	CU::Vector3f myVelocity;
	CU::Vector3f myTerminalVelocity;

	float myMass = 0.f;
	float myGravity = 0.f;

	float myDragCoeff = 0.f;
	float myCrossSectionArea = 0.f;
	float myResistanceDensity = 0.f;
	float myRadius = 0.f;

	bool m_IsEnabled : 1;
	bool m_IsStatic : 1;

	CU::Quaternion m_Yaw;
	CU::Quaternion m_Pitch;
	CU::Vector2f m_CenterPoint;

	btRigidBody* myBody = nullptr;
	btTransform* myWorldTranslation = nullptr;
	btTriangleIndexVertexArray* myVertexArray = nullptr;


};

template<typename Writer>
void RigidBody::Serialize(Writer& writer)
{

	writer.String("mass");
	writer.Double(myMass);
	writer.String("static");
	writer.Bool(m_IsStatic);


	


	writer.EndObject();
}

template<typename Value>
void RigidBody::Deserialize(const Value& value)
{
	myMass = (float)value["mass"].GetDouble();
	m_IsStatic = value["static"].GetBool();
}

