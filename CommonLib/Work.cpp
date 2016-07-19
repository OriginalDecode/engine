#include "Work.h"

CWork::CWork(const std::function<void()> aFunction)
	: myFunction(aFunction)
{
}

CWork::CWork()
{
}

CWork::~CWork()
{
}

void CWork::DoWork()
{
	myFunction();
}
