#include "DL_Debug.h"

namespace DL_Debug
{
	Debug* Debug::myInstance = nullptr;
	Debug::Debug()
	{
	}

	Debug::~Debug()
	{
	}

	bool Debug::Create(std::string aFile)
	{
		DL_ASSERT_EXP(myInstance != nullptr, "Debug file already created");
		myInstance = new DL_Debug::Debug();
	
		
		time_t now = time(0);
		struct tm tstruct;
		char buff[30];
		localtime_s(&tstruct, &now);
		strftime(buff, sizeof(buff), "%Y-%m-%d  %H_%M_%S", &tstruct); //Create a timestamp for the file, for debugg purpose.
		
		
		std::stringstream ss;
		char dirName[] = "Debugging"; //Directory name
		CreateDirectory(dirName, NULL); //Creates the Debugging directory if it doesn't exist before.
		
		ss << "Debugging\\" << aFile << "_" << buff << ".log"; //stringstream << folder << file << "seperator" << buff << "filetype";
		if (myInstance == nullptr)
		{
			std::cout << "No file created!" << std::endl;
			return (false);
		}
		myInstance->myOutputFile.open(ss.str().c_str());
		return (true);
	}

	void Debug::PrintMessage(const char *aString)
	{
		myOutputFile << aString;
		myOutputFile.flush();
	}

	void Debug::AssertMessage(const char *aFileName, int aLine, const char *aFunctionName, const char *aString)
	{
		std::stringstream sstream;

		sstream << "\n==========================" <<
			 "\nAssert at : " <<
			"\nFile: " << aFileName << 
			"\nLine: " << aLine <<
			"\nFunction: " << aFunctionName << 
			"\nMessage: " << aString;

		myOutputFile << 
			"\nAssert at: << \nFile: " << aFileName << 
			"\nLine: " << aLine <<
			"\nFunction: " << aFunctionName << 
			"\nMessage: " << aString;

		myOutputFile << "\nCallStack:\n";

		DL_Debug::StackWalker ss;
		ss.ShowCallstack();

		myOutputFile.flush();

		size_t size = strlen(sstream.str().c_str()) + 1;
		wchar_t* wa = new wchar_t[size];
		size_t tempSize;
		mbstowcs_s(&tempSize, wa, size, sstream.str().c_str(), size);

		_wassert(wa, _CRT_WIDE(__FILE__), __LINE__);
	}

	void Debug::DebugMessage(const int aLine, const char *aFileName, const char *aFormattedString, ...)
	{
		char buffer[255];
		va_list args;
		va_start(args, aFormattedString);
		vsprintf_s(buffer, aFormattedString, args);
		va_end(args);

		myOutputFile
			<< "File : " << __FILE__ << "\n"
			<< "Function : " << aFileName << "\n"
			<< "Line : " << aLine << "\n"
			<< "Message : " << buffer << "\n"
			<< "===========================\n";
			
		myOutputFile.flush();
	}

	Debug* Debug::GetInstance()
	{
		return myInstance;
	}

	bool Debug::Destroy()
	{
		myInstance->myOutputFile.close();

		myInstance = nullptr;
		delete[] myInstance;

		return (true);
	}

}
