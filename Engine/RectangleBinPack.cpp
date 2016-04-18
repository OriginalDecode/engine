#include "stdafx.h"
#include "RectangleBinPack.h"

namespace Snowblind
{

	CRectangleBinPack::CRectangleBinPack()
	{

	}

	CRectangleBinPack::~CRectangleBinPack()
	{

	}

	void CRectangleBinPack::Initiate(int aWidth, int aHeight)
	{
		myRoot.myHeight = aHeight;
		myRoot.myWidth = aWidth;
		myRoot.myLeft = nullptr;
		myRoot.myRight = nullptr;
		myRoot.myImage = nullptr;
		myRoot.myX = 0;
		myRoot.myY = 0;
	}

	SNode* CRectangleBinPack::Insert(int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource)
	{
		return Insert(&myRoot, aWidth, aHeight, aShaderResource);
	}

	SNode* CRectangleBinPack::Insert(SNode* aNode, int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource)
	{
		if (aNode->myLeft || aNode->myRight)
		{
			if (aNode->myLeft)
			{
				SNode *newNode = Insert(aNode->myLeft, aWidth, aHeight, aShaderResource);
				if (newNode)
				{
					return newNode;
				}
			}
			if (aNode->myRight)
			{
				SNode *newNode = Insert(aNode->myRight, aWidth, aHeight, aShaderResource);
				if (newNode)
				{
					return newNode;
				}
			}
			return nullptr;
		}

		if (aWidth > aNode->myWidth || aHeight > aNode->myHeight)
			return 0;

		int w = aNode->myWidth - aWidth;
		int h = aNode->myHeight - aHeight;

		aNode->myLeft = new SNode();
		aNode->myRight = new SNode();
		aNode->myImage = aShaderResource;

		if (w <= h) 
		{
			aNode->myLeft->myX = aNode->myX + aWidth;
			aNode->myLeft->myY = aNode->myWidth;
			aNode->myLeft->myWidth = w;
			aNode->myLeft->myHeight = aHeight;

			aNode->myRight->myX = aNode->myX;
			aNode->myRight->myY = aNode->myY + aHeight;
			aNode->myRight->myWidth = aNode->myWidth;
			aNode->myRight->myHeight = h;
		}
		else 
		{
			aNode->myLeft->myX = aNode->myX;
			aNode->myLeft->myY = aNode->myY + aHeight;
			aNode->myLeft->myWidth = aWidth;
			aNode->myLeft->myHeight = h;

			aNode->myRight->myX = aNode->myX + aWidth;
			aNode->myRight->myY = aNode->myY;
			aNode->myRight->myWidth = w;
			aNode->myRight->myHeight = aNode->myHeight;
		}

		aNode->myWidth = aWidth;
		aNode->myHeight = aHeight;
		return aNode;
	}

	SNode* CRectangleBinPack::GetRoot()
	{
		return &myRoot;
	}

	void SNode::Traverse()
	{
		DL_ASSERT("Not implemented!"); //Implement when needed to have good packing on font/texture.
	}

}