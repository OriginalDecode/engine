#include "DL_Debug.h"
#include <time.h>
#include "../Engine/snowblind_shared.h"
Ticket_Mutex dlDebug_Mutex;
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
		strftime(buff, sizeof(buff), "%Y-%m-%d  %H_%M_%S", &tstruct); //Create a timestamp for the file, for debug purpose.

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
			return false;
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
		m_LogFlags |= Warning_Filter;
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
		myOutputFile << "\nCallStack:\n";
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

		myOutputFile << "\nCallStack:";

		DL_Debug::StackWalker ss;
		ss.ShowCallstack();

		myOutputFile << "_________________________";
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
			<< AddTime() << "______DEBUG MESSAGE______\n" 
			<< AddTime() << "File : " << __FILE__ << "\n"
			<< AddTime() << "Function : " << aFileName << "\n"
			<< AddTime() << "Line : " << aLine << "\n"
			<< AddTime() << "[Message] : " << aString << "\n"
			<< AddTime() << "_______________\n";

		myOutputFile.flush();
	}

	void Debug::WriteLog(int filter_flag, const std::string& aString)
	{
		std::string filter_tag;

		if (filter_flag != 0)
		{
			if (m_LogFlags & filter_flag)
			{
				if (filter_flag & Engine_Filter)
					filter_tag = "Engine";
				else if (filter_flag & Font_Filter)
					filter_tag = "Font";
				else if (filter_flag & Model_Filter)
					filter_tag = "Model";
				else if (filter_flag & Physics_Filter)
					filter_tag = "Physics";
				else if (filter_flag & Render_Filter)
					filter_tag = "Model";
				else if (filter_flag & Resource_Filter)
					filter_tag = "Resource";
				else if (filter_flag & Update_Filter)
					filter_tag = "Update";
				else if (filter_flag & Warning_Filter)
					filter_tag = "Warning";
			}
		}
		else
		{
			filter_tag = "Message";
		}
		std::string str(aString.begin(), aString.end());

		BeginTicketMutex(&dlDebug_Mutex);

		myOutputFile << AddTime() << "[" << filter_tag << "]" << " : " << str << "\n";
		myOutputFile.flush();

		EndTicketMutex(&dlDebug_Mutex);

	}

	void Debug::WriteLogNoTimeStamp(const std::string& string)
	{
		BeginTicketMutex(&dlDebug_Mutex);
		std::string str(string.begin(), string.end());
		myOutputFile << str << "\n";
		myOutputFile.flush();
		EndTicketMutex(&dlDebug_Mutex);
	}

	void Debug::DisableFilters(int flags)
	{
		m_LogFlags &= ~flags;
	}

	void Debug::ActivateFilters(int flags)
	{
		m_LogFlags |= flags;
	}

	bool Debug::CheckFilter(int flags)
	{
		return m_LogFlags & flags;
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
		char buffer[255 * 16];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
		va_end(args);

		std::string toReturn = buffer;

		return toReturn;
	}


}
