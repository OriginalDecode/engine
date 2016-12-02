#pragma once
#include <tuple>
#include <DataStructures/GrowingArray.h>
class ShaderWarningHandler
{
public:
	ShaderWarningHandler() = default;
	std::string CheckWarning(const std::string& aWarningMessage, const std::string& aFilePath);

private:
	CU::GrowingArray<std::string> myShader;

	CU::GrowingArray<std::string> ParseFile(const std::string& filename);

	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, int charsToSkip);
	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, bool readFromFront, int charsToSkip);
	bool substr(const std::string& aStringToReadFrom, const std::string& toFind);

	u32 CountWord(char* aWord, s32 aWordLength, const std::string& aMessage);
	std::tuple<bool, std::string> CreateMessage(char* aWordToFind, s8 aWordLength, const std::string& aMessage);
	std::string myFilepath;


	bool myResult;

};

