#pragma once

namespace Snowblind
{
	class CSystemMonitor
	{
	public:
		CSystemMonitor();
		~CSystemMonitor();

		static int GetMemoryUsage(bool inKB = false);
		static float GetCPUUsage();
	private:

		static ULONGLONG FixCPUTimings(const FILETIME &a, const FILETIME &b);

		static FILETIME myPrevSysKernel;
		static FILETIME myPrevSysUser;
		static FILETIME myPrevProcKernel;
		static FILETIME myPrevProcUser;
		static bool myFirstTime;
	};
};