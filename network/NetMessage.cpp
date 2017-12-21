#include "NetMessage.h"

NetMessage::NetMessage(eNetMessageType type)
{
	m_MessageType = type;
}

NetMessage::~NetMessage() = default;

void NetMessage::PackMessage()
{
	Serialize(m_Stream);
}

void NetMessage::UnpackMessage(s8* message, s32 length)
{
	if(length > 0)
	{
		for(s32 i = 0; i < length; i++)
		{
			 m_Stream.push_back(message[i]);
		}
		Deserialize(m_Stream);
	}
	//constexpr int s = sizeof(u64);
}

bool NetMessage::IsType(eNetMessageType message_type)
{
	return (m_MessageType == (u64)message_type);
}

void NetMessage::Serialize(StreamType& stream)
{
	SERIALIZE(m_Stream, m_MessageType);
	SERIALIZE(m_Stream, m_GUID);
}

void NetMessage::Deserialize(StreamType& stream)
{
	DESERIALIZE(m_Stream, m_MessageType);
	DESERIALIZE(m_Stream, m_GUID);
}