#pragma once
#include "include\document.h"
#include "include\filereadstream.h"
#include "..\Math\Vector\Vector.h"
typedef int FRESULT;
typedef rapidjson::Value  JSONElement;
class JSONReader
{
public:
	static std::string ERROR_STR;
	JSONReader(const std::string& filepath);
	JSONReader() = default;
	~JSONReader();

	const JSONElement& GetElement(const std::string& anElement);

	bool DocumentHasMember(const std::string& tag);
	bool ElementHasMember(const rapidjson::Value& element, const std::string& tag);

	void ReadElement(const std::string& tag, bool& out);
	void ReadElement(const std::string& tag, int& out);
	void ReadElement(const std::string& tag, unsigned int& out);
	void ReadElement(const std::string& tag, float& out);
	void ReadElement(const std::string& tag, double& out);
	void ReadElement(const std::string& tag, std::string& out);
	
	void ReadElement(const JSONElement& el, const std::string& tag, bool& out);
	void ReadElement(const JSONElement& el, const std::string& tag, int& out);
	void ReadElement(const JSONElement& el, const std::string& tag, unsigned int& out);
	void ReadElement(const JSONElement& el, const std::string& tag, float& out);
	void ReadElement(const JSONElement& el, const std::string& tag, double& out);
	void ReadElement(const JSONElement& el, const std::string& tag, std::string& out);

	
	
	std::string ReadElement(const std::string& tag);
	std::string ReadElement(const JSONElement& el, const std::string& tag);

	
	void ReadElement(const rapidjson::Value& element, CU::Vector3f& out);
	void ReadElement(const rapidjson::Value& element, CU::Vector4f& out);
	void ReadElement(const rapidjson::Value& element, float& out);


	void OpenDocument(const std::string& filepath);
	void CloseDocument();


	std::string GetCurrentFile() { return myCurrentDocumentPath; }


	std::string OptionalReadElement(const JSONElement& el, const std::string& tag);

	const rapidjson::Document& GetDocument() const { return myDocument; }

private:

	rapidjson::Document myDocument;
	rapidjson::FileReadStream* myFileReaderStream;


	std::string myCurrentDocumentPath;
	FILE* myFile;

};
