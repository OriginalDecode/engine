#pragma once

struct OnLeftClick;

class Subscriber
{
public:
	Subscriber() = default;
	virtual void ReceiveMessage(const OnLeftClick& message);

};

