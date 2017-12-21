#pragma once
#include "BaseSystem.h"
namespace network
{
	class NetworkManager;
};

class NetworkSystem : public BaseSystem
{
public:
	NetworkSystem(NodeEntityManager& anEntityManager);
	void Update(float delta_time, bool paused) override;

private:

	network::NetworkManager* m_NetManager = nullptr;

};

