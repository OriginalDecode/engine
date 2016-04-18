#pragma once
#include <DataStructures/StaticArray.h>
#include <unordered_map>
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

	struct SCharData
	{
		CU::Math::Vector2<float> myTopLeftUV;
		short myWidth;
		short myHeight;
		char myChar;
	};

	struct SFontData
	{
		ID3D11ShaderResourceView* myAtlasView;
		int myAtlasWidth; //Should be dynamic?
		int myAtlasHeight; //Dynamic?
		FT_FaceRec_* myFaceData;
		std::unordered_map<char, SCharData> myCharData;
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
		ID3D11Device* myDevice;
		FT_LibraryRec_* myLibrary;
		FT_FaceRec_* myFace; //Create more of these if you want more fonts at the same time.
		const char* myFontPath;
		short myFontWidth;
		int* myAtlas;
		ID3D11ShaderResourceView* myAtlasView; //Will have to


	};
};

