#pragma once
#include <functional>
#include <string>
class Work
{
public:
	Work();
	Work(std::function<void()> aFunction);
	Work(std::function<void(std::string)> aFunction);

	~Work();

	void DoWork(); /* Adapt for delta time. */

private:
	std::function<void()> myFunction;

};

