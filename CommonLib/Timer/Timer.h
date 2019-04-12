#pragma once


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

		void ClearTime();

		float GetTime();

		//const Time& GetTime() const;
		//const Time& GetTotalTime() const;


	private:

		float m_Time = 0.f;

		Time m_Start = 0; //application start time

		Time m_Current = 0; // frame start time
		Time m_Prev = 0; // prev frame start time

		bool m_IsActive : 4;
		bool m_IsPaused : 4;
	};
}
