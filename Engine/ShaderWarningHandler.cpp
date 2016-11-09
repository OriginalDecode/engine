#include "stdafx.h"
#include "ShaderWarningHandler.h"
#include <string>
#include <fstream>
#include <algorithm>

ShaderWarningHandler::ShaderWarningHandler()
{
}


ShaderWarningHandler::~ShaderWarningHandler()
{
}

std::string ShaderWarningHandler::CheckWarning(const std::string& aWarningMessage, const std::string& aFilePath)
{
	myFilepath = aFilePath;
	s8 toFind[7] = { 'w', 'a', 'r', 'n','i','n','g' };
	s8 toFind2[5] = { 'e','r','r','o','r' };

	s8 toFindLen = 7;
	s8 toFindLen2 = 5;
	auto warn = CreateMessage(toFind, toFindLen, aWarningMessage);
	std::string toReturn = std::get<1>(warn);
	if (!std::get<0>(warn))
	{
		auto err = CreateMessage(toFind2, toFindLen2, aWarningMessage);
		toReturn = std::get<1>(err);
	}
	return toReturn;
}

CU::GrowingArray<std::string> ShaderWarningHandler::ParseFile(const std::string& filename)
{
	std::ifstream stream(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = stream.tellg();

	std::vector<char> result(pos);
	CU::GrowingArray<std::string> file;
	stream.seekg(0, std::ios::beg);
	stream.read(&result[0], pos);
	stream.close();

	for (int i = 0; i < result.size(); i++)
	{
		std::string toAdd;
		for (int j = i; j < result.size(); j++, i++)
		{
			if (result[j] == '\n')
				break;

			if (result[j] != '\t' && result[j] != '\r')
				toAdd += result[j];
		}

		file.Add(toAdd);
	}
	return file;
}

u32 ShaderWarningHandler::CountWord(char* aWord, s32 aWordLength, const std::string& aMessage)
{
	s32 strLen = s32(aMessage.length());
	s32 i = 0;
	s32 j = 0;
	s32 wordCount = 0;
	while (i < strLen)
	{
		if (aMessage[i] == aWord[0])
		{
			for (j = 0; j < aWordLength; j++)
			{
				if (aMessage[i + j] != aWord[j])
				{
					i += j;
					j = 0;
					break;
				}
			}
			if (j == aWordLength)
			{
				i += j;
				j = 0;
				wordCount++;
			}
		}
		else
			i++;
	}

	return wordCount;
}

std::tuple<bool, std::string> ShaderWarningHandler::CreateMessage(char* aWordToFind, s8 aWordLength, const std::string& aMessage)
{

	std::tuple<std::pair<bool, std::string>> toReturn;

	u32 wordCount = CountWord(aWordToFind, aWordLength, aMessage);
	if (wordCount > 0)
	{
		CU::GrowingArray<u32> warningLines;
		std::string toCut = aMessage;
		for (u32 m = 0; m < wordCount; m++)
		{
			toCut = substr(toCut, aWordToFind, true, 0);
			std::string split2 = substr(toCut, "(", false, 0);
			std::string lineNo = substr(split2, ",", true, 0);
			lineNo.erase(0, 1);
			warningLines.Add(std::atoi(lineNo.c_str()));
		}

		std::sort(warningLines.begin(), warningLines.end());

		myShader = ParseFile(myFilepath);

		std::string outPath = substr(myFilepath, "/", false, 0);
		outPath.erase(0, 1);
		std::string errorMessage(outPath);
		errorMessage += " has generated warnings.\n";
		for (s32 l = 0; l < warningLines.Size(); l++)
		{
			if (l - 1 > 0)
			{
				std::string shaderLine = myShader[warningLines[l] - 1];
				if (!substr(shaderLine, ";"))
				{
					shaderLine += myShader[warningLines[l]];
				}

				errorMessage += shaderLine + "\n";
			}
		}

		return std::make_tuple(true, errorMessage.c_str());
	}

	return std::make_tuple(false, aMessage.c_str());
}


/* Substring */

std::string ShaderWarningHandler::substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, int charsToSkip)
{
	std::string toCheck;
	if (aStringToReadFrom.rfind(toFind) != aStringToReadFrom.npos)
	{
		if (readCharactersBeforeToFind == true)
		{
			return aStringToReadFrom.substr(charsToSkip, aStringToReadFrom.rfind(toFind));
		}
		return aStringToReadFrom.substr(aStringToReadFrom.rfind(toFind));
	}
	return aStringToReadFrom;
}

bool ShaderWarningHandler::substr(const std::string& aStringToReadFrom, const std::string& toFind)
{
	if (aStringToReadFrom.rfind(toFind) != aStringToReadFrom.npos)
	{
		return true;
	}
	return false;
}

std::string ShaderWarningHandler::substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind, bool readFromFront, int charsToSkip)
{
	if (readFromFront == false)
		return substr(aStringToReadFrom, toFind, readCharactersBeforeToFind, charsToSkip);

	std::string toCheck;
	if (aStringToReadFrom.find(toFind) != aStringToReadFrom.npos)
	{
		if (readCharactersBeforeToFind == true)
		{
			return aStringToReadFrom.substr(charsToSkip, aStringToReadFrom.find(toFind));
		}
		return aStringToReadFrom.substr(aStringToReadFrom.find(toFind));
	}
	return aStringToReadFrom;
}

