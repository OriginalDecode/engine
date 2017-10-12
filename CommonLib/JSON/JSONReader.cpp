#include "JSONReader.h"
#include <assert.h>
#include <DL_Debug/DL_Debug.h>
std::string JSONReader::ERROR_STR = "JSON_NO_STRING_FOUND";

JSONReader::JSONReader(const std::string& filepath)
{
	bool _override = false;
	std::string new_path;
	if (filepath.find("T_Deferred") != filepath.npos)
	{
		DL_WARNING("incorrect shader used, falling back to regular shader");
		new_path = "Data/Shaders/debug_pbl_instanced.json";
		_override = true;
	}
	//if(filepath.find("/*T_Deferred*/))
	OpenDocument(_override ? new_path : filepath);
}

JSONReader::~JSONReader()
{
	CloseDocument();
}


FRESULT JSONReader::OpenFile()
{
	FRESULT toReturn;
	fopen_s(&myFile, myCurrentDocumentPath.c_str(), "r");
	_get_errno(&toReturn);

	return toReturn;
}

void JSONReader::OpenDocument(const std::string & filepath)
{
	myCurrentDocumentPath = filepath;
	OpenFile();
	assert(myFile != NULL && "File could not be found!");
	char buffer[2048]; //the buffer size determines how fast it can parse the file
	myFileReaderStream = new rapidjson::FileReadStream(myFile, buffer, sizeof(buffer));
	myDocument.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(*myFileReaderStream);
	assert(myFile != nullptr && "File were not open. Failed to open file, does it exist?");
}

void JSONReader::CloseDocument()
{
	if (!myFile)
		return;
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	delete myFileReaderStream;
	myFileReaderStream = nullptr;
	assert(myFileReaderStream == nullptr && "Failed to remove filereaderstream!");
	fclose(myFile);
	FRESULT error;
	_get_errno(&error);
	myFile = nullptr;
	assert(myFile == nullptr && "Failed to close file.");
}

bool JSONReader::DocumentHasMember(const std::string& tag)
{
	assert(myFile != nullptr && "json file not open.");
	assert(myFileReaderStream != nullptr && "JSONReader not initiated. Reader was null!");
	assert(myDocument != 0 && "Document had no valid FileReader attatched!");

	return myDocument.HasMember(tag.c_str());
}

bool JSONReader::ElementHasMember(const rapidjson::Value& element, const std::string& tag)
{
	assert(myFile != nullptr && "json file not open.");
	assert(myFileReaderStream != nullptr && "JSONReader not initiated. Reader was null!");
	assert(myDocument != 0 && "Document had no valid FileReader attatched!");

	return element.HasMember(tag.c_str());
}

void JSONReader::ReadElement(const std::string& tag, bool& out)
{
	if (DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetBool();
}

void JSONReader::ReadElement(const std::string& tag, int& out)
{
	if(DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetInt();
}

void JSONReader::ReadElement(const std::string& tag, unsigned int& out)
{
	if (DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetUint();
}

void JSONReader::ReadElement(const std::string& tag, float& out)
{
	if (DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetDouble();
}

void JSONReader::ReadElement(const std::string& tag, double& out)
{
	if (DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetDouble();
}

void JSONReader::ReadElement(const std::string& tag, std::string& out)
{
	out = ERROR_STR;
	if (DocumentHasMember(tag))
		out = myDocument[tag.c_str()].GetString();
}

std::string JSONReader::ReadElement(const std::string& tag)
{
	std::string return_value;
	ReadElement(tag, return_value);
	return return_value;
}

void JSONReader::ReadElement(const rapidjson::Value& element, CU::Vector3f& out)
{
	out.x = (float)element[0].GetDouble();
	out.y = (float)element[1].GetDouble();
	out.z = (float)element[2].GetDouble();
}

void JSONReader::ReadElement(const rapidjson::Value& element, CU::Vector4f& out)
{
	out.x = (float)element[0].GetDouble();
	out.y = (float)element[1].GetDouble();
	out.z = (float)element[2].GetDouble();
	out.w = (float)element[3].GetDouble();
}

void JSONReader::ReadElement(const rapidjson::Value& element, float& out)
{
	out = (float)element.GetDouble();
}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, bool& out)
{
	if (ElementHasMember(el, tag))
		out = el[tag.c_str()].GetBool();
}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, int& out)
{
	if (ElementHasMember(el, tag))
		out = el[tag.c_str()].GetInt();

}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, unsigned int& out)
{
	if (ElementHasMember(el, tag))
		out = el[tag.c_str()].GetUint();

}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, float& out)
{
	if (ElementHasMember(el, tag))
		out = (float)el[tag.c_str()].GetDouble();

}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, double& out)
{
	if (ElementHasMember(el, tag))
		out = el[tag.c_str()].GetDouble();

}

void JSONReader::ReadElement(const JSONElement& el, const std::string& tag, std::string& out)
{
	if (ElementHasMember(el, tag))
		out = el[tag.c_str()].GetString();

}

std::string JSONReader::ReadElement(const JSONElement& el, const std::string& tag)
{
	assert(el.HasMember(tag.c_str()) && "Failed to find tag!");
	return el[tag.c_str()].GetString();
}

std::string JSONReader::OptionalReadElement(const JSONElement& el, const std::string& tag)
{
	//assert(el.HasMember(tag.c_str()) && "Failed to find tag!");
	if(el.HasMember(tag.c_str()))
		return el[tag.c_str()].GetString();

	return std::string();
}



const JSONElement& JSONReader::GetElement(const std::string& element_name)
{
	assert(DocumentHasMember(element_name) && "failed to find element!");
	return myDocument[element_name.c_str()];
}

