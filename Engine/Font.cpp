#include "stdafx.h"
#include "Font.h"
#include <math.h>

namespace Snowblind
{
	Font::Font(const char* aFontPath)
	{
		FT_Error error = FT_Init_FreeType(&myLibrary);
		if (error)
		{
			//an error occured during library init.
		}

		error = FT_New_Face(myLibrary, aFontPath, 0, &myFace);

	/*	FT_Set_Char_Size(myFace, 0, 16 * 64, 300, 300);
		FT_Set_Pixel_Sizes(myFace, 0, 16);*/

		FT_Set_Pixel_Sizes(myFace, 0, 16);
		if (FT_Load_Char(myFace, 'X', FT_LOAD_RENDER))
		{
			//error
		}



	}

	Font::~Font()
	{
		FT_Done_Face(myFace);
		FT_Done_FreeType(myLibrary);
	}
};