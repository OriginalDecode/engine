#pragma once
#include "../CommonLib/Math/Matrix/Matrix44.h"
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;
class btVector3;
class btTransform;
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
#ifdef _DEBUG
	CRigidBody(Snowblind::CSynchronizer* aSychronizer);
#endif
	~CRigidBody();

	btRigidBody* InitAsPlane(const btVector3& aNormal);
	btRigidBody* InitAsCube();
	btRigidBody* InitAsSphere(const CU::Vector3f& aPosition);

	btRigidBody* GetBody();
	const CU::Matrix44f& GetOrientation();
	const CU::Vector4f& GetAABBPoint(int index);

private:
	void CalculatePoints();
#ifdef _DEBUG
	Snowblind::CSynchronizer* mySynchronizer;
#endif

	btRigidBody* myBody = nullptr;
	btCollisionShape* myShape = nullptr;
	btDefaultMotionState* myMotionState = nullptr;
	btTransform* myWorldTranslation = nullptr;
	CU::Matrix44f myOrientation;
	CU::Vector3f myPoints[8];


};

