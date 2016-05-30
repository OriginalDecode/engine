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

		CU::Math::Vector2<float> myTopLeftUVBorder;
		CU::Math::Vector2<float> myBottomRightUVBorder;


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
		ID3D11ShaderResourceView* myOutlineAtlasView;
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
		void LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY
			, float atlasWidth, float atlasHeight, SFontData* aFontData, FT_FaceRec_* aFace, int borderOffset = 0);

		void LoadOutline(const int index, const int atlasX, const int atlasY
			, const float atlasWidth, SFontData* aFontData, FT_FaceRec_* aFace, int borderOffset);


		FT_LibraryRec_* myLibrary;
		const char* myFontPath;
		short myFontHeightWidth;
		std::unordered_map<std::string, SFontData*> myFontData;
	};
};

