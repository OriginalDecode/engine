#pragma once


namespace CL
{
	unsigned long long nearest_Pow(unsigned long long aNum);
	int Color32Reverse(int x);


	struct SColor
	{
		SColor();
		SColor(unsigned int color);
		SColor(int red, int green, int blue, int alpha);
		int r;
		int g;
		int b;
		int a;
		int _color;
		void Convert(unsigned int aColor);
		void Convert(unsigned int aRed, unsigned int aGreen, unsigned int aBlue, unsigned int anAlpha);
	};
}