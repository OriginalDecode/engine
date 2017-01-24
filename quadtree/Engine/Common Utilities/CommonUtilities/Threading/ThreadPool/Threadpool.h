#pragma once

#include <queue>
#include <thread>
#include <atomic>

#include "..\..\DataStructures\GrowingArray.h"
#include "Work.h"
#include "Worker.h"

namespace CommonUtilities
{
	class Threadpool
	{
	public:
		Threadpool();
		~Threadpool();

		void Update();
		void WaitForWorkers();
		void AddWork(const Work& aWork);

	private:

		CU::GrowingArray<Worker> myWorker;
		std::queue<Work> myWork;
	};

}

namespace CU = CommonUtilities;