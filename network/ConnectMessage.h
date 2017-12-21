#pragma once
#include "NetMessage.h"

class ConnectMessage : public NetMessage
{
public:
	ConnectMessage();
    ConnectMessage(s32 message_id, GUID guid);
    ConnectMessage(s32 message_id, std::string connect_message);
    ConnectMessage(s32 message_id, std::string connect_message, GUID guid);
    
    std::string m_ConnectMessage;
private:
	void Serialize(StreamType& stream) override;
	void Deserialize(StreamType& stream) override;
};