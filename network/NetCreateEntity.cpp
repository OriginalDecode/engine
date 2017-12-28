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
	SERIALIZE(stream, cl::GuidToString(m_EntityGUID));
}

void NetCreateEntity::Deserialize(StreamType& stream)
{
	NetMessage::Deserialize(stream);
	std::string guid;
	DESERIALIZE(stream, guid);
	m_EntityGUID = cl::StrToGuid(guid);

}

