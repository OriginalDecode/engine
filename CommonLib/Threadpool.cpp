#include "Threadpool.h"
#include <thread>
#define FOR_EACH(_array) for (int i = 0; i < _array; i++)

CThreadpool::CThreadpool()
{
	myWorkers.Init(1);
	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		CWorker worker;
		myWorkers.Add(worker);
	}
	myWorkers.Optimize();

	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		myWorkers[i].Initiate();
	}
}

CThreadpool::~CThreadpool()
{
	myWorkers.RemoveAll();
}

void CThreadpool::Update()
{
	FOR_EACH(myWorkers.Size())
	{
		if (myWorkOrders.size() <= 0)
		{
			break;
		}

		if (myWorkers[i].IsDone())
		{
			myWorkers[i].AddWork(myWorkOrders.front());
			myWorkOrders.pop();
		}
	}
}

void CThreadpool::AddWork(CWork aWorkOrder)
{
	myWorkOrders.push(aWorkOrder);
}

void CThreadpool::CleanUp()
{
	FOR_EACH(myWorkers.Size())
	{
		myWorkers[i].CleanUp();
	}
}
