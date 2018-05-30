#pragma once
#include "tinyxml2.h"
#include <string>
//typedef tinyxml2::XMLElement XMLElement;

namespace cl
{
	class XMLElement
	{
	public:

		XMLElement() = default;
		XMLElement(tinyxml2::XMLDocument* doc);



		typedef tinyxml2::XMLNode* iterator;
		typedef const tinyxml2::XMLNode* const_iterator;
		iterator begin() { return m_Begin; }
		const_iterator begin() const { return m_Begin; }
		iterator end() { return m_End; }
		const_iterator end() const { return m_End; }

	private:
		iterator m_Begin = nullptr;
		iterator m_End = nullptr;
	};

	class XMLReader
	{

	public:

		XMLReader(const char* path);
		~XMLReader();

		void OpenDoc(const char* path);
		void CloseDoc(); 

		tinyxml2::XMLElement* FindFirstChildElement(const char* child);
		tinyxml2::XMLElement* FirstElement();
		tinyxml2::XMLElement* GetFirstElement(tinyxml2::XMLElement* parent);

		//Find the first encounter of the element_to_find
		tinyxml2::XMLElement* FindElement(tinyxml2::XMLElement* pElement, const char* element_to_find);
		tinyxml2::XMLElement* FindElement(const char* element_to_find);

		//Read Attribute
		bool ReadAttribute(const tinyxml2::XMLElement* element, const char*  attribute, std::string&  var);
		bool ReadAttribute(const tinyxml2::XMLElement* element, const char*  attribute, int* var);
		bool ReadAttribute(const tinyxml2::XMLElement* element, const char*  attribute, double* var);
		bool ReadAttribute(const tinyxml2::XMLElement* element, const char*  attribute, float* var);
		bool ReadAttribute(const tinyxml2::XMLElement* element, const char*  attribute, bool* var);



	private:

		std::string m_File = "";
		tinyxml2::XMLDocument* m_Document = nullptr;
		bool m_IsOpen = false;

	};

}
