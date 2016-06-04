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
#include <D3D11.h>
#include <D3DX11async.h>
#include "Engine.h"
#include <vector>
#define SAVE
#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
#endif

#define X_OFFSET 8
#define X_START 2
#define Y_OFFSET 4

namespace Snowblind
{
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

	CFont* CFontManager::LoadFont(const char* aFontPath, short aFontWidth, int aBorderWidth)
	{
		std::string fontFolder = aFontPath;
		if (!CL::substr(aFontPath, "/"))
		{
			fontFolder = "";
			TCHAR dir[32];
			GetSystemDirectory(dir, 32);
			fontFolder += dir;
			fontFolder = CL::substr(fontFolder, "\\", true, 0);
			fontFolder += "\\Fonts\\";
			fontFolder += aFontPath;
		}

		int atlasSize = (aFontWidth * aFontWidth); //This is correct
		atlasSize *= 2;
		FONT_LOG("Font Size W/H: %d", atlasSize);
		float atlasWidth = atlasSize; //have to be replaced.
		float atlasHeight = atlasSize; //have to be replaced

		std::stringstream key;
		key << aFontPath << "-" << aFontWidth;
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

		fontData->myAtlas = new int[atlasSize * atlasSize];
		ZeroMemory(fontData->myAtlas, (atlasSize * atlasSize) * sizeof(int));

		fontData->myFontHeightWidth = aFontWidth;
		myFontPath = fontFolder.c_str();

		FT_Face face;
		FT_Error error = FT_New_Face(myLibrary, myFontPath, 0, &face);
		FT_Size size = face->size;
		FONT_LOG("Loading font:%s", myFontPath);
		DL_ASSERT_EXP(!error, "Failed to load requested font.");

		FT_F26Dot6 ftSize = (FT_F26Dot6)(fontData->myFontHeightWidth * (1 << 6));
		error = FT_Set_Char_Size(face, ftSize, 0, 96, 0); // 96 = 100% scaling in Windows. 
		//error = FT_Set_Pixel_Sizes(face, 0, 16);
		DL_ASSERT_EXP(!error, "[FontManager] : Failed to set pixel size!");

#ifdef SAVE
		CreateDirectory("Glyphs", NULL); //Creates a folder for the glyphs
#endif
		int atlasX = X_START;
		int atlasY = 2;
		int currentMaxY = 0;

		//Create a good spacing between words. 
		error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
		DL_ASSERT_EXP(!error, "Failed to load glyph! x");
		FT_GlyphSlot space = face->glyph;
		fontData->myWordSpacing = space->metrics.width / 256.f;


		int currentMax = 126;
		int currentI = 32;


		for (int i = currentI; i < currentMax; i++)
		{
			int error = FT_Load_Char(face, i, FT_LOAD_RENDER);
			DL_ASSERT_EXP(!error, "Failed to load glyph!");
			FT_GlyphSlot slot = face->glyph;

			if (atlasX + slot->bitmap.width + (aBorderWidth * 2) > atlasWidth)
			{
				atlasX = X_START;
				atlasY = currentMaxY;
			}
			if (aBorderWidth > 0)
			{
				LoadOutline(i, atlasX, atlasY, atlasWidth, fontData, face, aBorderWidth);
			}
			LoadGlyph(i, atlasX, atlasY, currentMaxY, atlasWidth, atlasHeight, fontData, face, aBorderWidth);
		}

		DumpAtlas(fontData, atlasSize);

		fontData->myAtlasHeight = atlasSize;
		fontData->myAtlasWidth = atlasSize;
		fontData->myLineSpacing = (face->ascender - face->descender) / 64.f;
		FT_Done_Face(face);
		CFont* newFont = new CFont(fontData);
		return newFont;
	}

	void CFontManager::LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY
		, float atlasWidth, float atlasHeight, SFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
	{
		FT_Error error = FT_Load_Char(aFace, index, FT_LOAD_RENDER);
		DL_ASSERT_EXP(!error, "Failed to load glyph!");
		FT_GlyphSlot slot = aFace->glyph;
		FT_Bitmap bitmap = slot->bitmap;

		int height = bitmap.rows;
		int width = bitmap.width;

		SCharData glyphData;
		glyphData.myChar = index;
		glyphData.myHeight = height + (aBorderOffset * 2);
		glyphData.myWidth = width + (aBorderOffset * 2);

		glyphData.myTopLeftUV = { (float(atlasX) / atlasWidth), (float(atlasY) / atlasHeight) };
		glyphData.myBottomRightUV = { (float(atlasX + width + (aBorderOffset * 2)) / atlasWidth), (float(atlasY + height + (aBorderOffset * 2)) / atlasHeight) };

		glyphData.myAdvanceX = slot->metrics.width / 64.f;
		glyphData.myBearingX = ((slot->metrics.horiBearingX / 64.f) + (slot->metrics.width / 64.f)) + (aBorderOffset);
		glyphData.myBearingY = ((slot->metrics.horiBearingY - slot->metrics.height) / 64.f);

		//Kerning is needed
		if (glyphData.myTopLeftUV.x > 1 || glyphData.myTopLeftUV.y > 1 || glyphData.myBottomRightUV.x > 1 || glyphData.myBottomRightUV.y > 1)
		{
			FONT_LOG("Tried to set a UV coord to above 1 at glyph : %c , index %d", index, index);
			FONT_LOG("TopLeftUV X: %f", glyphData.myTopLeftUV.x);
			FONT_LOG("TopLeftUV Y: %f", glyphData.myTopLeftUV.y);
			FONT_LOG("BottomRightUV X: %f", glyphData.myBottomRightUV.x);
			FONT_LOG("BottomRightUV Y: %f", glyphData.myBottomRightUV.y);
			DumpAtlas(aFontData, atlasWidth);
			DL_ASSERT("Tried to set a glyph UV to above 1. See log for more information.");
		}

#ifdef SAVE
		int* gData = new int[bitmap.width * bitmap.rows];
		ZeroMemory(gData, bitmap.width * bitmap.rows * sizeof(int));
#endif
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (x < 0 || y < 0)
				{
					continue;
				}
				int& saved = aFontData->myAtlas[((atlasY)+aBorderOffset + y) *
					int(atlasWidth) + ((atlasX)+aBorderOffset + x)];
				saved |= bitmap.buffer[y * bitmap.width + x];

				if (y + (atlasY + Y_OFFSET) > maxY)
				{
					maxY = y + (atlasY + Y_OFFSET);
				}
#ifdef SAVE
				int& toSave = gData[y * bitmap.width + x];
				toSave = 0;
				toSave |= bitmap.buffer[y * bitmap.width + x];
				toSave = CL::Color32Reverse(toSave);
#endif
			}
		}

		atlasX = atlasX + width + X_OFFSET;
		aFontData->myCharData[index] = glyphData;

