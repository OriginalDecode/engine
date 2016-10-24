#pragma once
#include <functional>
class InputCommand
{
public:
	virtual ~InputCommand() { };
	std::function<void(void)> m_Function;

};

