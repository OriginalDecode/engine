#pragma once
#include <atomic>
#include <thread>

#include "Work.h"

namespace CommonUtilities
{

	class Worker
	{
	public:
		Worker();
		~Worker();

		void Init();

		bool GetIsDone();
		void AddWork(const Work& aWork);

		void Stop();
	private:

		void Run();

		std::atomic_bool myIsDone;
		std::atomic_bool myIsRunning;

		Work myWork;
		std::thread* myThread;
	};

}
namespace CU = CommonUtilities;