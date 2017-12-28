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

constexpr float update_time = 1.f / 15.f;

void NetworkSystem::Update(float delta_time, bool /*paused*/)
{
	static float s_UpdateTime = 0.f;
	s_UpdateTime += delta_time;

	if (s_UpdateTime < update_time)
		return;
	s_UpdateTime = 0.f;

	const EntityArray& entities = GetEntities();
	for (Entity e : entities)
	{
		const NetworkComponent& net_c = m_Manager.GetComponent<NetworkComponent>(e);

		if (net_c.m_Owner != m_NetManager->GetGUID())
			continue;

		const TranslationComponent& translation = m_Manager.GetComponent<TranslationComponent>(e);
		const CU::Vector3f& pos = translation.GetOrientation().GetPosition();
		float _realPos[3] = { pos.x,pos.y,pos.z };
		m_NetManager->Send(NetEntityData(net_c.m_GUID, _realPos));
	}
}