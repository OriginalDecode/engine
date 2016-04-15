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

#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
#endif

namespace Snowblind
{
	CFontManager::CFontManager()
	{
	}

	CFontManager::~CFontManager()
	{
		myFontPath = nullptr;
		myDevice = nullptr;
		myTextures.DeleteAll();
		FT_Done_Face(myFace);
		FT_Done_FreeType(myLibrary);
	}

	void CFontManager::Initiate()
	{
		myDevice = CEngine::GetInstance()->GetAPI()->GetDevice();
		int error = FT_Init_FreeType(&myLibrary);
		DL_ASSERT_EXP(!error, "Failed to initiate FreeType.");
	}

	void CFontManager::LoadFont(const char* aFontPath, short aFontWidth)
	{
		myFontWidth = aFontWidth;
		myFontPath = aFontPath;

		int error = FT_New_Face(myLibrary, myFontPath, 0, &myFace);
		FONT_LOG("Loading font:%s", myFontPath);
		DL_ASSERT_EXP(!error, "Failed to load requested font.");

		//error = FT_Set_Pixel_Sizes(myFace, aFontWidth, 0);
		error = FT_Set_Char_Size(myFace, (aFontWidth * 64), 0, 300, 300);
		DL_ASSERT_EXP(!error, "Failed to set pixel size!");



		//	error = FT_Set_Char_Size(myFace, aFontHeight, aFontHeight, 72, 72);

		CreateDirectory("Glyphs", NULL); //Creates a folder for the glyphs

		for (int i = 65; i < 128; i++)
		{
			error = FT_Load_Char(myFace, i, FT_LOAD_RENDER);

			if (error) //Replace with better error handling.
				continue;
			//DL_ASSERT_EXP(!error, "Failed to load glyph!");

			FT_GlyphSlot slot = myFace->glyph;
			slot->format = FT_GLYPH_FORMAT_BITMAP;
			FT_Bitmap bitmap = slot->bitmap;
			bitmap.pixel_mode = FT_PIXEL_MODE_MONO;
			FT_Render_Glyph(slot, FT_RENDER_MODE_MONO);
			// FT_RENDER_MODE_NORMAL
			int height = (bitmap.rows);
			int width = (bitmap.width);
			int pitch = bitmap.pitch;

			int gHeight = slot->metrics.height;
			int gWidth = slot->metrics.width;


			unsigned char* buffer = bitmap.buffer;
			int* gData = new int[width*height];


			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					if (x < 0 || y < 0)
					{
						continue;
					}

					int& saved = gData[y * bitmap.width + x];
					saved = 0;
					saved |= saved & bitmap.buffer[y * bitmap.width + x];
					saved |= 0xff000000;
				}
			}

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = gData;
			data.SysMemPitch = pitch * 4; //Use pitch for matrix effect

			D3D11_TEXTURE2D_DESC info;
			info.Width = width ;
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
			myDevice->CreateTexture2D(&info, &data, &texture);
			DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");



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
			CEngine::GetInstance()->GetAPI()->HandleErrors(hr, "Failed to save texture because : ");
			delete[] gData;
			gData = nullptr;
			texture->Release();
		}
	}



};