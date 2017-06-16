#pragma once
#include <queue>
#include "DataStructures/GrowingArray.h"
#include "Worker.h"
#include "Work.h"
class Threadpool
{
public:
	Threadpool() = default;

	void Initiate(const std::string& debug_name);
	void Update();
	void AddWork(Work aWorkOrder);
	void CleanUp();
	
	bool HasWork() { return !myWorkOrders.empty(); }
	int GetWorkSize() const { return myWorkOrders.size(); }
	bool CurrentWorkFinished() const;
private:
	std::queue<Work> myWorkOrders;
	CU::GrowingArray<Worker> myWorkers;
};

