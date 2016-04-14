#include "stdafx.h"
#include "FontManager.h"
#include <DL_Debug.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <D3DX11.h>
#include <D3D11.h>
#include "DirectX11.h"
#include "Engine.h"
#include "Texture.h"
#include <Utilities.h>
#include "EngineDefines.h"

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

	void CFontManager::LoadFont(const char* aFontPath, short aFontHeight)
	{
		myFontHeight = aFontHeight;
		myFontPath = aFontPath;

		int error = FT_New_Face(myLibrary, myFontPath, 0, &myFace);
		FONT_LOG("Loading font:%s", myFontPath);
		DL_ASSERT_EXP(!error, "Failed to load requested font.");
		error = FT_Set_Pixel_Sizes(myFace, 0, aFontHeight);
		DL_ASSERT_EXP(!error, "Failed to set pixel size!");

		for (int i = 65; i < 128; i++)
		{
			//int glyphIndex = FT_Get_Char_Index(myFace, i);
			/*error = FT_Load_Glyph(myFace, i, FT_LOAD_DEFAULT);
			if(error)
			continue;*/
			error = FT_Load_Char(myFace, i, FT_LOAD_RENDER);
			
			if (error)
				continue;
			//DL_ASSERT_EXP(!error, "Failed to load glyph!");

			FT_GlyphSlot slot = myFace->glyph;
			FT_Bitmap bitmap = slot->bitmap;
			bitmap.pixel_mode = FT_PIXEL_MODE_GRAY;
			if (bitmap.buffer == nullptr)
			{
				continue;
			}

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = bitmap.buffer;
			data.SysMemPitch = bitmap.pitch;

			D3D11_TEXTURE2D_DESC info;
			info.Width = bitmap.width;
			info.Height = bitmap.rows;
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
			ss << "Glyph_" << i << ".dds";
			HRESULT hr = D3DX11SaveTextureToFile(CEngine::GetInstance()->GetAPI()->GetContext(), texture, D3DX11_IMAGE_FILE_FORMAT::D3DX11_IFF_DDS, ss.str().c_str());
			CEngine::GetInstance()->GetAPI()->HandleErrors(hr, "Failed to save texture because : ");
		}
	}



};