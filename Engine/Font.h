#pragma once
#include "VertexStructs.h"
namespace Snowblind
{
	struct SFontData;
	class CFont
	{
	public:
		CFont();
		~CFont();

		void LoadFont(const char* aFilepath, int aSize);

		void SetText(const std::string& aText);
		const std::string& GetText() const;


	private:

		void UpdateBuffer();

		SFontData* myData;
		std::string myText;
		CU::GrowingArray<SVertexTypePosUV> myVertices;
		CU::GrowingArray<int> myIndices;
	};
}