#pragma once
#include <CommonLib/JSON/include/writer.h>
#include <CommonLib/JSON/include/prettywriter.h>
#include <CommonLib/JSON/include/document.h>
#include <network/NetMessageTypes.h>
struct BaseComponent
{
	virtual ~BaseComponent() = 0;
	bool m_Updated = false;
};

struct NetReplicate
{
	virtual void OnNotify(eNetMessageType type, void* pData) = 0;
};