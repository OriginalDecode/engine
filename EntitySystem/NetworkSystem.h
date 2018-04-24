#pragma once
#include "BaseSystem.h"
namespace network
{
	class NetworkManager;
};

class NetworkSystem : public BaseSystem
{
public:
	NetworkSystem(_EntityManager& anEntityManager);
	void Update(float delta_time, bool paused) override;

private:

	network::NetworkManager* m_NetManager = nullptr;

};

