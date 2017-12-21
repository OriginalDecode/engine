#include "NetworkSystem.h"
#include "NetworkComponent.h"
#include <engine/Engine.h>
#include <network/NetworkManager.h>
#include "TranslationComponent.h"
#include <network/NetEntityData.h>

NetworkSystem::NetworkSystem(NodeEntityManager& anEntityManager)
	: BaseSystem(anEntityManager, CreateFilter<Requires<NetworkComponent>>())
	, m_NetManager(Engine::GetInstance()->GetNetworkManager())
{
}

void NetworkSystem::Update(float /*delta_time*/, bool /*paused*/)
{

	const EntityArray& entities = GetEntities();
	for (Entity e : entities)
	{
		//const NetworkComponent& net_c = m_Manager.GetComponent<NetworkComponent>(e);

		//if (net_c.m_Owner != m_NetManager->GetGUID())
		//	continue;

		//const TranslationComponent& translation = m_Manager.GetComponent<TranslationComponent>(e);
		//const CU::Vector3f& pos = translation.GetOrientation().GetPosition();
		//float _realPos[3] = { pos.x,pos.y,pos.z };
		//m_NetManager->Send(NetEntityData(_realPos));
	}
}