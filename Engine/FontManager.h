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
struct FT_GlyphSlotRec_;



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

	ID3D11ShaderResourceView* myAtlasView;
};


class Texture;
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
	void DumpGlyph(int* source, int index, int width, int height, int pitch, bool isOutline = false);

	FT_LibraryRec_* myLibrary;
	const char* myFontPath;
	std::unordered_map<std::string, SFontData*> myFontData;

	void CalculateOutlineOffsets(const int index, FT_FaceRec_* aFace, int aBorderWidth);
	void CalculateGlyphOffsets(const int index, FT_GlyphSlotRec_* glyph);

	struct SCountData
	{
		int xNCount = 0;
		int xPCount = 0;
		int yNCount = 0;
		int yPCount = 0;
	};

	void CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData);
	void CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData);
	struct SOutlineOffset
	{
		int xDelta = 0;
		int yDelta = 0;

	} myOffset;

};