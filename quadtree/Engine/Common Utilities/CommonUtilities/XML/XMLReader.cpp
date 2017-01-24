#include "XMLReader.h"
#include "../DL_DEBUG/DL_Debug.h"
#include"../Misc/Global.h"
namespace CommonUtilities
{
	XMLReader::XMLReader()
	{
		myDocumentIsOpen = false;
		myDocument = nullptr;
	}

	/*XMLReader::~XMLReader()
	{

	}*/

	void XMLReader::OpenDoc(const char* aFilePath)
	{
		delete myDocument;
		if (myDocument == nullptr)
		{
			myDocument = new(tinyxml2::XMLDocument());
		}

		if (aFilePath == nullptr)
		{
			DL_DEBUG("File could not be loaded! %s", aFilePath);
			DL_ASSERT_EXP(aFilePath == nullptr, "Filepath were a nullptr");
		}
		else if (myDocument->LoadFile(aFilePath) != 0)
		{
			DL_DEBUG("File could not be found! %s", aFilePath);
			DL_ASSERT("File could not be found!");
		}

		myFilePath = aFilePath;
		myDocumentIsOpen = true;
	}

	void XMLReader::CloseDoc()
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("Can't close XMLDocument!");
			DL_ASSERT("Can't close XMLDocument!")
		}
		else if (myDocumentIsOpen == true)
		{
			myDocumentIsOpen = false;
			myFilePath = "";
			delete myDocument;
			myDocument = nullptr;
		}
	}

	XMLElement XMLReader::FindFirstChildElement(const std::string& aChildName)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");
		
		return myDocument->FirstChildElement(aChildName.c_str());
	}

	XMLElement XMLReader::FindChildElement(XMLElement aParent)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");

		return aParent->FirstChildElement();
	}

	
	//Read Attribute
	
	bool XMLReader::ReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, std::string& aTargetVar)
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("XMLReader : Cannot Read Attribute, file not open!");
			DL_ASSERT("XMLReader : Cannot Read Attribute, file not open!");
		}
		if (anElementToRead == nullptr)
			return false;

		if (anElementToRead->FindAttribute(anAttribute.c_str()) != 0)
		{
			aTargetVar = anElementToRead->Attribute(anAttribute.c_str());
			return true;
		}

		return false;
	}
	
	bool XMLReader::ReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, int* aTargetVar)
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("XMLReader : Cannot Read Attribute, file not open!");
			DL_ASSERT("XMLReader : Cannot Read Attribute, file not open!");
		}
		if (anElementToRead == nullptr)
			return false;

		if (anElementToRead->QueryIntAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, double* aTargetVar)
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("XMLReader : Cannot Read Attribute, file not open!");
			DL_ASSERT("XMLReader : Cannot Read Attribute, file not open!");
		}
		if (anElementToRead == nullptr)
			return false;

		if (anElementToRead->QueryDoubleAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, float* aTargetVar)
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("XMLReader : Cannot Read Attribute, file not open!");
			DL_ASSERT("XMLReader : Cannot Read Attribute, file not open!");
		}
		if (anElementToRead == nullptr)
			return false;

		if (anElementToRead->QueryFloatAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, bool* aTargetVar)
	{

		if (myDocumentIsOpen == false)
		{
			DL_DEBUG("XMLReader : Cannot Read Attribute, file not open!");
			DL_ASSERT("XMLReader : Cannot Read Attribute, file not open!");
		}
		if (anElementToRead == nullptr)
			return false;

		if (anElementToRead->QueryBoolAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		return false;
	}


	//Force Read Attribute
	bool XMLReader::ForceReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, std::string& aTargetVar)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");


		if (anElementToRead->FindAttribute(anAttribute.c_str()) != 0)
		{
			aTargetVar = anElementToRead->Attribute(anAttribute.c_str());
			return true;
		}

		DL_DEBUG("No Attribute with %s name found!", anAttribute.c_str());
		DL_ASSERT("No Attribute found!");

		return false;
	}
	
	bool XMLReader::ForceReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, int* aTargetVar)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");


		if (anElementToRead->QueryIntAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		DL_DEBUG("No Attribute with %s name found!", anAttribute.c_str());
		DL_ASSERT("No Attribute found!");

		return false;
	}
	
	bool XMLReader::ForceReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, double* aTargetVar)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");


		if (anElementToRead->QueryDoubleAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		DL_DEBUG("No Attribute with %s name found!", anAttribute.c_str());
		DL_ASSERT("No Attribute found!");

		return false;
	}

	bool XMLReader::ForceReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, float* aTargetVar)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");


		if (anElementToRead->QueryFloatAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		DL_DEBUG("No Attribute with %s name found!", anAttribute.c_str());
		DL_ASSERT("No Attribute found!");

		return false;
	}

	bool XMLReader::ForceReadAttribute(const XMLElement anElementToRead, const std::string& anAttribute, bool* aTargetVar)
	{
		DL_ASSERT_EXP(myDocumentIsOpen == false, "XMLReader : File not open!");


		if (anElementToRead->QueryBoolAttribute(anAttribute.c_str(), aTargetVar) == tinyxml2::XML_NO_ERROR)
			return true;

		DL_DEBUG("No Attribute with %s name found!", anAttribute.c_str());
		DL_ASSERT("No Attribute found!");

		return false;
	}


}



