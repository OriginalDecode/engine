#include "EventManager.h"
#include "../../DL_Debug/DL_Debug.h"
#include "../Utilities.h"

EventManager* EventManager::myInstance = nullptr;
EventManager::EventManager()
{
	for (int i = 0; i < static_cast<int>(eMessageType::COUNT); ++i)
	{
		mySubscribers[i].Init(64);
	}
}


EventManager::~EventManager()
{
}

EventManager* EventManager::GetInstance()
{
	return myInstance;
}

void EventManager::Create()
{
	myInstance = new EventManager();
}

void EventManager::Destroy()
{
	delete myInstance;
	myInstance = nullptr;
}

void EventManager::Subscribe(const eMessageType aMessageType, Subscriber* aSubscriber, ePriorityLayer aPriority, bool aLetThrough)
{
#ifdef _DEBUG
	CU::GrowingArray<SubscriberInfo>& subscribers = mySubscribers[static_cast<int>(aMessageType)];
	for (int i = 0; i < subscribers.Size(); ++i)
	{
		ASSERT(subscribers[i].mySubscriber != aSubscriber, "Tried to add the same subscriber to the same message twice.");
	}
#endif

	SubscriberInfo newSubscriber;
	newSubscriber.mySubscriber = aSubscriber;
	newSubscriber.myPriority = aPriority;
	newSubscriber.myLetThrough = aLetThrough;

	if (aPriority == ePriorityLayer::NO_PRIO)
	{
		mySubscribers[static_cast<int>(aMessageType)].Add(newSubscriber);
	}
	else
	{
		mySubscribers[static_cast<int>(aMessageType)].Add(newSubscriber);
		SortSubscribers(mySubscribers[static_cast<int>(aMessageType)]);
	}
}

void EventManager::Subscribe(const std::string& event, Subscriber* subscriber)
{
	uint64 hash = cl::Hash(event.c_str());
	Subscribe(hash, subscriber);
}

void EventManager::Subscribe(const uint64& event, Subscriber* subscriber)
{
	SubscriberInfo newSubscriber;
	newSubscriber.mySubscriber = subscriber;
	newSubscriber.myPriority = ePriorityLayer::NORMAL;
	newSubscriber.myLetThrough = true;



	if(m_EventSubscribers.find(event) == m_EventSubscribers.end())
		m_EventSubscribers.emplace(event, CU::GrowingArray<SubscriberInfo>());

	m_EventSubscribers.at(event).Add(newSubscriber);
}

void EventManager::UnSubscribe(const eMessageType aMessageType, Subscriber* aSubscriber)
{
	CU::GrowingArray<SubscriberInfo>& subscribers
		= mySubscribers[static_cast<int>(aMessageType)];

	for (int i = 0; i < subscribers.Size(); ++i)
	{
		if (subscribers[i].mySubscriber == aSubscriber)
		{
			subscribers.RemoveCyclicAtIndex(i);
			break;
		}
	}

	SortSubscribers(subscribers);
}

void EventManager::UnSubscribe(Subscriber* aSubscriber)
{
	for (int i = 0; i < static_cast<int>(eMessageType::COUNT); ++i)
	{
		CU::GrowingArray<SubscriberInfo, int>& subscribers
			= mySubscribers[i];

		for (int j = 0; j < subscribers.Size(); ++j)
		{
			if (subscribers[j].mySubscriber == aSubscriber)
			{
				subscribers.RemoveCyclicAtIndex(j);
				break;
			}
		}
	}
}

bool EventManager::IsSubscribed(const eMessageType aMessageType, Subscriber* aSubscriber)
{
	CU::GrowingArray<SubscriberInfo>& subscribers
		= mySubscribers[static_cast<int>(aMessageType)];

	for (int i = 0; i < subscribers.Size(); ++i)
	{
		if (subscribers[i].mySubscriber == aSubscriber)
		{
			return true;
		}
	}

	return false;
}

void EventManager::SendMessage(const char* event, void* data)
{
	uint64 hash = cl::Hash(event);
	SendMessage(hash, data);
}

void EventManager::SendMessage(const uint64& event, void* data)
{

	auto item = m_EventSubscribers.find(event);

	if (item != m_EventSubscribers.end())
	{
		CU::GrowingArray<SubscriberInfo>& subscribers = item->second;//m_EventSubscribers.at(event);

		if (subscribers.Size() > 0)
		{
			for (int i = 0; i < subscribers.Size(); ++i)
			{
				subscribers[i].mySubscriber->HandleEvent(event, data);
			}
		}
	}
}

void EventManager::SortSubscribers(CU::GrowingArray<SubscriberInfo> &aBuffer)
{
	int max = 0;
	if (aBuffer.Size() < 3)
	{
		if (aBuffer.Size() > 1 && (aBuffer[1].myPriority > aBuffer[0].myPriority))
		{
			std::swap(aBuffer[1], aBuffer[0]);
		}
		return;
	}

	for (int i = 0; i < aBuffer.Size(); ++i)
	{
		if (aBuffer[max].myPriority < aBuffer[i].myPriority)
			max = i;
	}

	std::swap(aBuffer[max], aBuffer[aBuffer.Size() - 1]);
	QuickSort(aBuffer, 0, aBuffer.Size() - 2);
}

void EventManager::QuickSort(CU::GrowingArray<SubscriberInfo> &aBuffer, const int aStart, const int aEnd)
{
	int lower = aStart + 1;
	int upper = aEnd;

	std::swap(aBuffer[aStart], aBuffer[(aStart + aEnd) / 2]);

	int bound = static_cast<int>(aBuffer[aStart].myPriority);

	while (lower <= upper)
	{
		while (bound > static_cast<int>(aBuffer[lower].myPriority))
			++lower;

		while (bound < static_cast<int>(aBuffer[upper].myPriority))
			--upper;

		if (lower < upper)
			std::swap(aBuffer[lower++], aBuffer[upper--]);
		else
			++lower;
	}

	std::swap(aBuffer[aStart], aBuffer[upper]);

	if (aStart < upper - 1)
		QuickSort(aBuffer, aStart, upper - 1);
	if (upper + 1 < aEnd)
		QuickSort(aBuffer, upper + 1, aEnd);
}

void EventManager::SendMessage(const uint64& event)
{
	SendMessage(event, nullptr);
}

void EventManager::SendMessage(const char* event)
{
	SendMessage(event, nullptr);
}


void EventManager::Send(const uint64& event, void* data)
{
	myInstance->SendMessage(event, data);
}

void EventManager::Send(const char* event, void* data)
{
	myInstance->SendMessage(event, data);
}