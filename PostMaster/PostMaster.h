#pragma once
#include "../standard_datatype.hpp"

#include <DataStructures/GrowingArray.h>
#include <DataStructures/StaticArray.h>
#include <Math/Vector/Vector.h>
#include <DataStructures/VectorOnStack.h>

#include "Message.h"
#include "MessageEnum.h"
#include "Subscriber.h"
#include <unordered_map>
#include <map>
#undef SendMessage

enum class ePriorityLayer
{
	NO_PRIO,
	NORMAL,
	HIGH
};

struct SubscriberInfo
{
	SubscriberInfo() = default;
	Subscriber* mySubscriber;
	ePriorityLayer myPriority = ePriorityLayer::NO_PRIO;
	bool myLetThrough = true;
};

class PostMaster
{
public:
	static PostMaster* GetInstance();
	static void Create();
	static void Destroy();

	void Subscribe(const eMessageType aMessageType, Subscriber* aSubscriber, ePriorityLayer aPriority = ePriorityLayer::NO_PRIO, bool aLetThrough = true);
	void Subscribe(const std::string& event, Subscriber* subscriber);
	void Subscribe(const u64& event, Subscriber* subscriber);

	void UnSubscribe(const eMessageType aMessageType, Subscriber* aSubscriber);
	void UnSubscribe(Subscriber* aSubscriber);
	bool IsSubscribed(const eMessageType aMessageType, Subscriber* aSubscriber);

	template<typename T>
	void SendMessage(const T& aMessage);

	void SendMessage(const u64& event, void* data);
	void SendMessage(const char* event, void* data);

	void SendMessage(const u64& event);
	void SendMessage(const char* event);
private:
	PostMaster();
	~PostMaster();
	static PostMaster* myInstance;
	
	void SortSubscribers(CU::GrowingArray<SubscriberInfo>& aSubscribers);
	void QuickSort(CU::GrowingArray<SubscriberInfo>& aBuffer, const int aStart, const int aEnd);

	CU::StaticArray<CU::GrowingArray<SubscriberInfo>, static_cast<int>(eMessageType::COUNT)> mySubscribers;
	std::map<u64, CU::GrowingArray<SubscriberInfo>> m_EventSubscribers;
};

template<typename T>
void PostMaster::SendMessage(const T& aMessage)
{
	CU::GrowingArray<SubscriberInfo>& subscribers = mySubscribers[static_cast<int>(aMessage.myMessageType)];

	if (subscribers.Size() > 0)
	{
		for (int i = 0; i < subscribers.Size(); ++i)
		{
			subscribers[i].mySubscriber->ReceiveMessage(aMessage);

			if (subscribers[i].myLetThrough == false)
			{
				return;
			}
		}
	}
	else 
	{
		DL_ASSERT("Message sent without subscriber.");
	}
}