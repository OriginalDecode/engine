#include "API.h"

namespace Snowblind
{
	CAPI::CAPI()
	{
	}


	CAPI::~CAPI()
	{
	}

	const char* CAPI::GetAPIName() const
	{
		return myAPI;
	}
};