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
		
		static bool Create();
		static bool Destroy();
		static TimeManager* GetInstance();
		static void Update();

		void UpdateMaster();

		int CreateTimer();

		Timer& GetTimer(int anIndex);
		inline const time_type GetDeltaTime();
		inline const time_type GetFPS();

		void Pause();
		void Start();

	private:
		TimeManager();
		~TimeManager();
		static TimeManager* myInstance;

		std::vector<Timer> myTimers;
		Timer myMasterTimer;
	};

	/*const time_type TimeManager::GetDeltaTime()
	{
		return myMasterTimer.GetTotalTime().GetSeconds();
	}*/
	const time_type TimeManager::GetDeltaTime()
	{
		return myMasterTimer.GetFrameTime().GetSeconds();
	}

	const time_type TimeManager::GetFPS()
	{
		return static_cast<time_type>(1) / GetDeltaTime();
	}

}
namespace CU = CommonUtilities;