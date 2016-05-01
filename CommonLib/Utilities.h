#pragma once
#include <string>

namespace CL
{
	unsigned long long nearest_Pow(unsigned long long aNum);
	int Color32Reverse(int x);
	//if readCharacterBeforeToFind == true it will read everything BEFORE the character/word you entered but if it is false it will read the word you entered and everything after.
	std::string substr(const std::string& aStringToReadFrom, const std::string& toFind, bool readCharactersBeforeToFind);

	struct SColor
	{
		SColor();
		SColor(unsigned int color);
		SColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
		unsigned int r;
		unsigned int g;
		unsigned int b;
		unsigned int a;
		int _color;
		void Convert(unsigned int aColor);
		void Convert(unsigned int aRed, unsigned int aGreen, unsigned int aBlue, unsigned int anAlpha);
	};
}