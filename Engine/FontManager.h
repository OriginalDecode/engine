#pragma once
#include <DataStructures/StaticArray.h>
#include <unordered_map>
#include "RectangleBinPack.h"

struct ID3D11Device;
struct FT_LibraryRec_;
struct FT_FaceRec_;
struct ID3D11ShaderResourceView;

namespace Snowblind
{
	struct SRectangle
	{
		float left;
		float top;
		float right;
		float bottom;
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
		CRectangleBinPack myPacker;
		Node* myTopNode;
		ID3D11Device* myDevice;
		FT_LibraryRec_* myLibrary;
		FT_FaceRec_* myFace;
		const char* myFontPath;
		short myFontWidth;

		void BuildAtlas();

	};
};