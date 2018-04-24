#pragma once
#include "BaseSystem.h"
class AISystem final : public BaseSystem
{
public:
	AISystem(_EntityManager& anEntityManager);
	void Update(float delta_time, bool paused) override;
};

