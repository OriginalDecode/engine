#pragma once
#include <DataStructures/StaticArray.h>
#include <map>

struct FT_LibraryRec_;
struct FT_FaceRec_;

struct SCharData
{
	~SCharData()
	{
		/*delete m_Data;
		m_Data = nullptr;*/
	}

	CU::Vector2f myTopLeftUV;
	CU::Vector2f myBottomRightUV;

	short myWidth;
	short myHeight;
	short m_Kerning = 0; //pixels to move back
	short myAdvanceX; //Distance to next character.
	short myBearingX;
	short myBearingY;
	char myChar;

	char* m_Data = nullptr;

};

struct SFontData
{
	~SFontData()
	{
		Engine::GetAPI()->ReleasePtr(m_AtlasView);
		delete[] myAtlas;
	}


	std::unordered_map<int, SCharData> myCharData;
	short myAtlasWidth;
	short myAtlasHeight;
	float myLineSpacing;
	float myWordSpacing;
	short myFontHeightWidth;
	int* myAtlas;

	IShaderResourceView* m_AtlasView;
};


class Texture;
class CFont;

class CFontManager
{
public:
	CFontManager();
	~CFontManager();
	void Initiate();
	CFont* LoadFont(const s8* aFontPath, u16 aFontWidth, u16 aBorderWidth);
private:
	void LoadGlyph(int index, int& x_pos, int& y_pos, int& maxY, FT_FaceRec_* aFace);

	void CalculateUV(SCharData &glyphData, int x_pos, int y_pos);

	void CreateAtlas(const int atlas_size);
	void SaveTexture(const SCharData& _char);

	FT_LibraryRec_* m_Library = nullptr;
	std::map<std::string, SFontData*> myFontData;

	SFontData* m_FontToLoad = nullptr;

};