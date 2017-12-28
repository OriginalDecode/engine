#pragma once
#include "NetMessage.h"
class NetEntityData : public NetMessage
{
public:
	NetEntityData();
	NetEntityData(GUID entity_guid, float vec[3]);


	GUID m_EntityGUID;
	float x, y, z;
private:
	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};
