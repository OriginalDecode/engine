#pragma once


struct ID3D11ShaderResourceView;
namespace Hex
{
	struct SNode
	{
		SNode* myLeft;
		SNode* myRight;
		int myX;
		int myY;
		int myWidth;
		int myHeight;
		ID3D11ShaderResourceView* myImage;
		void Traverse();
	};
	class CRectangleBinPack
	{
	public:
		CRectangleBinPack();
		~CRectangleBinPack();

		//Fixed init size
		void Initiate(int aWidth, int aHeight);
		SNode* Insert(int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource);
		SNode* GetRoot();
	private:
		SNode* Insert(SNode*, int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource);
		SNode myRoot;

	};
};