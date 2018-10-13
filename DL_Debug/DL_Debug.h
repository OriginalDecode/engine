#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

#include "DL_StackWalker.h"
#include "DL_Assert.h"
//Uses __VA_ARGS__ as an expression to crash at a specific point


enum eDEBUGLOG
{
	Update_Filter = 1,
	Render_Filter = 2,
	Physics_Filter = 4,
	Resource_Filter = 8,
	Engine_Filter = 16,
	Font_Filter = 32,
	Model_Filter = 64,
	Warning_Filter = 128,
	Network_Filter = 256,
};
#
#ifdef _DEBUG
#define RETURN(...) DL_Debug::Debug::GetInstance()->HandleVAArgs(__VA_ARGS__)

#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__, __LINE__, __FUNCSIG__ , string);

#define ASSERT(expression, string)if(expression == false){DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__, string); }

#define DL_PRINT(string)  DL_Debug::Debug::GetInstance()->PrintMessage(string);
//#define DL_DEBUG( ... )  DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__,__FUNCTION__, RETURN(__VA_ARGS__));

#define DL_WRITELOG(log, ...) DL_Debug::Debug::GetInstance()->WriteLog(log , RETURN(__VA_ARGS__));
#define TRACE_LOG(...) DL_Debug::Debug::GetInstance()->WriteLogNoTimeStamp(RETURN(__VA_ARGS__));


#define DL_MESSAGE(...) DL_Debug::Debug::GetInstance()->WriteLog(0, RETURN(__VA_ARGS__));
#define DL_MESSAGE_EXP(expression, ...) if(expression){DL_Debug::Debug::GetInstance()->WriteLog(0, RETURN(__VA_ARGS__));};

#define DL_WARNINGBOX(msg) MessageBox(NULL, msg,"Warning!", MB_ICONWARNING)
#define DL_WARNINGBOX_EXP(expression, msg) if(expression){MessageBox(NULL, msg,"Warning!", MB_ICONWARNING);};

#define DL_WARNING(...) DL_WRITELOG(Warning_Filter, __VA_ARGS__)


#define RENDER_LOG(...)		DL_WRITELOG( Render_Filter,		__VA_ARGS__)
#define UPDATE_LOG(...)		DL_WRITELOG( Update_Filter,		__VA_ARGS__)
#define PHYSX_LOG(...)		DL_WRITELOG( Physics_Filter,	__VA_ARGS__)
#define ENGINE_LOG(...)		DL_WRITELOG( Engine_Filter,		__VA_ARGS__)
#define RESOURCE_LOG(...)	DL_WRITELOG( Resource_Filter,	__VA_ARGS__)
#define FONT_LOG(...)		DL_WRITELOG( Font_Filter,		__VA_ARGS__)
#define MODEL_LOG(...)		DL_WRITELOG( Model_Filter,		__VA_ARGS__)
#define LOG_NETWORK(...)	DL_WRITELOG( Network_Filter,	__VA_ARGS__)

#define ALGORITHM_LOG(...)  DL_WRITELOG("Algorithm", __VA_ARGS__)
#define ALGORITHM_LOG_EXP(expression, ...) if(expression){ DL_WRITELOG("Algorithm", __VA_ARGS__)}

//expressions
#define ENGINE_LOG_EXP(expression, ...)		if(expression) { DL_WRITELOG("Engine",__VA_ARGS__)}
#else
#define RETURN(...) DL_Debug::Debug::GetInstance()->HandleVAArgs(__VA_ARGS__)

#define DL_ASSERT(string)

#define ASSERT(expression, string)

#define DL_PRINT(string) 
//#define DL_DEBUG( ... )  

#define DL_WRITELOG(log, ...) 

#define DL_MESSAGE(...) DL_Debug::Debug::GetInstance()->WriteLog(0, RETURN(__VA_ARGS__));
#define DL_MESSAGE_EXP(expression, ...) expression

#define DL_WARNINGBOX(msg) 
#define DL_WARNINGBOX_EXP(expression, msg) expression

#define DL_WARNING(...) DL_WRITELOG("WARNING", __VA_ARGS__)

#define RENDER_LOG(...)		
#define UPDATE_LOG(...)		
#define PHYSX_LOG(...)		
#define ENGINE_LOG(...)		
#define RESOURCE_LOG(...)	
#define FONT_LOG(...)
#define ALGORITHM_LOG(...)
#define MODEL_LOG(...)
#define TRACE_LOG(...)
//expressions
#define ENGINE_LOG_EXP(expression, ...)		
#define ALGORITHM_LOG_EXP(expression, ...)
#define LOG_NETWORK(...)
#endif


namespace DL_Debug
{
	class Debug
	{
	public:
	

		static bool Create(std::string aFile = "Log"); //defines the start of file.
		static bool Destroy();
		static Debug* GetInstance();

		void PrintMessage(const char *aString);
		void AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString);

		void DebugMessage(const int aLine, const char *aFileName, const std::string& aString);
		void WriteLog(int filter_flag, const std::string& aString);
		void WriteLogNoTimeStamp(const std::string& string);

		void DisableFilters(int flags);
		void ActivateFilters(int flags);
		bool CheckFilter(int flags);

		std::string HandleVAArgs(const char* aFormattedString, ...);
	private:
		Debug();
		~Debug();

		std::string AddTime();

		static Debug* myInstance;
		std::ofstream myOutputFile;

		int m_LogFlags = 0;
	};
};