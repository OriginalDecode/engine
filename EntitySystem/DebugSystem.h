#pragma once
#include "BaseSystem.h"
#include <Subscriber.h>
#include <Math/Vector/Vector.h>

struct OnLeftClick;
namespace Hex
{
	class Synchronizer;
};

class DebugSystem : public BaseSystem, public Subscriber
{
public:
	DebugSystem(EntityManager& entity_manager);
	~DebugSystem();
	void Update(float dt) override;
private:
	void ReceiveMessage(const OnLeftClick& message) override;
	Hex::Synchronizer* m_Synchronizer = nullptr;
	double m_AccumulatedTime = 0.0f;
	struct entity_collisions
	{
		Entity m_ID;
		CU::Vector3f m_Position;
	};
	CU::GrowingArray<entity_collisions> m_Collisions;
};