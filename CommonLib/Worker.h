#pragma once
#include <atomic>
#include "Work.h"
namespace std
{
	class thread;
}

class CWorker
{
public:
	CWorker();
	~CWorker();

	void Initiate();

	bool IsDone();
	void AddWork(const CWork& aWorkOrder);
	void Stop();
	void CleanUp();

private:
	std::thread* myWorkThread;
	CWork myWork;
	volatile std::atomic<bool> myIsDone;
	volatile std::atomic<bool> myIsRunning;
	volatile std::atomic<bool> myIsStopping = false;
	void Run();

};

