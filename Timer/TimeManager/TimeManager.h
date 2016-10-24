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
		
		TimeManager();
		~TimeManager();
		void Update();

		void UpdateMaster();

		int CreateTimer();

		Timer& GetTimer(int anIndex);
		inline const time_type GetDeltaTime();
		inline const time_type GetFPS();
		inline const time_type GetFrameTime();

		void Pause();
		void Start();

	private:
		//static TimeManager* myInstance;

		std::vector<Timer> myTimers;
		Timer myMasterTimer;
	};

	const time_type TimeManager::GetDeltaTime()
	{
		return myMasterTimer.GetFrameTime().GetSeconds();
	}

	const time_type TimeManager::GetFPS()
	{
		return static_cast<time_type>(1) / GetDeltaTime();
	}


	const time_type TimeManager::GetFrameTime()
	{
		return myMasterTimer.GetFrameTime().GetSeconds();
	}


}
