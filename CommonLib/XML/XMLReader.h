#pragma once
#include "tinyxml2.h"
#include <string>
#define XMLElement tinyxml2::XMLElement*
namespace CommonUtilities
{
	class XMLReader
	{

	public:

		XMLReader();
		~XMLReader();

		void OpenDoc(const char*  aFilePath);
		void CloseDoc(); 

		XMLElement FindFirstChildElement(const std::string&  aChildName);
		XMLElement FindChildElement(XMLElement aParent);
		
		XMLElement ForceFindFirstChildElement(const std::string&  aChildName) const;
		XMLElement ForceFindFirstChildElement(XMLElement aParent, const std::string&  aChildName) const;



		//Read Attribute
		bool ReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, std::string&  aTargetVar);
		bool ReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, int* aTargetVar);
		bool ReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, double* aTargetVar);
		bool ReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, float* aTargetVar);
		bool ReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, bool* aTargetVar);

		//Force Read Attribute
		bool ForceReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, std::string&  aTargetVar);
		bool ForceReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, int* aTargetVar);
		bool ForceReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, double* aTargetVar);
		bool ForceReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, float* aTargetVar);
		bool ForceReadAttribute(const XMLElement anElementToRead, const std::string&  anAttribute, bool* aTargetVar);

	private:

		tinyxml2::XMLDocument* myDocument;

		bool myDocumentIsOpen;
		std::string myFilePath;

	};

}
namespace CU = CommonUtilities;