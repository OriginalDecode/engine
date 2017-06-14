#include "AISystem.h"
#include "AIComponent.h"

AISystem::AISystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<AIComponent>>())
{
}

void AISystem::Update(float /*delta_time*/)
{

}

