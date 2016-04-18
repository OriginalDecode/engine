#pragma once
#include <DataStructures/StaticArray.h>
#include <unordered_map>
//#include "RectangleBinPack.h"
struct ID3D11Device;
struct FT_LibraryRec_;
struct FT_FaceRec_;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace Snowblind
{
	struct SColor
	{
		SColor(int color);
		SColor(int red, int green, int blue, int alpha);
		int r;
		int g;
		int b;
		int a;
		int _color;
	};

	class CTexture;
	class CFontManager
	{
	public:
		CFontManager();
		~CFontManager();
		void Initiate();
		void LoadFont(const char* aFontPath, short aFontWidth);
		ID3D11ShaderResourceView* GetShaderResource();
	private:

		//CRectangleBinPack myPacker;

		ID3D11Device* myDevice;
		FT_LibraryRec_* myLibrary;
		FT_FaceRec_* myFace;
		const char* myFontPath;
		short myFontWidth;
		int* myAtlas;
		ID3D11ShaderResourceView* myAtlasView;


	};
};