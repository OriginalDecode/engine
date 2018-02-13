#include "stdafx.h"
#include "FontManager.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include FT_STROKER_H
#include FT_OUTLINE_H

#include <Utilities.h>
#include "EngineDefines.h"
#include "Font.h"
#define COBJMACROS
#include <d3d11.h>
#include "Engine.h"
#include <vector>
#include <ScreenGrab.h>
//#define SAVE
#ifdef SAVE
#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
#endif
#endif


#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsDevice.h>

constexpr s32 x_offset = 8;
constexpr s32 y_offset = 8;
constexpr s32 x_start = 0;

constexpr s32 multiple_baseline = 64;
constexpr s32 dpi = 96;

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	myFontPath = nullptr;
	FT_Done_FreeType(myLibrary);
	for (auto it = myFontData.begin(); it != myFontData.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
}

void CFontManager::Initiate()
{
	int error = FT_Init_FreeType(&myLibrary);
	DL_ASSERT_EXP(!error, "Failed to initiate FreeType.");
}

CFont* CFontManager::LoadFont(const s8* aFontPath, u16 aSize, u16 aBorderWidth)
{
	std::string fontFolder = aFontPath;
	if (!cl::substr(aFontPath, "/"))
	{
		fontFolder = "";
		TCHAR dir[32];
		GetSystemDirectory(dir, 32);
		fontFolder += dir;
		fontFolder = cl::substr(fontFolder, "\\", true, 0);
		fontFolder += "\\Fonts\\";
		fontFolder += aFontPath;
	}

	u16 font_width = aSize;
	int atlasSize = (font_width * font_width) * dpi;
	atlasSize = int(cl::nearest_Pow_Under(atlasSize));
	float atlasWidth = (float)atlasSize; //static_cast<float>(atlasSize) / 2;
	float atlasHeight = (float)atlasSize; // static_cast<float>(atlasSize) / 2;
	FONT_LOG("Font Size W: %d H: %d", (s32)atlasWidth, (s32)atlasHeight);

	std::stringstream key;
	key << aFontPath << "-" << font_width;
	SFontData* fontData = nullptr;

	if (myFontData.find(key.str()) == myFontData.end())
	{
		fontData = new SFontData;
		myFontData[key.str()] = fontData;
	}
	else
	{
		fontData = myFontData[key.str()];
		FONT_LOG("Font Data already found, creating Font Object with existing font data! %s", key.str().c_str());
		return new CFont(fontData);
	}

	const int total_size = (atlasWidth * atlasHeight);
	fontData->myAtlas = new int[total_size];
	ZeroMemory(fontData->myAtlas, total_size * sizeof(int));

	fontData->myFontHeightWidth = font_width;
	myFontPath = fontFolder.c_str();

	FT_Face face;
	FT_Error error = FT_New_Face(myLibrary, myFontPath, 0, &face);
	FONT_LOG("Loading font:%s", myFontPath);
	DL_ASSERT_EXP(!error, "Failed to load requested font.");

	FT_F26Dot6 ftSize = (FT_F26Dot6)(fontData->myFontHeightWidth * multiple_baseline);
	error = FT_Set_Char_Size(face, ftSize, 0, dpi * multiple_baseline, 0); // 96 = 100% scaling in Windows. 
	DL_ASSERT_EXP(!error, "[FontManager] : Failed to set pixel size!");

	int atlasX = x_start;
	int atlasY = 0;
	int currentMaxY = 0;

	//Create a good spacing between words. 
	error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
	DL_ASSERT_EXP(!error, "Failed to load glyph! x");
	FT_GlyphSlot space = face->glyph;
	fontData->myWordSpacing = static_cast<short>(space->metrics.width / 256.f);

	int currentMax = 127;
	int currentI = 32;


	for (int i = currentI; i < currentMax; i++)
	{
		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		DL_ASSERT_EXP(!error, "Failed to load glyph!");
		FT_GlyphSlot slot = face->glyph;

		FT_Matrix matrix = {
			(int)((1.0 / multiple_baseline) * 0x10000L),
			(int)((0.0) * 0x10000L),
			(int)((0.0) * 0x10000L),
			(int)((1.0) * 0x10000L) };

		//FT_Set_Transform(face, &matrix, NULL);

		if (atlasX + slot->bitmap.width + (aBorderWidth * 2) > atlasWidth)
		{
			atlasX = x_start;
			atlasY = currentMaxY;
		}
		if (aBorderWidth > 0)
		{
			LoadOutline(i, atlasX, atlasY, atlasWidth, fontData, face, aBorderWidth);
		}
		LoadGlyph(i, atlasX, atlasY, currentMaxY, atlasWidth, atlasHeight, fontData, face, aBorderWidth);
		

	}

	graphics::Texture2DDesc _desc;
	_desc.m_Width = (u32)atlasWidth;
	_desc.m_Height = (u32)atlasHeight;
	_desc.m_Format = graphics::RGBA8_UNORM;
	_desc.m_CPUAccessFlag = graphics::WRITE;
	_desc.m_Binding = graphics::BIND_SHADER_RESOURCE;
	_desc.m_Usage = graphics::DYNAMIC_USAGE;
	_desc.m_MipLevels = 1;
	_desc.m_SampleCount = 1;
	_desc.m_ArraySize = 1;

	auto* api = Engine::GetAPI();

	const s32 channel_count = 4; //RGBA 
	const s32 pitch = atlasSize * channel_count;
	s8* data = (s8*)fontData->myAtlas;
	ITexture2D* texture = api->GetDevice().CreateTexture2D(_desc, data, pitch, "AtlasTexture");
	DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");

	fontData->m_AtlasView = api->GetDevice().CreateShaderResource(_desc, texture, "Font Atlas");

#ifdef SAVE
	CreateDirectory("Atlas", NULL);
	Texture::SaveToDisk(L"Atlas/atlas.dds", texture);
#endif
	api->ReleasePtr(texture);

	fontData->myAtlasHeight = atlasSize;
	fontData->myAtlasWidth = atlasSize;
	fontData->myLineSpacing = (face->ascender - face->descender) / multiple_baseline;
	FT_Done_Face(face);
	CFont* newFont = new CFont(fontData);
	return newFont;
}

