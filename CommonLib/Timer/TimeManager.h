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

		Base::Timer& GetTimer(int anIndex);
		inline const float GetDeltaTime();
		inline const float GetFPS();
		inline const float GetFrameTime();

		void Pause();
		void Start();

		Base::Timer& GetMasterTimer() { return myMasterTimer; }

	private:
		//static TimeManager* myInstance;

		std::vector<Base::Timer> myTimers;
		Base::Timer myMasterTimer;
	};

	const float TimeManager::GetDeltaTime()
	{
		return myMasterTimer.GetTime();
	}

	const float TimeManager::GetFPS()
	{
		return 0.f;
	}


	const float TimeManager::GetFrameTime()
	{
		return myMasterTimer.GetTime();
	}


}
