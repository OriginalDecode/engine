#pragma once
#include "BaseSystem.h"
class AISystem final : public BaseSystem
{
public:
	AISystem(CEntityManager& anEntityManager);
	void Update(float delta_time) override;
};

