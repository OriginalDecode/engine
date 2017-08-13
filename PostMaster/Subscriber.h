#pragma once

struct OnLeftClick;
#include "../standard_datatype.hpp"
class Subscriber
{
public:
	Subscriber() = default;

	virtual void HandleEvent(u64 , void* ) { };


	virtual void ReceiveMessage(const OnLeftClick& ) { }




};