void CFontManager::LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY
							 , float atlasWidth, float atlasHeight, SFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{
	FT_Error error = FT_Load_Char(aFace, index, FT_LOAD_RENDER);
	//FT_Error error = FT_Load_Glyph(aFace, index, FT_LOAD_RENDER);
	DL_ASSERT_EXP(!error, "Failed to load glyph!");
	FT_GlyphSlot slot = aFace->glyph;
	FT_Bitmap bitmap = slot->bitmap;

	const int height = bitmap.rows;
	const int width = bitmap.width;

	
	SCharData glyphData;
	glyphData.myChar = static_cast<char>(index);
	glyphData.myHeight = static_cast<short>(height + (aBorderOffset * 2));
	glyphData.myWidth = static_cast<short>(width + (aBorderOffset * 2));

	const float _width = atlasX + glyphData.myWidth;
	const float _height = atlasY + glyphData.myHeight;

	glyphData.myTopLeftUV = { (float(atlasX) / atlasWidth), (float(atlasY) / atlasHeight) };
	glyphData.myBottomRightUV = { (float(atlasX + glyphData.myWidth) / atlasWidth), (float(atlasY + glyphData.myHeight) / atlasHeight) };

	glyphData.myAdvanceX = slot->metrics.horiAdvance / multiple_baseline;
	glyphData.myBearingX = ((slot->metrics.horiBearingX + slot->metrics.width) / multiple_baseline);
	glyphData.myBearingY = ((slot->metrics.horiBearingY - slot->metrics.height) / multiple_baseline);


	if (glyphData.myTopLeftUV.x > 1 || glyphData.myTopLeftUV.y > 1 || glyphData.myBottomRightUV.x > 1 || glyphData.myBottomRightUV.y > 1)
	{
		FONT_LOG("Tried to set a UV coord to above 1 at glyph : %c , index %d", index, index);
		FONT_LOG("TopLeftUV X: %f", glyphData.myTopLeftUV.x);
		FONT_LOG("TopLeftUV Y: %f", glyphData.myTopLeftUV.y);
		FONT_LOG("BottomRightUV X: %f", glyphData.myBottomRightUV.x);
		FONT_LOG("BottomRightUV Y: %f", glyphData.myBottomRightUV.y);
		DL_ASSERT("Tried to set a glyph UV to above 1. See log for more information.");
	}


	CalculateGlyphOffsets(index, slot);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (x < 0 || y < 0)
			{
				continue;
			}
			int& saved = aFontData->myAtlas[((atlasY) + y) * int(atlasWidth) + (atlasX + x)];
			saved |= bitmap.buffer[y * bitmap.width + x];

			if (y + (atlasY + y_offset) > maxY)
			{
				maxY = y + (atlasY + y_offset);
			}

		//https://github.com/rougier/freetype-gl/blob/master/texture-font.c
		}
	}

	atlasX = atlasX + width + x_offset;
	aFontData->myCharData[static_cast<char>(index)] = glyphData;
}

