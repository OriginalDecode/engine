#include "ChatMessage.h"

ChatMessage::ChatMessage(std::string message)
{
	m_MessageType = CHAT_MESSAGE;
	m_ChatMessage = message;
}

void ChatMessage::Serialize(StreamType& stream)
{
	NetMessage::Serialize(stream);
	SERIALIZE(m_Stream, m_ChatMessage);
}

void ChatMessage::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	DESERIALIZE(m_Stream, m_ChatMessage);
}