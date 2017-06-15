#include "Threadpool.h"
#include <thread>
#define FOR_EACH(_array) for (int i = 0; i < _array; i++)

void Threadpool::Initiate(const std::string& debug_name)
{
	myWorkers.ReInit(1);

	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		Worker worker;
		myWorkers.Add(worker);
	}
	myWorkers.Optimize();

	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		std::stringstream ss;
		ss << debug_name << i;
		myWorkers[i].Initiate(ss.str());
	}
}

void Threadpool::Update()
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

void Threadpool::AddWork(Work aWorkOrder)
{
	myWorkOrders.push(aWorkOrder);
}

void Threadpool::CleanUp()
{
	FOR_EACH(myWorkers.Size())
	{
		myWorkers[i].CleanUp();
	}
}
