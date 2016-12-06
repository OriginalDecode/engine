#include "NetworkSystem.h"
#include "NetworkComponent.h"


NetworkSystem::NetworkSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<NetworkComponent>>())
{
}

void NetworkSystem::Update(float /*delta_time*/)
{

}