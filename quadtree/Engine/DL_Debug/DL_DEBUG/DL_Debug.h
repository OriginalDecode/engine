#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include "DL_StackWalker.h"
#include "DL_Assert.h"

//Uses __VA_ARGS__ as an expression to crash at a specific point

#define DL_ASSERT(string) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__,string);

#define DL_ASSERT_EXP(expression, string) if(expression){MessageBox(GetActiveWindow(), string "\nError has been logged!", NULL, NULL);DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__,__LINE__,__FUNCTION__,string);};

#define DL_PRINT(string)  DL_Debug::Debug::GetInstance()->PrintMessage(string);
#define DL_DEBUG( ... )  DL_Debug::Debug::GetInstance()->DebugMessage(__LINE__,__FUNCTION__,__VA_ARGS__);
namespace DL_Debug
{
	class Debug
	{
	public:
		static bool Create(std::string aFile = "Log_"); //defines the start of file.
		static bool Destroy();
		static Debug* GetInstance();
		void PrintMessage(const char *aString);
		void AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString);
		void DebugMessage(const int aLine, const char *aFileName, const char *aFormattedString, ...);
	private:
		Debug();
		~Debug();
		static Debug* myInstance;
		std::ofstream myOutputFile;
	};
};