#include "NetMessage.h"
#include <Engine/Engine.h>
#include <network/NetworkManager.h>

NetMessage::NetMessage(eNetMessageType type)
	: m_MessageType(type)
	, m_GUID(Engine::GetInstance()->GetNetworkManager()->GetGUID())
{
	
}

NetMessage::~NetMessage() = default;

void NetMessage::PackMessage()
{
	Serialize(m_Stream);
}

void NetMessage::UnpackMessage(int8* message, int32 length)
{
	if(length > 0)
	{
		for(int32 i = 0; i < length; i++)
		{
			 m_Stream.push_back(message[i]);
		}
		Deserialize(m_Stream);
	}
}

bool NetMessage::IsType(eNetMessageType message_type)
{
	return (m_MessageType == (uint64)message_type);
}

void NetMessage::Serialize(StreamType& stream)
{
	SERIALIZE(stream, m_MessageType);
	SERIALIZE(stream, cl::GuidToString(m_GUID));
}

void NetMessage::Deserialize(StreamType& stream)
{
	DESERIALIZE(stream, m_MessageType);
	std::string guid;
	DESERIALIZE(stream, guid);
	m_GUID = cl::StrToGuid(guid);
}