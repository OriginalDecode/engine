#include "ChatMessage.h"

ChatMessage::ChatMessage(std::string message)
{
	m_ID = CHAT_MESSAGE;
	m_ChatMessage = message;
}

void ChatMessage::Serialize(StreamType& stream)
{
	__SUPER::Serialize(stream);
	SERIALIZE(m_Stream, m_ChatMessage);
}

void ChatMessage::Deserialize(StreamType& stream)
{
	__SUPER::Deserialize(stream);
	DESERIALIZE(m_Stream, m_ChatMessage);
}