#ifdef SAVE
		if (bitmap.rows <= 0 || bitmap.pitch <= 0)
		{
			delete[] gData;
			gData = nullptr;
			return;
		}

		DumpGlyph(gData, index, width, height, bitmap.pitch);

		delete[] gData;
		gData = nullptr;
#endif
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

		glyph->format = FT_GLYPH_FORMAT_OUTLINE;

		err = FT_Stroker_New(myLibrary, &stroker);
		DL_ASSERT_EXP(!err, "Failed to get glyph!");

		FT_Stroker_Set(stroker, aBorderOffset * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
		err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
		DL_ASSERT_EXP(err == 0, "Failed to stroke");

		err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
		DL_ASSERT_EXP(err == 0, "Failed to add glyph to bitmap");

		FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
		for (int x = 0; x < bitmapGlyph->bitmap.width; x++)
		{
			for (int y = 0; y < bitmapGlyph->bitmap.rows; y++)
			{
				int& data = aFontData->myAtlas[(atlasY + y) * int(atlasWidth) + (atlasX + x)];
				data = 0;
				data |= bitmapGlyph->bitmap.buffer[y * bitmapGlyph->bitmap.width + x];
				data = CL::Color32Reverse(data);
			}
		}

		FT_Stroker_Done(stroker);
	}

	void CFontManager::DumpAtlas(SFontData* fontData, int atlasSize)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = fontData->myAtlas;
		data.SysMemPitch = atlasSize * 4;

		D3D11_TEXTURE2D_DESC info;
		info.Width = atlasSize;
		info.Height = atlasSize;
		info.MipLevels = 1;
		info.ArraySize = 1;
		info.SampleDesc.Count = 1;
		info.SampleDesc.Quality = 0;
		info.MiscFlags = 0;
		info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		info.Usage = D3D11_USAGE_DYNAMIC;
		info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Texture2D* texture;
		CEngine::GetDirectX()->GetDevice()->CreateTexture2D(&info, &data, &texture);
		DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");
		CEngine::GetDirectX()->GetDevice()->CreateShaderResourceView(texture, nullptr, &fontData->myAtlasView);

		std::string name = "";
		name = CL::substr(myFontPath, "\\", false, 1);
		name = CL::substr(name, ".", true, 1);

		if (CL::substr(myFontPath, "/"))
		{
			name = CL::substr(myFontPath, "/", false, 1);
			name = CL::substr(name, ".", true, 1);
		}

		std::stringstream ss3;
		D3DX11_IMAGE_FILE_FORMAT format;
#ifdef SAVE_DDS
		ss3 << "Glyphs/Atlas_" << name << ".dds";
		format = D3DX11_IFF_DDS;
#endif
#ifdef SAVE_PNG
		ss3 << "Glyphs/Atlas_" << name << ".png";
		format = D3DX11_IFF_PNG;
#endif
		HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetDirectX()->GetContext(), texture, format, ss3.str().c_str());
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to save texture because : ");
	}

	void CFontManager::DumpGlyph(int* source, int index, int width, int height, int pitch)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = source;
		data.SysMemPitch = pitch * 4;

		D3D11_TEXTURE2D_DESC info;
		info.Width = width;
		info.Height = height;
		info.MipLevels = 1;
		info.ArraySize = 1;
		info.SampleDesc.Count = 1;
		info.SampleDesc.Quality = 0;
		info.MiscFlags = 0;
		info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		info.Usage = D3D11_USAGE_DYNAMIC;
		info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Texture2D* texture;
		CEngine::GetDirectX()->GetDevice()->CreateTexture2D(&info, &data, &texture);
		DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");

		std::stringstream ss;
		D3DX11_IMAGE_FILE_FORMAT format;
#ifdef SAVE_DDS
		ss << "Glyphs/Glyph_" << index << ".dds";
		format = D3DX11_IFF_DDS;
#endif
#ifdef SAVE_PNG
		ss << "Glyphs/Glyph_" << index << ".png";
		format = D3DX11_IFF_PNG;
#endif
		HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetDirectX()->GetContext(), texture, format, ss.str().c_str());
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to save texture because : ");
		texture->Release();
	}

	SFontData::~SFontData()
	{
		SAFE_DELETE(myAtlas);
		SAFE_RELEASE(myAtlasView);
	}

};