#pragma once


struct ID3D11ShaderResourceView;
namespace Snowblind
{
	struct Node
	{
		Node* myLeft;
		Node* myRight;
		int myX;
		int myY;
		int myWidth;
		int myHeight;
		ID3D11ShaderResourceView* myImage;
	};
	class CRectangleBinPack
	{
	public:
		CRectangleBinPack();
		~CRectangleBinPack();

		//Fixed init size
		void Initiate(int aWidth, int aHeight);
		Node* Insert(int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource);
		Node* GetRoot();
	private:
		Node* Insert(Node*, int aWidth, int aHeight, ID3D11ShaderResourceView* aShaderResource);
		Node myRoot;

	};
};