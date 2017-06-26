#pragma once

struct OnLeftClick;
#include "../standard_datatype.hpp"
class Subscriber
{
public:
	Subscriber() = default;

	virtual void HandleEvent(u64 event, void* data) { };


	virtual void ReceiveMessage(const OnLeftClick& message) { }




};
