#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>
#include "Timer.h"
#include "Time.h"
namespace CommonUtilities
{
	class TimeManager
	{
	public:
		friend class Megaton;

		int GetTimerID();

		void Update();
		void UpdateMaster();

		int CreateTimer();

		Timer& GetTimer(int anIndex);
		inline const float GetDeltaTime();
		inline const float GetFPS();


	private:
		TimeManager();

		Timer aTimers;
		std::vector<Timer> myTimers;
		Timer myMasterTimer;
	};

	const float TimeManager::GetDeltaTime()
	{
		return myMasterTimer.GetFrameTime().GetSeconds();
	}
	const float TimeManager::GetFPS()
	{
		return 1 / GetDeltaTime();
	}
}
namespace CU = CommonUtilities;