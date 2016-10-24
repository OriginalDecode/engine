#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../Engine/VertexStructs.h"
#include <standard_datatype.hpp>
#include <vector>
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class btVector3;
class btTransform;
class btTriangleIndexVertexArray;
enum class eBodyType
{
	eSTATIC,
	eDYNAMIC,
	
};


namespace Snowblind
{
	class CSynchronizer;
}

class CRigidBody
{
public:
	CRigidBody();
	~CRigidBody();

	btRigidBody* InitAsPlane(const btVector3& aNormal);
	btRigidBody* InitAsTerrain(std::vector<float> vertices, std::vector<s32> indices);
	btRigidBody* InitAsSphere(float aRadius, float aMass, float aGravityForce, float anInitialResistance, const CU::Vector3f& aPosition);
	/*
		Air Resistance, Water Resistance, Sludge etc...
		Air is 1.293
	*/
	void SetResistanceDensity(float aDensity);
	void SetPosition(const CU::Vector3f& aPosition);

	void Update(float deltaTime);
	btRigidBody* GetBody();
	const CU::Matrix44f& GetOrientation();
	void Impulse(const CU::Vector3f& anImpulseVector);
	CU::Vector3f GetLinearVelocity();
private:
	CU::Vector3f myVelocity; //Only downwards right now.
	CU::Vector3f myTerminalVelocity;

	float myMass = 0.f;
	float myDragCoeff = 0.f;
	float myGravity = 0.f;
	float myCrossSectionArea = 0.f;
	float myResistanceDensity = 0.f;
	float myRadius = 0.f;
	

	btRigidBody* myBody = nullptr;
	btCollisionShape* myShape = nullptr;
	btDefaultMotionState* myMotionState = nullptr;
	btTransform* myWorldTranslation = nullptr;
	btTriangleIndexVertexArray* myVertexArray = nullptr;
	CU::Matrix44f myOrientation;

};

