#include "stdafx.h"
#include "Font.h"
#include "FontManager.h"

namespace Snowblind
{
	CFont::CFont()
	{
	}

	CFont::~CFont()
	{
		delete myData;
		myData = nullptr;
	}

	

	void CFont::LoadFont(const char* aFilepath, int aSize)
	{
		myData = CEngine::GetInstance()->LoadFont(aFilepath, aSize);
	}

	void CFont::SetText(const std::string& aText)
	{
		if (myText != aText)
		{
			myText = aText;
			UpdateBuffer();
		}
	}

	const std::string& CFont::GetText() const
	{
		return myText;
	}

	void CFont::UpdateBuffer()
	{
		int count = myText.length();
		float drawX = 0.f;
		float drawY = 0.f;

		myVertices.RemoveAll();
		myIndices.RemoveAll();

		SVertexTypePosUV v;
		for (int i = 0, row = 0; i < count; i++)
		{
			if (myText[i] == '\n')
			{
				drawX = 0;
				drawY -= 33.f;
				row++;
				continue;
			}

			SCharData& charData = myData->myCharData[i];

			float left = drawX;
			float right = left + charData.myWidth;
			float top = drawY;
			float bottom = drawY + charData.myHeight;

			v.myPosition = { left, top };
			v.myUV = charData.myTopLeftUV;
			myVertices.Add(v);

			v.myPosition = { right, bottom };
			v.myUV = charData.myBottomRightUV;
			myVertices.Add(v);

			v.myPosition = { left, bottom };
			v.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
			myVertices.Add(v);

			v.myPosition = { right, top };
			v.myUV = { charData.myBottomRightUV.x,charData.myTopLeftUV.y };
			myVertices.Add(v);

			int startIndex = (i - row) * 4.f;

			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 1);
			myIndices.Add(startIndex + 2);

			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 3);
			myIndices.Add(startIndex + 1);


			//drawX += charData.myAdvanceX;


		}


	}

};