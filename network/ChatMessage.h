#pragma once
#include "NetMessage.h"
#include <string.h>
class ChatMessage : public NetMessage
{
public:
	ChatMessage() = default;
	ChatMessage(std::string chat_message);

	std::string m_ChatMessage;

private:

	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;

};


