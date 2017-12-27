#include "NetMessage.h"

NetMessage::NetMessage(eNetMessageType type)
{
	m_MessageType = type;
}

NetMessage::~NetMessage() = default;

void NetMessage::PackMessage()
{
	Serialize(m_Stream);
}

void NetMessage::UnpackMessage(s8* message, s32 length)
{
	if(length > 0)
	{
		for(s32 i = 0; i < length; i++)
		{
			 m_Stream.push_back(message[i]);
		}
		Deserialize(m_Stream);
	}
}

bool NetMessage::IsType(eNetMessageType message_type)
{
	return (m_MessageType == (u64)message_type);
}

void NetMessage::Serialize(StreamType& stream)
{
	SERIALIZE(stream, m_MessageType);

	OLECHAR* str;
	HRESULT hr = StringFromCLSID(m_GUID, &str);
	assert(hr == S_OK && "Failed to convert to string!");
	SERIALIZE(stream, str);
	::CoTaskMemFree(str);
}

void NetMessage::Deserialize(StreamType& stream)
{
	DESERIALIZE(stream, m_MessageType);
	std::string guid;
	guid.reserve(60);
	DESERIALIZE(stream, guid);
	//HRESULT hr = CLSIDFromString(str, &m_GUID);
	//assert(hr == S_OK && "Failed to convert to string!");
	//::CoTaskMemFree(str);
	//delete str;

}