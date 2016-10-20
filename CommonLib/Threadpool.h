#pragma once
#include <queue>
#include "DataStructures/GrowingArray.h"
#include "Worker.h"
#include "Work.h"
class Threadpool
{
public:
	Threadpool();
	~Threadpool();

	void Update();
	void AddWork(Work aWorkOrder);
	void CleanUp();

private:
	std::queue<Work> myWorkOrders;
	CU::GrowingArray<Worker> myWorkers;
};

