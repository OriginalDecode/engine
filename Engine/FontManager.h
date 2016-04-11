#pragma once
#include <DataStructures/StaticArray.h>
struct ID3D11Device;
struct FT_LibraryRec_;
struct FT_FaceRec_;
#define CHAR_COUNT 256

namespace Snowblind
{
	class CTexture;
	class CFontManager
	{
	public:
		CFontManager();
		~CFontManager();
		void Initiate();
		void LoadFont(const char* aFontPath, short aFontHeight);
	private:

		ID3D11Device* myDevice;
		FT_LibraryRec_* myLibrary;
		FT_FaceRec_* myFace;
		const char* myFontPath;
		short myFontHeight;
		CU::StaticArray<CTexture*, CHAR_COUNT> myTextures;




	};
};