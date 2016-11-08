#include "NetworkSystem.h"
#include "NetworkComponent.h"


NetworkSystem::NetworkSystem(CEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<NetworkController>>())
{
}

void NetworkSystem::Update(float /*delta_time*/)
{

}