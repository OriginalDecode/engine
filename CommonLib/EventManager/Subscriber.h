#pragma once

struct OnLeftClick;
#include <standard_datatype.hpp>
class Subscriber
{
public:
	Subscriber() = default;

	virtual void HandleEvent(uint64 /*event*/, void* /*pData*/) { };
	virtual void ReceiveMessage(const OnLeftClick& ) { }

};
