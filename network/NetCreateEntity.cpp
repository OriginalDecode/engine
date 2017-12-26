#include "NetCreateEntity.h"
NetCreateEntity::NetCreateEntity()
	: NetMessage(NET_CREATE_ENTITY)
{
}

NetCreateEntity::NetCreateEntity(GUID guid)
	: NetMessage(NET_CREATE_ENTITY)
	, m_EntityGUID(guid)
{
}


void NetCreateEntity::Serialize(StreamType& stream)
{
	NetMessage::Serialize(stream);

	OLECHAR* guidString;
	StringFromCLSID(m_EntityGUID, &guidString);
	SERIALIZE(m_Stream, guidString);
	::CoTaskMemFree(guidString);
	//SERIALIZE(m_Stream, m_EntityGUID.Data4);
}

void NetCreateEntity::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	DESERIALIZE(m_Stream, m_EntityGUID.Data1);
	DESERIALIZE(m_Stream, m_EntityGUID.Data2);
	DESERIALIZE(m_Stream, m_EntityGUID.Data3);
	//DESERIALIZE(m_Stream, m_EntityGUID.Data4);




}

