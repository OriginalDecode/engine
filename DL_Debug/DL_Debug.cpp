#include "DL_Debug.h"

namespace DL_Debug
{
	Debug* Debug::myInstance = nullptr;

	Debug* Debug::GetInstance()
	{
		return myInstance;
	}

	bool Debug::Create(std::string aFile)
	{
		DL_ASSERT_EXP(myInstance == nullptr, "Debug file already created");
		myInstance = new DL_Debug::Debug();

		time_t now = time(0);
		struct tm tstruct;
		char buff[30];
		localtime_s(&tstruct, &now);
		strftime(buff, sizeof(buff), "%Y-%m-%d  %H_%M_%S", &tstruct); //Create a timestamp for the file, for debugg purpose.

		std::stringstream ss;
		char dirName[] = "Logs"; //Directory name
		CreateDirectory(dirName, NULL); //Creates the Debugging directory if it doesn't exist before.
#ifdef _DEBUG
		ss << "Logs\\" << aFile << "_" << buff << "_Debug" << ".log"; //stringstream << folder << file << "seperator" << buff << "filetype";
#else 
		ss << "Logs\\" << aFile << "_" << buff << "_Release" << ".log"; //stringstream << folder << file << "seperator" << buff << "filetype";
#endif
		if (myInstance == nullptr)
		{
			std::cout << "No file created!" << std::endl;
			return (false);
		}

		myInstance->myOutputFile.open(ss.str().c_str());
		return (true);
	}

	bool Debug::Destroy()
	{
		myInstance->myOutputFile.close();
		delete myInstance;
		myInstance = nullptr;
		return true;
	}

	Debug::Debug()
	{
		myDebugLogs.set(DEBUGLOG::Update, true);
		myDebugLogs.set(DEBUGLOG::Render, true);
		myDebugLogs.set(DEBUGLOG::Physics, true);
		myDebugLogs.set(DEBUGLOG::Resource, true);
		myDebugLogs.set(DEBUGLOG::Engine, true);
	}

	Debug::~Debug()
	{
	}

	void Debug::PrintMessage(const char *aString)
	{
		myOutputFile << AddTime() << aString << "\n";
		myOutputFile.flush();
	}

	void Debug::AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString)
	{
		std::stringstream sstream;

		sstream << "\nAssert at : " <<
			"\nFile: " << aFileName <<
			"\nLine: " << aLine <<
			"\nFunction: " << aFunctionName <<
			"\nMessage: " << aString;

		myOutputFile <<
			"\n" << AddTime() << "Assert at: " <<
			"\n" << AddTime() << "File: " << aFileName <<
			"\n" << AddTime() << "Line: " << aLine <<
			"\n" << AddTime() << "Function: " << aFunctionName <<
			"\n" << AddTime() << "Message: " << aString;

		myOutputFile << "\nCallStack:\n";

		DL_Debug::StackWalker ss;
		ss.ShowCallstack();

		myOutputFile << "\nCallStack:\n";
		myOutputFile << "\n==========================";
		myOutputFile.flush();


		time_t now = time(0);
		struct tm tstruct;
		char buff[30];
		localtime_s(&tstruct, &now);
		strftime(buff, sizeof(buff), "%Y-%m-%d  %H_%M_%S", &tstruct);

		size_t size = strlen(sstream.str().c_str()) + 1;
		wchar_t* wa = new wchar_t[size];
		size_t tempSize;
		mbstowcs_s(&tempSize, wa, size, sstream.str().c_str(), size);

		_wassert(wa, _CRT_WIDE(__FILE__), __LINE__);
	}

	void Debug::DebugMessage(const int aLine, const char *aFileName, const std::string& aString)
	{
		myOutputFile
			<< AddTime() << "==========DEBUG MESSAGE==========\n" 
			<< AddTime() << "File : " << __FILE__ << "\n"
			<< AddTime() << "Function : " << aFileName << "\n"
			<< AddTime() << "Line : " << aLine << "\n"
			<< AddTime() << "[Message] : " << aString << "\n"
			<< AddTime() << "=================================\n";

		myOutputFile.flush();
	}

	void Debug::WriteLog(const std::string& aFilter, const std::string& aString)
	{
		if (aFilter == "Engine" && myDebugLogs[DEBUGLOG::Engine] == 1)
			return;

		if (aFilter == "Update" && myDebugLogs[DEBUGLOG::Update] == 1)
			return;

		if (aFilter == "Render" && myDebugLogs[DEBUGLOG::Render] == 1)
			return;

		if (aFilter == "Resource" && myDebugLogs[DEBUGLOG::Resource] == 1)
			return;

		if (aFilter == "PhysX" && myDebugLogs[DEBUGLOG::Physics] == 1)
			return;

		myOutputFile
			<< AddTime() << "[" << aFilter << "]" << " : " << aString << "\n";

		myOutputFile.flush();

	}

	void Debug::DisableFilters(const DEBUGLOG& anEnum)
	{
		switch (anEnum)
		{
		case DEBUGLOG::Update:
			myDebugLogs[DEBUGLOG::Update] = 1;
			break;
		case DEBUGLOG::Render:
			myDebugLogs[DEBUGLOG::Render] = 1;
			break;
		case DEBUGLOG::Physics:
			myDebugLogs[DEBUGLOG::Physics] = 1;
			break;
		case DEBUGLOG::Resource:
			myDebugLogs[DEBUGLOG::Resource] = 1;
			break;
		case DEBUGLOG::Engine:
			myDebugLogs[DEBUGLOG::Engine] = 1;
			break;
		}
	}

	void Debug::ActivateFilter(const DEBUGLOG& anEnum)
	{
		switch (anEnum)
		{
		case DEBUGLOG::Update:
			myDebugLogs[DEBUGLOG::Update] = 0;
			break;
		case DEBUGLOG::Render:
			myDebugLogs[DEBUGLOG::Render] = 0;
			break;
		case DEBUGLOG::Physics:
			myDebugLogs[DEBUGLOG::Physics] = 0;
			break;
		case DEBUGLOG::Resource:
			myDebugLogs[DEBUGLOG::Resource] = 0;
			break;
		case DEBUGLOG::Engine:
			myDebugLogs[DEBUGLOG::Engine] = 0;
			break;
		}
	}

	std::string Debug::AddTime()
	{
		time_t now = time(0);
		struct tm tstruct;
		char buff[30];
		localtime_s(&tstruct, &now);
		strftime(buff, sizeof(buff), "[%H:%M:%S]", &tstruct);

		std::string toReturn = buff;

		return toReturn;
	}

	std::string Debug::HandleVAArgs(const char* aFormattedString, ...)
	{
		char buffer[255];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
		va_end(args);

		std::string toReturn = buffer;

		return toReturn;
	}

	const bool Debug::CheckFilter(const DEBUGLOG& aFilter)
	{
		switch (aFilter)
		{
		case DEBUGLOG::Engine:
			if (myDebugLogs[DEBUGLOG::Engine] == 0)
				return true;
			break;
		case DEBUGLOG::Physics:
			if (myDebugLogs[DEBUGLOG::Physics] == 0)
				return true;
			break;
		case DEBUGLOG::Render:
			if (myDebugLogs[DEBUGLOG::Render] == 0)
				return true;
			break;
		case DEBUGLOG::Resource:
			if (myDebugLogs[DEBUGLOG::Resource] == 0)
				return true;
			break;
		case DEBUGLOG::Update:
			if (myDebugLogs[DEBUGLOG::Update] == 0)
				return true;
			break;
		}

		return false;
	}

}
