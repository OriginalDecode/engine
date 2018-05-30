#include "XMLReader.h"
#include <DL_Debug/DL_Debug.h>
#include <string>
namespace cl
{
	XMLReader::XMLReader(const char* path)
	{
		OpenDoc(path);
	}

	XMLReader::~XMLReader()
	{
		CloseDoc();
	}

	void XMLReader::OpenDoc(const char* path)
	{
		delete m_Document;
		m_Document = new tinyxml2::XMLDocument;

		tinyxml2::XMLError err = m_Document->LoadFile(path);
		ASSERT(err  == 0, "An error occurred while loading the XML file");
		m_File = path;
		m_IsOpen = true;
	}

	void XMLReader::CloseDoc()
	{
		delete m_Document;
		m_Document = nullptr;
		m_IsOpen = false;
	}

	tinyxml2::XMLElement* XMLReader::FindFirstChildElement(const char* child)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		return m_Document->FirstChildElement(child);
	}

	tinyxml2::XMLElement* XMLReader::GetFirstElement(tinyxml2::XMLElement* parent)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		return parent->FirstChildElement();
	}

	tinyxml2::XMLElement* XMLReader::FindElement(tinyxml2::XMLElement* pElement, const char* element_to_find)
	{
		if (tinyxml2::XMLElement* element = pElement->FirstChildElement(element_to_find))
		{
			return element;
		}

		for (auto* e = pElement->FirstChildElement(); e; e = pElement->NextSiblingElement())
		{
			if (tinyxml2::XMLElement* element = e->FirstChildElement(element_to_find))
			{
				return element;
			}

		}

		return nullptr;

	}

	tinyxml2::XMLElement* XMLReader::FindElement(const char* element_to_find)
	{
		return FindElement(FirstElement(), element_to_find);
	}

	tinyxml2::XMLElement* XMLReader::FirstElement()
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		return m_Document->FirstChildElement();
	}

	bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* element, const char* attribute, bool* var)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		ASSERT(element->FindAttribute(attribute), "XMLReader : Failed to find attribute");

		if (element->QueryBoolAttribute(attribute, var) == tinyxml2::XML_SUCCESS)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* element, const char* attribute, float* var)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		ASSERT(element->FindAttribute(attribute), "XMLReader : Failed to find attribute");

		if (element->QueryFloatAttribute(attribute, var) == tinyxml2::XML_SUCCESS)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* element, const char* attribute, double* var)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		ASSERT(element->FindAttribute(attribute), "XMLReader : Failed to find attribute");

		if (element->QueryDoubleAttribute(attribute, var) == tinyxml2::XML_SUCCESS)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* element, const char* attribute, int* var)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		ASSERT(element->FindAttribute(attribute), "XMLReader : Failed to find attribute");
		if (element->QueryIntAttribute(attribute, var) == tinyxml2::XML_SUCCESS)
			return true;

		return false;
	}

	bool XMLReader::ReadAttribute(const tinyxml2::XMLElement* element, const char* attribute, std::string& var)
	{
		ASSERT(m_IsOpen, "XMLReader : File not open!");
		ASSERT(element->FindAttribute(attribute), "XMLReader : Failed to find attribute");
		var = element->Attribute(attribute);
		return true;
	}
}
