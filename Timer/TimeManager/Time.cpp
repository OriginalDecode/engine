#include "Time.h"

namespace CommonUtilities
{
		Time::Time()
		{
			myDeltaSeconds = 0;
		}


		Time::~Time()
		{
		}

		void Time::Update(LARGE_INTEGER& aLastEnd, LARGE_INTEGER& anEnd, LARGE_INTEGER& aClockFreq)
		{
			myDeltaSeconds = (anEnd.QuadPart - aLastEnd.QuadPart) / static_cast<time_type>(aClockFreq.QuadPart);
		}

		time_type Time::GetHours()
		{
			return static_cast<time_type>((myDeltaSeconds / 60.f) / 60.f);
		}

		time_type Time::GetMinutes()
		{
			return static_cast<time_type>(myDeltaSeconds / 60.f);
		}

		time_type Time::GetSeconds()
		{
			return myDeltaSeconds;
		}

		time_type Time::GetMilliseconds()
		{
			return static_cast<time_type>(myDeltaSeconds * 1000.f);
		}

		time_type Time::GetMicroseconds()
		{
			return static_cast<time_type>(myDeltaSeconds / 1000000.f);
		}

		time_type Time::GetFPS()
		{
			return 1 / myDeltaSeconds;
		}

		void Time::SetTime(time_type someTime)
		{
			myDeltaSeconds = someTime;
		}
}