#pragma once
#include <atomic>
#include "Work.h"
namespace std
{
	class thread;
}

class Worker
{
public:
	Worker();
	~Worker();

	void Initiate();

	bool IsDone();
	void AddWork(const Work& aWorkOrder);
	void Stop();
	void CleanUp();

private:
	std::thread* myWorkThread;
	Work myWork;
	volatile std::atomic<bool> myIsDone;
	volatile std::atomic<bool> myIsRunning;
	volatile std::atomic<bool> myIsStopping = false;
	void Run();

};

