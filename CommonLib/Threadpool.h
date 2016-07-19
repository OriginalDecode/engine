#pragma once
#include <queue>
#include "DataStructures/GrowingArray.h"
#include "Worker.h"
#include "Work.h"
class CThreadpool
{
public:
	CThreadpool();
	~CThreadpool();

	void Update();
	void AddWork(CWork aWorkOrder);
	void CleanUp();

private:
	std::queue<CWork> myWorkOrders;
	CU::GrowingArray<CWorker> myWorkers;
};

