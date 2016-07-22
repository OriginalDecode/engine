#pragma once
#include "include\document.h"
#include "include\filereadstream.h"
#include "..\Math\Vector\Vector.h"
typedef int FRESULT;
typedef rapidjson::Value  JSONElement;
class JSONReader
{
public:

	JSONReader(const std::string& aFilePath);
	~JSONReader();


	void ReadElement(const std::string& aTag, bool& aBool);
	void ReadElement(const std::string& aTag, int& anInt);
	void ReadElement(const std::string& aTag, unsigned int& anInt);
	void ReadElement(const std::string& aTag, float& aFloat);
	void ReadElement(const std::string& aTag, double& aDouble);
	void ReadElement(const std::string& aTag, std::string& aString);

	void ReadElement(const std::string& aTag, const std::string& aSubTag, bool& aBool);
	void ReadElement(const std::string& aTag, const std::string& aSubTag, int& anInt);
	void ReadElement(const std::string& aTag, const std::string& aSubTag, unsigned int& anInt);
	void ReadElement(const std::string& aTag, const std::string& aSubTag, float& aFloat);
	void ReadElement(const std::string& aTag, const std::string& aSubTag, double& aDouble);
	void ReadElement(const std::string& aTag, const std::string& aSubTag, std::string& aString);
	template<typename T>
	void ReadElement(const std::string& aTag, const std::string& aSubTag, T& aVariable);
	template<typename T>
	void ReadElement(const std::string& aTag, T& aVariable);

	template <typename T>
	void ReadElement(const std::string& aTag, const std::string& aSubTag, CU::Math::Vector3<T>& aVec3);

	template <typename T>
	void _ReadElement(const rapidjson::Value& anElement, CU::Math::Vector3<T>& aVec3);



	void ForceReadElement(const std::string& aTag, bool& aBool);
	void ForceReadElement(const std::string& aTag, int& anInt);
	void ForceReadElement(const std::string& aTag, unsigned int& anInt);
	void ForceReadElement(const std::string& aTag, float& aFloat);
	void ForceReadElement(const std::string& aTag, double& aDouble);
	void ForceReadElement(const std::string& aTag, std::string& aString);


	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, bool& aBool);
	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, int& anInt);
	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, unsigned int& anInt);
	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, float& aFloat);
	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, double& aDouble);
	void ForceReadElement(const std::string& aTag, const std::string& aSubTag, std::string& aString);

	template<typename T>
	void ReadElement(const std::string& aTag, CU::Math::Vector2<T>& aVector);
	template<typename T>
	void ReadElement(const std::string& aTag, CU::Math::Vector3<T>& aVector);
	template<typename T>
	void ReadElement(const std::string& aTag, CU::Math::Vector4<T>& aVector);

	template<typename T>
	void ForceReadElement(const std::string& aTag, CU::Math::Vector2<T>& aVector);
	template<typename T>
	void ForceReadElement(const std::string& aTag, CU::Math::Vector3<T>& aVector);
	template<typename T>
	void ForceReadElement(const std::string& aTag, CU::Math::Vector4<T>& aVector);

	const JSONElement& GetElement(const std::string& anElement);

private:
	void OpenDocument(const std::string& aFilePath);
	void CloseDocument();

	FRESULT OpenFile();
	void ValidateFileType(const std::string& aFilePath);

	template<typename T>
	const T CheckXValue(const std::string& aTag);
	template<typename T>
	const T CheckYValue(const std::string& aTag);
	template<typename T>
	const T CheckZValue(const std::string& aTag);
	template<typename T>
	const T CheckWValue(const std::string& aTag);

	rapidjson::Document myDocument;
	rapidjson::FileReadStream* myFileReaderStream;

	std::string myCurrentDocumentPath;
	FILE* myFile;

};

