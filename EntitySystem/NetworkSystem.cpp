#include "NetworkSystem.h"
#include "NetworkComponent.h"


NetworkSystem::NetworkSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<NetworkComponent>>())
{
}

void NetworkSystem::Update(float /*delta_time*/)
{

}