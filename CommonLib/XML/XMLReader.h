#pragma once
#include "tinyxml2.h"
#include <string>
typedef tinyxml2::XMLElement XMLElement;
namespace cl
{
	class XMLReader
	{

	public:

		XMLReader(const char* path);
		~XMLReader();

		void OpenDoc(const char* path);
		void CloseDoc(); 

		XMLElement* FindFirstChildElement(const char* child);
		XMLElement* FindChildElement(XMLElement* parent);



		//Read Attribute
		bool ReadAttribute(const XMLElement* element, const char*  attribute, std::string&  var);
		bool ReadAttribute(const XMLElement* element, const char*  attribute, int* var);
		bool ReadAttribute(const XMLElement* element, const char*  attribute, double* var);
		bool ReadAttribute(const XMLElement* element, const char*  attribute, float* var);
		bool ReadAttribute(const XMLElement* element, const char*  attribute, bool* var);

		

	private:

		std::string m_File = "";
		tinyxml2::XMLDocument* m_Document = nullptr;
		bool m_IsOpen = false;
	};

}