template<typename T>
const T JSONReader::CheckXValue(const std::string& aTag)
{
	if (myDocument[aTag.c_str()].IsArray() == false)
	{
		if (myDocument[aTag.c_str()].HasMember("X") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["X"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("x") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["x"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("R") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["R"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("r") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["r"].GetDouble());
		}
		assert(false && "Failed to find subtag. Check JSON file.");
	}

	return static_cast<T>(myDocument[aTag.c_str()][0].GetDouble());
}

template<typename T>
const T JSONReader::CheckYValue(const std::string& aTag)
{
	if (myDocument[aTag.c_str()].IsArray() == false)
	{
		if (myDocument[aTag.c_str()].HasMember("Y") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["Y"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("y") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["y"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("G") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["G"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("b") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["b"].GetDouble());
		}
		assert(false && "Failed to find subtag. Check JSON file.");
	}

	return static_cast<T>(myDocument[aTag.c_str()][1].GetDouble());
}

template<typename T>
const T JSONReader::CheckZValue(const std::string& aTag)
{
	if (myDocument[aTag.c_str()].IsArray() == false)
	{
		if (myDocument[aTag.c_str()].HasMember("Z") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["Z"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("z") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["z"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("B") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["B"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("b") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["b"].GetDouble());
		}
		assert(false && "Failed to find subtag. Check JSON file.");
	}

	return static_cast<T>(myDocument[aTag.c_str()][2].GetDouble());
}

template<typename T>
const T JSONReader::CheckWValue(const std::string& aTag)
{
	if (myDocument[aTag.c_str()].IsArray() == false)
	{
		if (myDocument[aTag.c_str()].HasMember("W") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["W"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("w") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["w"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("A") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["A"].GetDouble());
		}
		else if (myDocument[aTag.c_str()].HasMember("a") == true)
		{
			return static_cast<T>(myDocument[aTag.c_str()]["a"].GetDouble());
		}
		assert(false && "Failed to find subtag. Check JSON file.");
	}

	return static_cast<T>(myDocument[aTag.c_str()][3].GetDouble());
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, CU::Math::Vector2<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		aVector.myX = CheckXValue<T>(aTag);
		aVector.myY = CheckYValue<T>(aTag);
	}
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, CU::Math::Vector3<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		aVector.myX = CheckXValue<T>(aTag);
		aVector.myY = CheckYValue<T>(aTag);
		aVector.myZ = CheckZValue<T>(aTag);
	}
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, CU::Math::Vector4<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");

	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		aVector.myX = CheckXValue<T>(aTag);
		aVector.myY = CheckYValue<T>(aTag);
		aVector.myZ = CheckZValue<T>(aTag);
		aVector.myW = CheckWValue<T>(aTag);
	}
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, CU::Math::Vector3<T>& aVec3)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");

		const auto& data = myDocument[aTag.c_str()][aSubTag.c_str()];
		aVec3.x = data[0].GetDouble();
		aVec3.y = data[1].GetDouble();
		aVec3.z = data[2].GetDouble();

	}
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, const std::string& aSubTag, T& aVariable)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		if (myDocument[aTag.c_str()].HasMember(aSubTag.c_str()) == true)
		{
			assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
			assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");

			const auto& data = myDocument[aTag.c_str()][aSubTag.c_str()];

			for (rapidjson::SizeType i = 0; i < data.Size(); i++)
			{
				aVariable[i] = data[i].GetString();
			}
		}
	}
}

template<typename T>
void JSONReader::ReadElement(const std::string& aTag, T& aVariable)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	if (myDocument.HasMember(aTag.c_str()) == true)
	{
		assert(myFileReaderStream != nullptr && "JSONReader were not initiated. Reader were null");
		assert(myDocument != 0 && "Document had no valid FileReaderStream attached.");

		const auto& data = myDocument[aTag.c_str()];

		for (rapidjson::SizeType i = 0; i < data.Size(); i++)
		{
			aVariable[i] = data[i].GetString();
		}
	}
}

template <typename T>
void JSONReader::_ReadElement(const rapidjson::Value& anElement, CU::Math::Vector3<T>& aVec3)
{
	//const rapidjson::Value& data = anElement->value[aTag.c_str()];
	aVec3.x = anElement[0].GetDouble();
	aVec3.y = anElement[1].GetDouble();
	aVec3.z = anElement[2].GetDouble();

}

template<typename T>
void JSONReader::ForceReadElement(const std::string& aTag, CU::Math::Vector2<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	aVector.myX = CheckXValue<T>(aTag);
	aVector.myY = CheckYValue<T>(aTag);
}

template<typename T>
void JSONReader::ForceReadElement(const std::string& aTag, CU::Math::Vector3<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	aVector.myX = CheckXValue<T>(aTag);
	aVector.myY = CheckYValue<T>(aTag);
	aVector.myZ = CheckZValue<T>(aTag);
}

template<typename T>
void JSONReader::ForceReadElement(const std::string& aTag, CU::Math::Vector4<T>& aVector)
{
	assert(myFile != nullptr && "File were not open. Did you forget to OpenDocument()?");
	assert(myDocument.HasMember(aTag.c_str()) == true && "the tag were not a member of this document");
	aVector.myX = CheckXValue<T>(aTag);
	aVector.myY = CheckYValue<T>(aTag);
	aVector.myZ = CheckZValue<T>(aTag);
	aVector.myW = CheckWValue<T>(aTag);
}