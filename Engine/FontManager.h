#pragma once
#include <DataStructures/StaticArray.h>
#include <unordered_map>
struct ID3D11Device;
struct FT_LibraryRec_;
struct FT_FaceRec_;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Snowblind
{
	struct SColor
	{
		SColor(int color);
		SColor(int red, int green, int blue, int alpha);
		int r;
		int g;
		int b;
		int a;
		int _color;
	};

	struct SCharData
	{
		CU::Math::Vector2<float> myTopLeftUV;
		CU::Math::Vector2<float> myBottomRightUV;

		short myWidth;
		short myHeight;
		char myChar;
	};

	struct SFontData
	{
		~SFontData();
		ID3D11ShaderResourceView* myAtlasView;
		short myAtlasWidth; 
		short myAtlasHeight;
		int* myAtlas;
		FT_FaceRec_* myFaceData;
		std::unordered_map<char, SCharData> myCharData;
	};


	class CTexture;
	class CFont;
	class CFontManager
	{
	public:
		CFontManager();
		~CFontManager();
		void Initiate();
		CFont* LoadFont(const char* aFontPath, short aFontWidth);
	private:
		FT_LibraryRec_* myLibrary;
		const char* myFontPath;
		short myFontWidth;

	};
};

