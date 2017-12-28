#include "NetEntityData.h"
NetEntityData::NetEntityData(GUID entity_guid, float vec[3])
	: NetMessage(NET_ENTITY_DATA)
	, m_EntityGUID(entity_guid)
{
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

NetEntityData::NetEntityData()
	: NetMessage(NET_ENTITY_DATA)
{
}

void NetEntityData::Serialize(StreamType& stream)
{
	NetMessage::Serialize(stream);
	SERIALIZE(stream, cl::GuidToString(m_EntityGUID));
	SERIALIZE(m_Stream, x);
	SERIALIZE(m_Stream, y);
	SERIALIZE(m_Stream, z);
}

void NetEntityData::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	std::string guid;
	DESERIALIZE(stream, guid);
	m_EntityGUID = cl::StrToGuid(guid);

	DESERIALIZE(m_Stream, x);
	DESERIALIZE(m_Stream, y);
	DESERIALIZE(m_Stream, z);
}

