#pragma once
#include "InspectorView.h"
class PhysicsView : public InspectorView
{
public:
	PhysicsView(EntityManager& em);
	~PhysicsView() override;

	void Update() override;

private:

};

