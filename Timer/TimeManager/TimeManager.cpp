#include "TimeManager.h"
#include <assert.h>



namespace CommonUtilities
{
	//TimeManager* TimeManager::myInstance = nullptr;

	//bool TimeManager::Create()
	//{
	//	if (myInstance == nullptr)
	//	{
	//		myInstance = new TimeManager();
	//		return true;
	//	}
	//
	//	return false;
	//}
	//
	//bool TimeManager::Destroy()
	//{
	//	if (myInstance == nullptr)
	//	{
	//		return false;
	//	}
	//
	//	delete(myInstance);
	//
	//	return true;
	//}
	//
	//TimeManager* TimeManager::GetInstance()
	//{
	//	return myInstance;
	//}

	TimeManager::TimeManager()
	{
		myMasterTimer.Start();
	}

	TimeManager::~TimeManager()
	{

	}

	int TimeManager::CreateTimer()
	{
		Timer newTimer;
		newTimer.Start();
		myTimers.push_back(newTimer);
		return static_cast<int>(myTimers.size()) - 1;
	}

	void TimeManager::Update()
	{
		myMasterTimer.Update();
		for (unsigned char i = 0; i < myTimers.size(); ++i)
		{
			myTimers[i].Update();
		}
	}

	void TimeManager::UpdateMaster()
	{
		myMasterTimer.Update();
	}

	Timer& TimeManager::GetTimer(int anIndex)
	{
		assert(anIndex < static_cast<int>(myTimers.size()) && "Too large Index");
		return myTimers[anIndex];
	}


	void TimeManager::Pause()
	{
		//if (myInstance != nullptr)
			myMasterTimer.Pause();
	}

	void TimeManager::Start()
	{
		//if (myInstance != nullptr)
			myMasterTimer.Start();
	}

}
