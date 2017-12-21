#pragma once
#include "NetMessage.h"

class NetCreateEntity: public NetMessage
{
public:
	NetCreateEntity();
	NetCreateEntity(GUID guid);

	GUID m_EntityGUID;

private: 
	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};


