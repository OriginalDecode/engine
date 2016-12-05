#include "AISystem.h"
#include "AIComponent.h"

AISystem::AISystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<AIController>>())
{
}

void AISystem::Update(float /*delta_time*/)
{

}

