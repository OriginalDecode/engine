#pragma once
#include <chrono>

namespace Base
{
	using Time = __int64;
	class Timer
	{
	public:
		Timer();
		~Timer() = default;


		void Update();

		void Start();
		void Stop();
		void Pause();
		void Resume();


		const float GetTime() const;
		const float GetTotalTime() const;
	private:

		using TimePoint = std::chrono::steady_clock::time_point;

		TimePoint m_Start;		// application start time
		TimePoint m_Prev;		// current frame start time
		TimePoint m_Current;	// prev frame start time

		float m_Time = 0.f;


		bool m_IsActive : 4;
		bool m_IsPaused : 4;
	};
}
