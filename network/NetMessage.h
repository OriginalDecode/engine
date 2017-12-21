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
	u64 m_MessageType; //8 byte
    GUID m_GUID; //16 byte
	
protected:

	virtual void Serialize(StreamType& stream);
	virtual void Deserialize(StreamType& stream);

};