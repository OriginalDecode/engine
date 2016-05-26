#include "Utilities.h"

namespace CL
{
	//if readCharacterBeforeToFind == true it will read everything BEFORE the character/word you entered but if it is false it will read the word you entered and everything after.
	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind)
	{
		std::string toCheck;
		if (aStringToReadFrom.find(toFind) != aStringToReadFrom.npos)
		{
			if (readCharactersBeforeToFind == true)
			{
				return aStringToReadFrom.substr(0, aStringToReadFrom.find(toFind));
			}
			return aStringToReadFrom.substr(aStringToReadFrom.find(toFind));
		}
		return aStringToReadFrom;
	}

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

	// AA BB GG RR
	int Color32Reverse(int x)
	{
		return
			((x & 0xFF000000) >> 24) |
			((x & 0x00FF0000) >> 8) |
			((x & 0x0000FF00) << 8) |
			((x & 0x000000FF) << 24);
	}

	SColor::SColor(unsigned int color)
		: r(0)
		, g(0)
		, b(0)
		, a(0)
		, _color(0)
	{
		a = (color)& 0xFF;
		r = (color >> 8) & 0xFF;
		g = (color >> 16) & 0xFF;
		b = (color >> 24) & 0xFF;
		_color = color;
	}

	SColor::SColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
	{
		_color = (red << 24) + (green << 16) + (blue << 8) + (alpha);
	}

	SColor::SColor()
		: r(255)
		, g(255)
		, b(255)
		, a(255)
		, _color(0)
	{
		_color = (r << 24) + (g << 16) + (b << 8) + (a);
	}

	void SColor::Convert(unsigned int aColor)
	{
		a = (aColor)& 0xFF;
		r = (aColor >> 8) & 0xFF;
		g = (aColor >> 16) & 0xFF;
		b = (aColor >> 24) & 0xFF;
		_color = aColor;
	}

	void SColor::Convert(unsigned int aRed, unsigned int aGreen, unsigned int aBlue, unsigned int anAlpha)
	{
		_color = (aRed << 24) + (aGreen << 16) + (aBlue << 8) + (anAlpha);
	}

	float RadToDegree(float aRadian)
	{
		return aRadian = aRadian * (180 / 3.1415926535f);
	}

	float DegreeToRad(float aDegree)
	{
		return aDegree = aDegree * (3.1415926535f / 180);
	}

}