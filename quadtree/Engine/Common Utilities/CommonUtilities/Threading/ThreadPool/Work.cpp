#include "Work.h"

namespace CommonUtilities
{
	Work::Work()
	{
	}

	Work::Work(std::function<void()> aWork)
	{
		myWork = aWork;
	}


	Work::~Work()
	{

	}


	void Work::DoWork()
	{
		myWork();
	}
}