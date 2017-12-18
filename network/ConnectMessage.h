#pragma once
#include "NetMessage.h"

class ConnectMessage : public NetMessage
{
public:
    ConnectMessage() = default;
    ConnectMessage(s32 message_id);
    ConnectMessage(s32 message_id, std::string connect_message);
    ConnectMessage(s32 message_id, std::string connect_message, GUID guid);
    
    std::string m_ConnectMessage;
private:
	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};