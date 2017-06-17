#pragma once
#include "BaseSystem.h"

class Synchronizer;
class RenderSystem : public BaseSystem
{
public:
	RenderSystem(NodeEntityManager& anEntityManager);

	void Update(float dt) override;
	bool Inside(const CU::Vector4f& translation, const CU::Vector4f& direction, const CU::Vector4f& pos);
private:
	Synchronizer* mySynchronizer;

};

