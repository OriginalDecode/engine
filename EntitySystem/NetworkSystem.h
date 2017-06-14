#pragma once
#include "BaseSystem.h"
class NetworkSystem : public BaseSystem
{
public:
	NetworkSystem(NodeEntityManager& anEntityManager);
	void Update(float delta_time) override;
};

