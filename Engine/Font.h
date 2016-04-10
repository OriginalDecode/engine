#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H




namespace Snowblind
{
	class CTexture;

	struct SCharacterData
	{
		CTexture* myTexture;
		int myWidth;
		int myHeight;
		int myXBearing;
		int myYBearing;
		int myOffsetToNextCharacter;
	};

	class Font
	{
	public:
		Font(const char* aFontPath);
		~Font();

	private:
		FT_Library myLibrary;
		FT_Face myFace;

	};
};