#pragma once
#include <atomic>
#include "Work.h"
#include <string.h>

namespace std
{
	class thread;
}

class Worker
{
public:
	Worker();
	~Worker();

	void Initiate(const std::string& debug_name );

	bool IsDone() const;
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

