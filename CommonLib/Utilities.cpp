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

	int Color32Reverse(int x)
	{
		return
			((x & 0xFF000000) >> 24) |
			((x & 0x00FF0000) >> 8) |
			((x & 0x0000FF00) << 8) |
			((x & 0x000000FF) << 24);
	}


}