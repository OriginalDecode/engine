#include "Work.h"

Work::Work(const std::function<void()> aFunction)
	: myFunction(aFunction)
{
}

Work::Work()
{
}

Work::~Work()
{
}

void Work::DoWork()
{
	myFunction();
}
