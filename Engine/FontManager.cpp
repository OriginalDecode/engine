#include "stdafx.h"
#include "FontManager.h"
#include <DL_Debug.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

#include <D3DX11.h>
#include <D3D11.h>
#include "DirectX11.h"
#include "Engine.h"
#include "Texture.h"
#include <Utilities.h>
#include "EngineDefines.h"
#include <Math/Vector/Vector.h>
#include "Font.h"
//#define SAVE
#ifdef SAVE
#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
#endif
#endif


namespace Snowblind
{
	SColor::SColor(int color)
		: r(0)
		, g(0)
		, b(0)
		, a(0)
		, _color(0)
	{
		a = (color)& 0xFF;
		r = (color >> 8) & 0xFF;
		g = (color >> 16) & 0xFF;
		b = (color >> 24) & 0xFF;
		_color = color;
	}

	SColor::SColor(int red, int green, int blue, int alpha)
	{
		_color = (red << 24) + (green << 16) + (blue << 8) + (alpha);
	}

	CFontManager::CFontManager()
	{
	}

	CFontManager::~CFontManager()
	{
		myFontPath = nullptr;
		FT_Done_FreeType(myLibrary);
		
		//myAtlasView->Release();
		//myAtlasView = nullptr;
	}

	void CFontManager::Initiate()
	{
		int error = FT_Init_FreeType(&myLibrary);
		DL_ASSERT_EXP(!error, "Failed to initiate FreeType.");
	}

	CFont* CFontManager::LoadFont(const char* aFontPath, short aFontWidth)
	{
		SFontData* fontData = new SFontData;
		fontData->myAtlas = new int[512 * 512];
		ZeroMemory(fontData->myAtlas, (512 * 512) * sizeof(int));
		FT_Face face = fontData->myFaceData;





		fontData->myFontHeightWidth = aFontWidth;
		myFontPath = aFontPath;
		int error = FT_New_Face(myLibrary, myFontPath, 0, &face);
		FONT_LOG("Loading font:%s", myFontPath);
		DL_ASSERT_EXP(!error, "Failed to load requested font.");

		error = FT_Set_Char_Size(face, (fontData->myFontHeightWidth << 6), 0, 300, 300);
		DL_ASSERT_EXP(!error, "Failed to set pixel size!");

#ifdef SAVE
		CreateDirectory("Glyphs", NULL); //Creates a folder for the glyphs
#endif

		int atlasX = 0;
		int atlasY = 0;
		float atlasWidth = 512; //have to be replaced.
		float atlasHeight = 512; //have to be replaced
		int currentMaxY = 0;

		//Create a good spacing between words. 
		error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
		DL_ASSERT_EXP(!error, "Failed to load glyph! x");
		FT_GlyphSlot space = face->glyph;
		fontData->myWordSpacing = space->metrics.width >> 8;

		for (int i = 32; i < 126; i++)
		{
			error = FT_Load_Char(face, i, FT_LOAD_RENDER);
			DL_ASSERT_EXP(!error, "Failed to load glyph!");
			FT_GlyphSlot slot = face->glyph;
			slot->format = FT_GLYPH_FORMAT_BITMAP;
			FT_Bitmap bitmap = slot->bitmap;
			bitmap.pixel_mode = FT_PIXEL_MODE_MONO;
			FT_Render_Glyph(slot, FT_RENDER_MODE_MONO);

			int height = bitmap.rows;
			int width = bitmap.width;

			if (atlasX + width > atlasWidth)
			{
				atlasX = 0;
				atlasY = currentMaxY;
			}

			SCharData glyphData;
			glyphData.myChar = i;
			glyphData.myHeight = height;
			glyphData.myWidth = width;
			glyphData.myTopLeftUV = { float(atlasX) / atlasWidth, float(atlasY) / atlasHeight };
			glyphData.myBottomRightUV = { float(atlasX + width) / atlasWidth, float(atlasY + height) / atlasHeight };
			glyphData.myAdvanceX = slot->metrics.width >> 6;
			glyphData.myBearingX = ((slot->metrics.horiBearingX + slot->metrics.width) >> 6);
			glyphData.myBearingY = (slot->metrics.horiBearingY - slot->metrics.height) >> 6;


			//Face holds the ascender. Look at that. I believe that can solve my issues.
			if (glyphData.myTopLeftUV.x > 1 || glyphData.myTopLeftUV.y > 1 || glyphData.myBottomRightUV.x > 1 || glyphData.myBottomRightUV.y > 1)
			{
				FONT_LOG("Tried to set a UV coord to above 1 at glyph : %c , index %d", i, i);
				FONT_LOG("TopLeftUV X: %f", glyphData.myTopLeftUV.x);
				FONT_LOG("TopLeftUV Y: %f", glyphData.myTopLeftUV.y);
				FONT_LOG("BottomRightUV X: %f", glyphData.myBottomRightUV.x);
				FONT_LOG("BottomRightUV Y: %f", glyphData.myBottomRightUV.y);
				DL_ASSERT("Tried to set a glyph UV to above 1. See log for more information.");
			}

			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					if (x < 0 || y < 0)
					{
						continue;
					}
					int& saved = fontData->myAtlas[(atlasY + y) * int(atlasWidth) + (atlasX + x)];
					saved = 0;
					saved |= bitmap.buffer[y * bitmap.width + x];
					saved = CL::Color32Reverse(saved);

					if (y + (atlasY + 8) > currentMaxY)
					{
						currentMaxY = y + (atlasY + 8);
					}

				}
			}
			atlasX = atlasX + width + 2;
			fontData->myCharData[i] = glyphData;


#ifdef SAVE
			std::stringstream ss;
			D3DX11_IMAGE_FILE_FORMAT format;
#ifdef SAVE_DDS
			ss << "Glyphs/Glyph_" << i << ".dds";
			format = D3DX11_IFF_DDS;
#endif
#ifdef SAVE_PNG
			ss << "Glyphs/Glyph_" << i << ".png";
			format = D3DX11_IFF_PNG;
#endif
			HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetInstance()->GetAPI()->GetContext(), texture, format, ss.str().c_str());
			CEngine::GetDirectX()->HandleErrors(hr, "Failed to save texture because : ");
			texture->Release();
#endif
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = fontData->myAtlas;
		data.SysMemPitch = 512 * 4;

		D3D11_TEXTURE2D_DESC info;
		info.Width = 512;
		info.Height = 512;
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

		texture->Release();

#ifdef SAVE
		std::stringstream ss;
		D3DX11_IMAGE_FILE_FORMAT format;
#ifdef SAVE_DDS
		ss << "Glyphs/Atlas_" << ".dds";
		format = D3DX11_IFF_DDS;
#endif
#ifdef SAVE_PNG
		ss << "Glyphs/Atlas_" << ".png";
		format = D3DX11_IFF_PNG;
#endif
		HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetInstance()->GetAPI()->GetContext(), texture, format, ss.str().c_str());
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to save texture because : ");
#endif

		fontData->myAtlasHeight = 512;
		fontData->myAtlasWidth = 512;
		fontData->myLineSpacing = (face->height >> 6) * 2.f;
		FT_Done_Face(face);
		CFont* newFont = new CFont(fontData);
		return newFont;
	}

	SFontData::~SFontData()
	{
		delete myAtlas;
		myAtlas = nullptr;
		myAtlasView->Release();
		myAtlasView = nullptr;
	}

};