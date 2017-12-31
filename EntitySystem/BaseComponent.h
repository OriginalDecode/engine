#pragma once
#include <CommonLib/JSON/include/writer.h>
#include <CommonLib/JSON/include/prettywriter.h>
#include <CommonLib/JSON/include/document.h>
#include <network/NetMessageTypes.h>
typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> JsonWriter;
struct BaseComponent
{
	virtual ~BaseComponent() = 0;
	bool m_Updated = false;

	virtual void Serialize(JsonWriter& writer) const = 0;
	virtual void Deserialize(const rapidjson::Value& value) = 0;

};

struct NetReplicate
{
	virtual void OnNotify(eNetMessageType type, void* pData) = 0;
};