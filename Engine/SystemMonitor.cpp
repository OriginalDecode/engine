#include "stdafx.h"
#include "SystemMonitor.h"
#include <Psapi.h>

namespace Snowblind
{
	FILETIME CSystemMonitor::myPrevSysKernel;
	FILETIME CSystemMonitor::myPrevSysUser;
	FILETIME CSystemMonitor::myPrevProcKernel;
	FILETIME CSystemMonitor::myPrevProcUser;
	bool CSystemMonitor::myFirstTime = true;

	CSystemMonitor::CSystemMonitor()
	{
	}


	CSystemMonitor::~CSystemMonitor()
	{
	}

	int CSystemMonitor::GetMemoryUsage(bool inKB)
	{
		PROCESS_MEMORY_COUNTERS memCounter;
		GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
		int toReturn = 0;

		toReturn = memCounter.WorkingSetSize / 1024 / 1024;

		if (inKB)
			toReturn = memCounter.WorkingSetSize / 1024;

		return toReturn;
	}

	float CSystemMonitor::GetCPUUsage()
	{
		FILETIME sysIdle, sysKernel, sysUser;
		FILETIME procCreation, procExit, procKernel, procUser;

		if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
			!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
		{
			// can't get time info so return
			return -1.;
		}

		// check for first call
		if (myFirstTime)
		{
			// save time info before return
			myPrevSysKernel.dwLowDateTime = sysKernel.dwLowDateTime;
			myPrevSysKernel.dwHighDateTime = sysKernel.dwHighDateTime;

			myPrevSysUser.dwLowDateTime = sysUser.dwLowDateTime;
			myPrevSysUser.dwHighDateTime = sysUser.dwHighDateTime;

			myPrevProcKernel.dwLowDateTime = procKernel.dwLowDateTime;
			myPrevProcKernel.dwHighDateTime = procKernel.dwHighDateTime;

			myPrevProcUser.dwLowDateTime = procUser.dwLowDateTime;
			myPrevProcUser.dwHighDateTime = procUser.dwHighDateTime;
			myFirstTime = false;
			return -1.;
		}

		ULONGLONG sysKernelDiff = FixCPUTimings(sysKernel, myPrevSysKernel);
		ULONGLONG sysUserDiff = FixCPUTimings(sysUser, myPrevSysUser);

		ULONGLONG procKernelDiff = FixCPUTimings(procKernel, myPrevProcKernel);
		ULONGLONG procUserDiff = FixCPUTimings(procUser, myPrevProcUser);

		ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
		ULONGLONG procTotal = procKernelDiff + procUserDiff;

		return (float)((100.0 * procTotal) / sysTotal);
	}

	ULONGLONG CSystemMonitor::FixCPUTimings(const FILETIME &a, const FILETIME &b)
	{
		LARGE_INTEGER la, lb;
		la.LowPart = a.dwLowDateTime;
		la.HighPart = a.dwHighDateTime;
		lb.LowPart = b.dwLowDateTime;
		lb.HighPart = b.dwHighDateTime;

		return la.QuadPart - lb.QuadPart;
	}
};