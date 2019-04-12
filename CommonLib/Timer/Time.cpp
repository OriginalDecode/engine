#include "Time.h"
//
//namespace Base
//{
//	void Time::Update(long long& prev, long long& end, long long& freq)
//	{
//		m_Time = (end - prev) / static_cast<float>(freq);
//	}
//
//	float Time::AsHours() const
//	{
//		return (m_Time / 60.f) / 60.f;
//	}
//
//	float Time::AsMinutes() const
//	{
//		return m_Time / 60.f;
//	}
//
//	float Time::AsSeconds() const
//	{
//		return m_Time;
//	}
//
//	float Time::AsMS() const
//	{
//		return m_Time * 1000.f;
//	}
//
//	float Time::AsMicro() const
//	{
//		return (m_Time * 1000.f) * 1000.f;
//	}
//
//	float Time::GetFPS() const
//	{
//		return 1.f / m_Time;
//	}
//
//	void Time::SetTime(float someTime)
//	{
//		m_Time = someTime;
//	}
//};