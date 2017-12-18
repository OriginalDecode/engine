#include "ConnectMessage.h"

ConnectMessage::ConnectMessage(s32 message_id, std::string connect_message)
{
	m_MessageType = message_id;
	m_ConnectMessage = connect_message;
}

ConnectMessage::ConnectMessage(s32 message_id, std::string connect_message, GUID guid)
{
	m_MessageType = message_id;
	m_ConnectMessage = connect_message;
	m_GUID = guid;
}

ConnectMessage::ConnectMessage(s32 message_id)
{
    m_MessageType = message_id;
}



void ConnectMessage::Serialize(StreamType& stream)
{
	NetMessage::Serialize(stream);
	SERIALIZE(m_Stream, m_ConnectMessage);
}

void ConnectMessage::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	DESERIALIZE(m_Stream, m_ConnectMessage);
}