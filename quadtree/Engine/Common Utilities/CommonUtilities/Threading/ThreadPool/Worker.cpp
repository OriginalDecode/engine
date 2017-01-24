#include "Worker.h"

namespace CommonUtilities
{
	Worker::Worker()
	{
		myIsDone = true;
		myThread = nullptr;
		myIsRunning = true;
	}


	Worker::~Worker()
	{
		if (myThread != nullptr)
		{
			myThread->join();
			delete myThread;
			myThread = nullptr;
		}
	}

	void Worker::Init()
	{
		myThread = new std::thread([&] {Run(); });
	}


	bool Worker::GetIsDone()
	{
		return myIsDone;
	}

	void Worker::AddWork(const Work& aWork)
	{
		myWork = aWork;
		myIsDone = false;
	}

	void Worker::Run()
	{
		while (myIsRunning == true)
		{
			while (myIsDone == true)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}

			myWork.DoWork();
			myIsDone = true;
		}
	}

	void Worker::Stop()
	{
		myIsRunning = false;
	}
};