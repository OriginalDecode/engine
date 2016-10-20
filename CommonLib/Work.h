#pragma once
#include <functional>
class Work
{
public:
	Work();
	Work(std::function<void()> aFunction);
	~Work();

	void DoWork(); /* Adapt for delta time. */

private:
	std::function<void()> myFunction;

};

