#pragma once
#include "BaseSystem.h"
#include <Subscriber.h>
#include <Math/Vector/Vector.h>
#include <Math/Matrix/Matrix.h>
struct OnLeftClick;
struct DebugComponent;

class Synchronizer;

class DebugSystem : public BaseSystem, public Subscriber
{
public:
	DebugSystem(EntityManager& entity_manager);
	~DebugSystem();
	void Update(float dt) override;
private:
	void ReceiveMessage(const OnLeftClick& message) override;
	Synchronizer* m_Synchronizer = nullptr;
	double m_AccumulatedTime = 0.0f;
	void RenderBox(const DebugComponent& component, const CU::Matrix44f& orientation);
	struct entity_collisions
	{
		Entity m_ID;
		CU::Vector3f m_Position;
	};

	Entity m_PrevID;
};