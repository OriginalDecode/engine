#include "AISystem.h"
#include "AIComponent.h"

AISystem::AISystem(_EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<AIComponent>>())
{
}

void AISystem::Update(float /*delta_time*/, bool paused)
{
	if (paused)
		return;

}

