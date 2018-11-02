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
#include "Engine.h"

#define SAVE
#ifdef SAVE
#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
#endif
#endif

//#define COBJMACROS


#include <Engine/IGraphicsAPI.h>
#include <Engine/IGraphicsContext.h>
#include <Engine/IGraphicsDevice.h>

constexpr s32 x_offset = 1;
constexpr s32 y_offset = 1;
constexpr s32 x_start = 0;

constexpr s32 multiple_baseline = 64;
constexpr s32 dpi = 72;

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
	FT_Done_FreeType(m_Library);
	for (auto it = myFontData.begin(); it != myFontData.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
}

void CFontManager::Initiate()
{
	int error = FT_Init_FreeType(&m_Library);
	ASSERT(error == 0, "Failed to initiate FreeType.");
}

CFont* CFontManager::LoadFont(const s8* aFontPath, u16 aSize, u16 aBorderWidth)
{

	//Check for windows default fonts
	std::string fontFolder = aFontPath;
	if (!cl::substr(aFontPath, "/"))
	{
		fontFolder = "";
		TCHAR dir[32];
		GetSystemDirectory(dir, 32);
		fontFolder += dir;

		size_t pos = fontFolder.find("\\");
		fontFolder = fontFolder.substr(0, pos);

		fontFolder += "\\Fonts\\";
		fontFolder += aFontPath;
	}

	u16 font_width = aSize;
	int atlasSize = (font_width * font_width) * 2; // *dpi;
	atlasSize = int(cl::nearest_Pow(atlasSize));
	FONT_LOG("Font Size %dx%d", atlasSize, atlasSize);

	char key[255];
	sprintf_s(key, "%s-%d", aFontPath, font_width);


	if (myFontData.find(key) != myFontData.end())
	{
		FONT_LOG("Font Data already found, creating Font Object with existing font data! %s", key);
		return new CFont(myFontData[key]);
	}

	m_FontToLoad = nullptr;
	m_FontToLoad = new SFontData;

	const int total_size = atlasSize * atlasSize;
	m_FontToLoad->myAtlas = new int[total_size];
	ZeroMemory(m_FontToLoad->myAtlas, total_size * sizeof(int));

	m_FontToLoad->myFontHeightWidth = font_width;
	m_FontToLoad->myAtlasWidth = atlasSize;
	m_FontToLoad->myAtlasHeight = atlasSize;

	FT_Face face;
	FT_Error error = FT_New_Face(m_Library, fontFolder.c_str(), 0, &face);
	FONT_LOG("Loading font : %s", fontFolder.c_str());
	ASSERT(error == 0, "Failed to load requested font.");

	FT_F26Dot6 ftSize = (FT_F26Dot6)(m_FontToLoad->myFontHeightWidth * multiple_baseline);
	//error = FT_Set_Char_Size(face, ftSize, 0, dpi * multiple_baseline, 0); // 96 = 100% scaling in Windows. 
	error = FT_Set_Pixel_Sizes(face, aSize, 0);
	ASSERT(error == 0, "Failed to set pixel size!");

	int atlasX = x_start;
	int atlasY = 0;
	int currentMaxY = 0;

	//Create a good spacing between words. 
	error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
	ASSERT(error == 0, "Failed to load glyph! x");

	m_FontToLoad->myWordSpacing = face->glyph->metrics.width / 256.f;

	constexpr int start_char_index = 32;
	constexpr int end_char_index = 127;

	for (int i = start_char_index; i < end_char_index; ++i)
	{
		LoadGlyph(i, atlasX, atlasY, currentMaxY, face);
	}

	CreateAtlas(atlasSize);

	m_FontToLoad->myLineSpacing = (face->ascender - face->descender) / multiple_baseline;
	FT_Done_Face(face);
	myFontData.emplace(key, m_FontToLoad);
	return new CFont(m_FontToLoad);
}

