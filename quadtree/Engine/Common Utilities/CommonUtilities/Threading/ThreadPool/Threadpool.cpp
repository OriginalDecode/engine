#include "Threadpool.h"

namespace CommonUtilities
{
	Threadpool::Threadpool()
	{
		myWorker.Init(4, true);

		for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			myWorker.Add(Worker());
			myWorker[i].Init();
		}
	}


	Threadpool::~Threadpool()
	{
		for (int i = 0; i < myWorker.Size(); i++)
		{
			myWorker[i].Stop();
		}
	}

	void Threadpool::AddWork(const Work& aWork)
	{
		myWork.push(aWork);
	}

	void Threadpool::Update()
	{
		while (myWork.empty() == false)
		{
			for (int i = 0; i < myWorker.Size(); i++)
			{
				if (myWorker[i].GetIsDone() == true)
				{
					myWorker[i].AddWork(myWork.front());
					myWork.pop();

					if (myWork.empty() == true)
					{
						break;
					}
				}
			}
		}

		WaitForWorkers();
	}

	void Threadpool::WaitForWorkers()
	{
		for (unsigned short i = 0; i < myWorker.Size(); ++i)
		{
			while (myWorker[i].GetIsDone() == false)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
		}
	}
}