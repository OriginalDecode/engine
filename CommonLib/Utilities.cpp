#include "Utilities.h"

namespace CL
{
	unsigned long long nearest_Pow(unsigned long long aNum)
	{
		aNum--;
		aNum |= aNum >> 1;
		aNum |= aNum >> 2;
		aNum |= aNum >> 4;
		aNum |= aNum >> 8;
		aNum |= aNum >> 16;
		aNum++;
		return aNum;
	}
}