#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
#include "../CommonLib/DataStructures/GrowingArray.h"
#include "../Engine/VertexStructs.h"
#include <standard_datatype.hpp>

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
	btRigidBody* InitAsCube();
	btRigidBody* InitAsTerrain(CU::GrowingArray<float> vertices, CU::GrowingArray<s32> indices);
	btRigidBody* InitAsSphere(const CU::Vector3f& aPosition);

	btRigidBody* GetBody();
	const CU::Matrix44f& GetOrientation();
	void Impulse(const CU::Vector3f& anImpulseVector);
private:
	btRigidBody* myBody = nullptr;
	btCollisionShape* myShape = nullptr;
	btDefaultMotionState* myMotionState = nullptr;
	btTransform* myWorldTranslation = nullptr;
	btTriangleIndexVertexArray* myVertexArray = nullptr;
	CU::Matrix44f myOrientation;
};

