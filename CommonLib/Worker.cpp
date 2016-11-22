#include "Worker.h"
#include "Work.h"
#include <thread>
#include "Utilities.h"
Worker::Worker()
	: myIsDone(true)
	, myIsRunning(true)
	, myWorkThread(nullptr)
{
}

Worker::~Worker()
{
	myIsDone = true;
	myIsRunning = false;
}

void Worker::Initiate()
{
	myWorkThread = new std::thread([&] {Run(); });
	CL::SetThreadName(myWorkThread->get_id(), "Worker Thread");
}

bool Worker::IsDone()
{
	return myIsDone;
}

void Worker::AddWork(const Work& aWorkOrder)
{
	myWork = aWorkOrder;
	myIsDone = false;
}

void Worker::Stop()
{
	myIsRunning = false;
	myIsDone = true;
}

void Worker::CleanUp()
{
	myIsStopping = true;
	myIsDone = true;
	myIsRunning = false;

	myWorkThread->join();
	delete myWorkThread;
	myWorkThread = nullptr;
}

void Worker::Run()
{
	while (myIsRunning)
	{
		while (myIsDone)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			if (myIsStopping)
				break;
		}
		if (!myIsDone)
		{
			myWork.DoWork();
			myIsDone = true;
		}
	}
}
