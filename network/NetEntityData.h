#pragma once
#include "NetMessage.h"
class NetEntityData : public NetMessage
{
public:
	NetEntityData() = default;
	NetEntityData(float vec[3]);


private:
	float m_Position[3];

	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};

NetEntityData::NetEntityData(float vec[3])
{
	memcpy(&m_Position, vec, sizeof(float) * 3);
}

void NetEntityData::Serialize(StreamType& stream)
{
	/*NetMessage::Serialize(stream);
	SERIALIZE(m_Stream, m_Position[0]);
	SERIALIZE(m_Stream, m_Position[1]);
	SERIALIZE(m_Stream, m_Position[2]);*/
}

void NetEntityData::Deserialize(StreamType& stream)
{
	//NetMessage::Deserialize(stream);
	/*DESERIALIZE(m_Stream, m_Position[0]);
	DESERIALIZE(m_Stream, m_Position[1]);
	DESERIALIZE(m_Stream, m_Position[2]);*/
}

