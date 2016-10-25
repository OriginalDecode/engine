#include "JSONReader.h"
#include <assert.h>

#define SUCCESS 0

void JSONReader::ValidateFileType(const std::string& aFilePath)
{
	if (aFilePath.rfind(".json") == std::string::npos)
		assert(false && "Invalid filetype. Must be .json");
}

FRESULT JSONReader::OpenFile()
{
	FRESULT toReturn;
	fopen_s(&myFile, myCurrentDocumentPath.c_str(), "r");
	_get_errno(&toReturn);

	return toReturn;
}

JSONReader::JSONReader(const std::string& aFilePath)
{
	OpenDocument(aFilePath);
}

JSONReader::~JSONReader()
{
	CloseDocument();
}

bool JSONReader::HasElement(const std::string& aTag)
{
	return myDocument.HasMember(aTag.c_str());
}

bool JSONReader::HasElement(const rapidjson::Value& anElement)
{
	return anElement.HasMember("");
}

void JSONReader::OpenDocument(const std::string & aFilePath)
{
	ValidateFileType(aFilePath);
	myCurrentDocumentPath = aFilePath;
	OpenFile();
	assert(myFile != NULL && "File could not be found!");
	char buffer[2048];
	myFileReaderStream = new rapidjson::FileReadStream(myFile, buffer, sizeof(buffer));
	myDocument.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(*myFileReaderStream);
	assert(myFile != nullptr && "File were not open. Failed to open file, does it exist?");
}

void JSONReader::CloseDocument()
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	delete myFileReaderStream;
	myFileReaderStream = nullptr;
	assert(myFileReaderStream == nullptr && "Failed to remove filereaderstream!");
	fclose(myFile);
	myFile = nullptr;
	assert(myFile == nullptr && "Failed to close file.");
}

void JSONReader::ReadElement(const std::string& aTag, bool& aBool)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aBool = myDocument[aTag.c_str()].GetBool();
	}
}

void JSONReader::ReadElement(const std::string& aTag, int& anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		anInt = myDocument[aTag.c_str()].GetInt();
	}
}

void JSONReader::ReadElement(const std::string& aTag, unsigned int& anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		anInt = myDocument[aTag.c_str()].GetUint();
	}
}

void JSONReader::ReadElement(const std::string& aTag, float& aFloat)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aFloat = static_cast<float>(myDocument[aTag.c_str()].GetDouble());
	}
}

void JSONReader::ReadElement(const std::string& aTag, double& aDouble)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aDouble = myDocument[aTag.c_str()].GetDouble();
	}
}

void JSONReader::ReadElement(const std::string& aTag, std::string& aString)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aString = myDocument[aTag.c_str()].GetString();
	}
}

std::string JSONReader::ReadElement(const std::string& aTag)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");

		return myDocument[aTag.c_str()].GetString();
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, bool& aBool)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aBool = myDocument[aTag.c_str()][aSubTag.c_str()].GetBool();
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, int& anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		anInt = myDocument[aTag.c_str()][aSubTag.c_str()].GetInt();
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, unsigned int& anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		anInt = myDocument[aTag.c_str()][aSubTag.c_str()].GetUint();
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, float& aFloat)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aFloat = static_cast<float>(myDocument[aTag.c_str()][aSubTag.c_str()].GetDouble());
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, double& aDouble)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aDouble = myDocument[aTag.c_str()][aSubTag.c_str()].GetDouble();
	}
}

void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, std::string& aString)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
		aString = myDocument[aTag.c_str()][aSubTag.c_str()].GetString();
	}
}

void JSONReader::ReadElement(const rapidjson::Value& anElement, float& aFloat)
{
	aFloat = anElement.GetDouble();
}



std::string JSONReader::ReadElement(const rapidjson::Value& element, int index)
{
	return element[index].GetString();
}

void JSONReader::ForceReadElement(const std::string & aTag, bool & aBool)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aBool = myDocument[aTag.c_str()].GetBool();
}

void JSONReader::ForceReadElement(const std::string & aTag, int & anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	anInt = myDocument[aTag.c_str()].GetInt();
}

void JSONReader::ForceReadElement(const std::string & aTag, unsigned int & anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	anInt = myDocument[aTag.c_str()].GetUint();
}

void JSONReader::ForceReadElement(const std::string & aTag, float & aFloat)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aFloat = static_cast<float>(myDocument[aTag.c_str()].GetDouble());
}

void JSONReader::ForceReadElement(const std::string & aTag, double & aDouble)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aDouble = myDocument[aTag.c_str()].GetDouble();
}

void JSONReader::ForceReadElement(const std::string & aTag, std::string & aString)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aString = myDocument[aTag.c_str()].GetString();
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, bool & aBool)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aBool = myDocument[aTag.c_str()][aSubTag.c_str()].GetBool();
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, int & anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	anInt = myDocument[aTag.c_str()][aSubTag.c_str()].GetInt();
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, unsigned int & anInt)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	anInt = myDocument[aTag.c_str()][aSubTag.c_str()].GetUint();
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, float & aFloat)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aFloat = static_cast<float>(myDocument[aTag.c_str()][aSubTag.c_str()].GetDouble());
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, double & aDouble)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aDouble = myDocument[aTag.c_str()][aSubTag.c_str()].GetDouble();
}

void JSONReader::ForceReadElement(const std::string & aTag, const std::string& aSubTag, std::string & aString)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	aString = myDocument[aTag.c_str()][aSubTag.c_str()].GetString();
}

const JSONElement& JSONReader::GetElement(const std::string& anElement)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(anElement.c_str()) == true && "the tag were not a member of this document");
	assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
	assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");
	return myDocument[anElement.c_str()];
}

