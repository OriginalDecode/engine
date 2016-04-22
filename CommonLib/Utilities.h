#pragma once


namespace CL
{
	unsigned long long nearest_Pow(unsigned long long aNum);
	int Color32Reverse(int x);


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