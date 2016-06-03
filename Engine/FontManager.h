#pragma once
#include <DataStructures/StaticArray.h>
#include <unordered_map>
struct ID3D11Device;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

struct FT_LibraryRec_;
struct FT_FaceRec_;
struct FT_GlyphRec_;


namespace Snowblind
{

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
		float myLineSpacing;
		short myWordSpacing;
		short myFontHeightWidth;
		int* myAtlas;
		int* myOutlineAtlas;

		ID3D11ShaderResourceView* myAtlasView;
	};

	class CTexture;
	class CFont;

	class CFontManager
	{
	public:
		CFontManager();
		~CFontManager();
		void Initiate();
		CFont* LoadFont(const char* aFontPath, short aFontWidth, int aBorderWidth);
	private:
		void LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY
			, float atlasWidth, float atlasHeight, SFontData* aFontData, FT_FaceRec_* aFace, int aBorderWidth = 0);

		void LoadOutline(const int index, const int atlasX, const int atlasY
			, const float atlasWidth, SFontData* aFontData, FT_FaceRec_* aFace, int aBorderWidth);


		void DumpAtlas(SFontData* fontData, int atlasSize);
		void DumpGlyph(int* source, int index, int width, int height, int pitch);

		FT_LibraryRec_* myLibrary;
		const char* myFontPath;
		std::unordered_map<std::string, SFontData*> myFontData;
	};
};

