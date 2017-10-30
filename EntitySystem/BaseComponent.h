#pragma once
#include <CommonLib/JSON/include/writer.h>
#include <CommonLib/JSON/include/prettywriter.h>
#include <CommonLib/JSON/include/document.h>
struct BaseComponent
{
	virtual ~BaseComponent() = 0;

	
	bool m_Updated = false;
};

