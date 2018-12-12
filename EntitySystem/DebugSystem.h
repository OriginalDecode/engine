#pragma once
#include "BaseSystem.h"
#include <Subscriber.h>
#include <Math/Vector/Vector.h>
#include <Math/Matrix/Matrix.h>
struct OnLeftClick;
struct DebugComponent;

class Synchronizer;

struct GizmoHandle;

#if !defined(_FINAL) && !defined(_PROFILE)
class DebugSystem : public BaseSystem, public Subscriber
{
public:
	DebugSystem(_EntityManager& entity_manager);
	~DebugSystem();
	void Update(float dt, bool paused) override;
private:
	void HandleEvent(uint64 event, void* data = nullptr) override;
	void ReceiveMessage(const OnLeftClick& message) override;

	void UpdateOBBs();
	bool CheckGizmoCollision(const CU::Vector3f& cam_pos, const CU::Vector3f& ray_dir);
	Synchronizer* m_Synchronizer = nullptr;
	double m_AccumulatedTime = 0.0f;
	void RenderBox(const DebugComponent& component, const CU::Matrix44f& orientation);
	struct entity_collisions
	{
		Entity m_ID;
		CU::Vector3f m_Position;
	};
	float m_MouseDeltaModifier = 1.f;
	int32 m_PrevID;
	int32 m_CurrentEntity;
	bool m_Holding = false;
	GizmoHandle* m_Direction = nullptr;
	std::string current_model;


};
#endif