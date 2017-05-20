#include "ConnectMessage.h"

ConnectMessage::ConnectMessage(s32 message_id, std::string connect_message)
{
	m_ID = message_id;
	m_ConnectMessage = connect_message;
}

ConnectMessage::ConnectMessage(s32 message_id, std::string connect_message, u32 gid)
{
	m_ID = message_id;
	m_ConnectMessage = connect_message;
	m_GID = gid;
}

ConnectMessage::ConnectMessage(s32 message_id)
{
    m_ID = message_id;
}



void ConnectMessage::Serialize(StreamType& stream)
{
	__SUPER::Serialize(stream);
	SERIALIZE(m_Stream, m_ConnectMessage);
}

void ConnectMessage::Deserialize(StreamType& stream)
{
	__SUPER::Deserialize(stream);
	DESERIALIZE(m_Stream, m_ConnectMessage);
}