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
		short myAdvanceX; //Distance to next character.
		short myBearingX;
		short myBearingY;
		char myChar;
	};

	struct SFontData
	{
		~SFontData();
		std::unordered_map<char, SCharData> myCharData;
		short myAtlasWidth; 
		short myAtlasHeight;
		short myLineSpacing;
		short myWordSpacing;
		short myFontHeightWidth;
		int* myAtlas;
		ID3D11ShaderResourceView* myAtlasView;
		FT_FaceRec_* myFaceData;
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
	};
};

