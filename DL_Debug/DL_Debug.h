#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <bitset>

#include "DL_StackWalker.h"
#include "DL_Assert.h"
//Uses __VA_ARGS__ as an expression to crash at a specific point
#define RETURN(...) DL_Debug::Debug::GetInstance()->HandleVAArgs(__VA_ARGS__)

#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__,string);

#define DL_ASSERT_EXP(expression, string) if(expression){DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__,string);};

#define DL_PRINT(string)  DL_Debug::Debug::GetInstance()->PrintMessage(string);
#define DL_DEBUG( ... )  DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__,__FUNCTION__, RETURN(__VA_ARGS__));

#define DL_WRITELOG(log, ...) DL_Debug::Debug::GetInstance()->WriteLog(log , RETURN(__VA_ARGS__));

#define DL_MESSAGE(...) DL_Debug::Debug::GetInstance()->WriteLog("Message" , RETURN(__VA_ARGS__));
#define DL_MESSAGE_EXP(expression, ...) if(expression){DL_Debug::Debug::GetInstance()->WriteLog("Message", RETURN(__VA_ARGS__));};


#define RENDER_LOG(...)		DL_WRITELOG("Render",	__VA_ARGS__)
#define UPDATE_LOG(...)		DL_WRITELOG("Update",	__VA_ARGS__)
#define PHYSX_LOG(...)		DL_WRITELOG("Physics",	__VA_ARGS__)
#define ENGINE_LOG(...)		DL_WRITELOG("Engine",	__VA_ARGS__)
#define RESOURCE_LOG(...)	DL_WRITELOG("Resource", __VA_ARGS__)

//expressions
#define ENGINE_LOG_EXP(expression, ...)		if(expression) { DL_WRITELOG("Engine",__VA_ARGS__)}

namespace DL_Debug
{
	class Debug
	{
	public:
		enum DEBUGLOG
		{
			Update,
			Render,
			Physics,
			Resource,
			Engine,
			_COUNT
		};

		static bool Create(std::string aFile = "Log_"); //defines the start of file.
		static bool Destroy();
		static Debug* GetInstance();
		
		void PrintMessage(const char *aString);
		void AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString);
		void DebugMessage(const int aLine, const char *aFileName, const std::string& aString);
		void WriteLog(const std::string& aFilter, const std::string& aString);

		void DisableFilters(const DEBUGLOG& anEnum);
		void ActivateFilter(const DEBUGLOG& anEnum);
		const bool CheckFilter(const DEBUGLOG& aFilter);

		const float GetTime() const;
		std::string HandleVAArgs(const char* aFormattedString, ...);

	private:
		Debug();
		~Debug();

		std::string AddTime();

		static Debug* myInstance;
		std::ofstream myOutputFile;

		bool myEngineDebug;
		bool myRenderDebug;
		bool myUpdateDebug;
		bool myPhysXDebug;
		bool myResourceDebug;

		std::bitset<5> myDebugLogs;
	};
};