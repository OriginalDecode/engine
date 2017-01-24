#include "TimeManager.h"

#include <assert.h>

namespace CommonUtilities
{
	TimeManager::TimeManager()
	{
		myMasterTimer.Start();
	}

	int TimeManager::CreateTimer()
	{
		Timer newTimer;
		newTimer.Start();
		myTimers.push_back(newTimer);
		return myTimers.size() - 1;
	}

	void TimeManager::Update()
	{
		for (unsigned int i = 0; i < myTimers.size(); ++i)
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
}
