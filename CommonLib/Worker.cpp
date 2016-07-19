#include "Worker.h"
#include "Work.h"
#include <thread>
#include <iostream>
CWorker::CWorker()
	: myIsDone(true)
	, myIsRunning(true)
	, myWorkThread(nullptr)
{
}

CWorker::~CWorker()
{
	myIsDone = true;
	myIsRunning = false;
}

void CWorker::Initiate()
{
	myWorkThread = new std::thread([&] {Run(); });
}

bool CWorker::IsDone()
{
	return myIsDone;
}

void CWorker::AddWork(const CWork& aWorkOrder)
{
	myWork = aWorkOrder;
	myIsDone = false;
}

void CWorker::Stop()
{
	myIsRunning = false;
	myIsDone = true;
}

void CWorker::CleanUp()
{
	myIsStopping = true;
	myIsDone = true;
	myIsRunning = false;

	myWorkThread->join();
	delete myWorkThread;
	myWorkThread = nullptr;
}

void CWorker::Run()
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
