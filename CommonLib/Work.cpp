#include "Work.h"

Work::Work(const std::function<void()> aFunction)
	: myFunction(aFunction)
{
}

Work::Work()
{
}

Work::Work(std::function<void(std::string)> aFunction)
{

}

Work::~Work()
{
}

void Work::DoWork()
{
	myFunction();
}
