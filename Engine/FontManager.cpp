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

#define SAVE_DDS
#ifndef SAVE_DDS
#define SAVE_PNG
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
		myDevice = nullptr;
		FT_Done_Face(myFace);
		FT_Done_FreeType(myLibrary);
		myAtlas->Release();
		myAtlas = nullptr;
	}

	void CFontManager::Initiate()
	{
		myDevice = CEngine::GetDirectX()->GetDevice(); //Obtain the device.
		int error = FT_Init_FreeType(&myLibrary);
		DL_ASSERT_EXP(!error, "Failed to initiate FreeType.");
		myPacker.Initiate(512, 512);

		D3D11_TEXTURE2D_DESC info;
		info.Width = 512;
		info.Height = 512;
		info.MipLevels = 1;
		info.ArraySize = 1;
		info.SampleDesc.Count = 1;
		info.SampleDesc.Quality = 0;
		info.MiscFlags = 0;
		info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		info.Usage = D3D11_USAGE_DEFAULT;
		info.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		myDevice->CreateTexture2D(&info, nullptr, &myAtlas);

		D3D11_RENDER_TARGET_VIEW_DESC rtd;
		ZeroMemory(&rtd, sizeof(rtd));
		rtd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtd.Texture2DArray.MipSlice = 0;
		rtd.Texture2DArray.ArraySize = 1;
		myDevice->CreateRenderTargetView(myAtlas, &rtd, &myRenderTarget);
	}

	void CFontManager::LoadFont(const char* aFontPath, short aFontWidth)
	{
		myFontWidth = aFontWidth;
		myFontPath = aFontPath;
		int error = FT_New_Face(myLibrary, myFontPath, 0, &myFace);
		FONT_LOG("Loading font:%s", myFontPath);
		DL_ASSERT_EXP(!error, "Failed to load requested font.");

		error = FT_Set_Char_Size(myFace, (aFontWidth * 64), 0, 300, 300);
		DL_ASSERT_EXP(!error, "Failed to set pixel size!");
		CreateDirectory("Glyphs", NULL); //Creates a folder for the glyphs


		for (int i = 65; i < 128; i++)
		{
			error = FT_Load_Char(myFace, i, FT_LOAD_RENDER);
			DL_ASSERT_EXP(!error, "Failed to load glyph!");
			FT_GlyphSlot slot = myFace->glyph;
			slot->format = FT_GLYPH_FORMAT_BITMAP;
			FT_Bitmap bitmap = slot->bitmap;
			bitmap.pixel_mode = FT_PIXEL_MODE_MONO;
			FT_Render_Glyph(slot, FT_RENDER_MODE_MONO);

			int height = bitmap.rows;
			int width = bitmap.width;
			int pitch = bitmap.pitch;
			//int gHeight = slot->metrics.height;
			//int gWidth = slot->metrics.width;

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
					saved |= bitmap.buffer[y * bitmap.width + x];

					// Remove this if statement if you only want in the alpha channel.
					SColor color(saved);
					if (color.a > 0)
					{
						saved |= 0xffffffff;
					}
					saved = CL::Color32Reverse(saved);
				}
			}
			FONT_LOG("Successfully created & flipped bitmap");

			CEngine::GetDirectX()->SetViewport(width, height, 0);

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = gData;
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
			myDevice->CreateTexture2D(&info, &data, &texture);
			DL_ASSERT_EXP(texture != nullptr, "Texture is nullptr!");

			ID3D11ShaderResourceView* shaderResource;
			myDevice->CreateShaderResourceView(texture, nullptr, &shaderResource);
			myTopNode = myPacker.Insert(width, height, shaderResource);

			CEngine::GetDirectX()->GetContext()->OMSetRenderTargets(1, &myRenderTarget, nullptr);

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

			delete[] gData;
			gData = nullptr;
		}

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
		HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetInstance()->GetAPI()->GetContext(), myAtlas, format, ss.str().c_str());
		CEngine::GetDirectX()->HandleErrors(hr, "Failed to save texture because : ");

		CEngine::GetDirectX()->ResetViewport();
		CEngine::GetDirectX()->ResetRendertarget();
	}


	ID3D11ShaderResourceView* CFontManager::GetShaderResource()
	{
		return myPacker.GetRoot()->myImage;
	}
};