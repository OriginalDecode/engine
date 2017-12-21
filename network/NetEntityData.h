#pragma once
#include "NetMessage.h"
class NetEntityData : public NetMessage
{
public:
	NetEntityData();
	NetEntityData(float vec[3]);


	float x, y, z;
private:
	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};

NetEntityData::NetEntityData(float vec[3])
	: NetMessage(NET_ENTITY_DATA)
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

NetEntityData::NetEntityData()
	: NetMessage(NET_ENTITY_DATA)
{
}

void NetEntityData::Serialize(StreamType& stream)
{
	NetMessage::Serialize(stream);
	SERIALIZE(m_Stream, x);
	SERIALIZE(m_Stream, y);
	SERIALIZE(m_Stream, z);
}

void NetEntityData::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	DESERIALIZE(m_Stream, x);
	DESERIALIZE(m_Stream, y);
	DESERIALIZE(m_Stream, z);
}

