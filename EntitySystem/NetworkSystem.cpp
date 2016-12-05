#include "NetworkSystem.h"
#include "NetworkComponent.h"


NetworkSystem::NetworkSystem(EntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<NetworkController>>())
{
}

void NetworkSystem::Update(float /*delta_time*/)
{

}