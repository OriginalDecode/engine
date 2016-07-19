#pragma once
#include <functional>
class CWork
{
public:
	CWork();
	CWork(const std::function<void()> aFunction);
	~CWork();

	void DoWork(); /* Adapt for delta time. */

private:
	std::function<void()> myFunction;

};

