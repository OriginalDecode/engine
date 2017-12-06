#pragma once
#include "serialize.h"
#include "NetMessageTypes.h"
#include "NetIncludes.h"
#define SERIALIZE(stream, type) serialize(type,stream)
#define DESERIALIZE(stream, type) type = deserialize<decltype(type)>(stream)

class NetMessage
{
public:
	NetMessage() = default;
	virtual ~NetMessage(); //set to default in .cpp
	void PackMessage();
	void UnpackMessage(s8* message, s32 length);
	bool IsType(eNetMessageType message_type);


	StreamType m_Stream;
	u64 m_MessageType;
    u32 m_GID;
protected:

	virtual void Serialize(StreamType& stream);
	virtual void Deserialize(StreamType& stream);

};