void CFontManager::LoadGlyph(int index, int& pen_x, int& pen_y, int& maxY, FT_FaceRec_* face)
{
	FT_Error error = FT_Load_Char(face, index, FT_LOAD_DEFAULT | FT_LOAD_TARGET_LIGHT);
	ASSERT(error == 0, "Failed to load glyph!");

	FT_GlyphSlot slot = face->glyph;
	error = FT_Render_Glyph(slot, FT_RENDER_MODE_LCD);
	ASSERT(error == 0, "Failed to render glyph!");

	const FT_Bitmap& bitmap = slot->bitmap;

	slot->subglyphs;

	SCharData glyphData;
	glyphData.myChar = char(index);
	glyphData.myHeight = u16(bitmap.rows);
	glyphData.myWidth = u16(bitmap.width);
	glyphData.m_Pitch = u16(bitmap.pitch);


	if (pen_x + glyphData.myWidth  > m_FontToLoad->myAtlasWidth)
	{
		pen_x = x_start;
		pen_y = maxY;
	}

	CalculateUV(glyphData, pen_x, pen_y);

	slot->advance.x; //width???

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

	for (u16 row = 0; row < glyphData.myHeight; ++row)
	{
		for (u16 col = 0; col < glyphData.myWidth; ++col)
		{
			const int x = pen_x + col;
			const int y = pen_y + row;
			const int tex_width = m_FontToLoad->myAtlasWidth;


			int& saved = m_FontToLoad->myAtlas[y * tex_width + x];
			saved |= bitmap.buffer[row * bitmap.pitch + col];

		}

		if (row + (pen_y + y_offset) > maxY)
			maxY = row + (pen_y + y_offset);
	}

	pen_x += glyphData.myWidth + x_offset;

	m_FontToLoad->myCharData.emplace(index, glyphData);
}


void CFontManager::CalculateUV(SCharData& glyphData, int x_pos, int y_pos)
{
	glyphData.myTopLeftUV.x = float(x_pos) / m_FontToLoad->myAtlasWidth;
	glyphData.myTopLeftUV.y = float(y_pos) / m_FontToLoad->myAtlasHeight;

	glyphData.myBottomRightUV.x = float(x_pos + glyphData.myWidth) / m_FontToLoad->myAtlasWidth;
	glyphData.myBottomRightUV.y = float(y_pos + glyphData.myHeight) / m_FontToLoad->myAtlasHeight;

}

void CFontManager::CreateAtlas(const int size)
{
	graphics::Texture2DDesc _desc;
	_desc.m_Width = (u32)m_FontToLoad->myAtlasWidth;
	_desc.m_Height = (u32)m_FontToLoad->myAtlasHeight;
	_desc.m_Format = graphics::RGBA8_UNORM;
	_desc.m_CPUAccessFlag = graphics::WRITE;
	_desc.m_Binding = graphics::BIND_SHADER_RESOURCE;
	_desc.m_Usage = graphics::DYNAMIC_USAGE;
	_desc.m_MipLevels = 1;
	_desc.m_SampleCount = 1;
	_desc.m_ArraySize = 1;

	auto* api = Engine::GetAPI();

	const s32 channel_count = 4; //RGBA 
	const s32 pitch = m_FontToLoad->myAtlasWidth * channel_count;
	ITexture2D* texture = api->GetDevice().CreateTexture2D(_desc, (s8*)m_FontToLoad->myAtlas, pitch, "AtlasTexture");
	ASSERT(texture != nullptr, "Texture is nullptr!");

	m_FontToLoad->m_AtlasView = api->GetDevice().CreateShaderResource(_desc, texture, "Font Atlas");

#ifdef SAVE
	CreateDirectory("Atlas", NULL);
	Texture::SaveToDisk(L"Atlas/atlas.dds", texture);
#endif
	api->ReleasePtr(texture);
}

void CFontManager::SaveTexture(const SCharData& _char)
{
	if (_char.myWidth <= 0 || _char.myHeight <= 0)
		return;

	graphics::Texture2DDesc tex_desc;
	tex_desc.m_Width = (u32)_char.myWidth;
	tex_desc.m_Height = (u32)_char.myHeight;
	tex_desc.m_Format = graphics::RGBA8_UNORM;
	tex_desc.m_CPUAccessFlag = graphics::WRITE;
	tex_desc.m_Binding = graphics::BIND_SHADER_RESOURCE;
	tex_desc.m_Usage = graphics::DYNAMIC_USAGE;
	tex_desc.m_MipLevels = 1;
	tex_desc.m_SampleCount = 1;
	tex_desc.m_ArraySize = 1;

	auto* api = Engine::GetAPI();

	const s32 channel_count = 4; //RGBA 
	const s32 pitch = 4;
	ITexture2D* texture = api->GetDevice().CreateTexture2D(tex_desc, _char.m_Data, pitch, "glyph");
	ASSERT(texture != nullptr, "Texture is nullptr!");

#ifdef SAVE
	CreateDirectory("Glyphs", NULL);

	wchar_t temp[100];
	swprintf_s(temp, L"Glyphs/%c.png", _char.myChar);
	Texture::SaveToDisk(temp, texture, texture_format::PNG);
#endif
	api->ReleasePtr(texture);
}