void CFontManager::LoadOutline(const int index, const int atlasX, const int atlasY
							   , const float atlasWidth, SFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{
	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	DL_ASSERT_EXP(!err, "Failed to load glyph!");
	err = FT_Get_Glyph(aFace->glyph, &glyph);
	DL_ASSERT_EXP(!err, "Failed to get glyph!");

	/*FT_Outline out = aFace->glyph->outline;
	u32 c = out.n_points;
	FT_Vector* v = out.points;*/


	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	DL_ASSERT_EXP(!err, "Failed to get glyph!");

	FT_Stroker_Set(stroker, aBorderOffset * multiple_baseline, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	DL_ASSERT_EXP(err == 0, "Failed to stroke");

	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	DL_ASSERT_EXP(err == 0, "Failed to add glyph to bitmap");

	//Bitmap width can be wrong on outline glyphs and creates an issue where they're not aligned with the regular glyphs.
	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;


	CalculateOutlineOffsets(index, aFace, aBorderOffset);
	for (u32 x = 0; x < width; x++)
	{
		for (u32 y = 0; y < height; y++)
		{

			int& data = aFontData->myAtlas[((atlasY + myOffset.yDelta) + y) * int(atlasWidth) + ((atlasX + myOffset.xDelta) + x)];
			data = 0;
			data |= bitmapGlyph->bitmap.buffer[y * width + x];
			data = cl::Color32Reverse(data);


		}
	}

	FT_Stroker_Done(stroker);
}

void CFontManager::CalculateOutlineOffsets(const int index, FT_FaceRec_* aFace, int aBorderOffset)
{
	myOffset.xDelta = 0;
	myOffset.yDelta = 0;

	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	DL_ASSERT_EXP(!err, "Failed to load glyph!");
	err = FT_Get_Glyph(aFace->glyph, &glyph);
	DL_ASSERT_EXP(!err, "Failed to get glyph!");

	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	DL_ASSERT_EXP(!err, "Failed to get glyph!");

	FT_Stroker_Set(stroker, aBorderOffset * multiple_baseline, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	DL_ASSERT_EXP(err == 0, "Failed to stroke");

	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	DL_ASSERT_EXP(err == 0, "Failed to add glyph to bitmap");

	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;

	int xDelta = 0;
	int yDelta = 0;

	SCountData countData;
	for (u32 x = 0; x < width; x++)
	{
		for (u32 y = 0; y < height; y++)
		{
			unsigned char& data = bitmapGlyph->bitmap.buffer[y * width + x];

			if (data == 0)
				CountOffsets(x, y, width, height, countData);
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myOffset.xDelta = xDelta;
	myOffset.yDelta = yDelta;
}

void CFontManager::CalculateGlyphOffsets(const int /*index*/, FT_GlyphSlotRec_* glyph)
{

	int xDelta = 0;
	int yDelta = 0;

	unsigned int width = glyph->bitmap.width;
	unsigned int height = glyph->bitmap.rows;

	SCountData countData;
	for (u32 x = 0; x < width; x++)
	{
		for (u32 y = 0; y < height; y++)
		{
			unsigned char& data = glyph->bitmap.buffer[y * width + x];

			if (data == 0)
				CountOffsets(x, y, width, height, countData);
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myOffset.xDelta = xDelta;
	myOffset.yDelta = yDelta;
}

void CFontManager::CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData)
{
	if (x < 1)
	{
		countData.xNCount++;
	}

	if (x > width - 1)
	{
		countData.xPCount++;
	}

	if (y < 1)
	{
		countData.yNCount++;
	}

	if (y > height - 1)
	{
		countData.yPCount++;
	}
}

void CFontManager::CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData)
{
	if (countData.xNCount == height)
	{
		countData.xNCount = 0;
		deltaX--;
	}

	if (countData.xPCount == height)
	{
		countData.xPCount = 0;
		deltaX++;
	}

	if (countData.yNCount == width)
	{
		countData.yNCount = 0;
		deltaY--;
	}

	if (countData.yPCount == width)
	{
		countData.yPCount = 0;
		deltaY++;
	}
}

SFontData::~SFontData()
{
	SAFE_DELETE(myAtlas);
}

