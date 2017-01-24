#pragma once
#include "Misc\Global.h"
#include "TreeDweller.h"
typedef CU::GrowingArray<TreeDweller*> DwellerList;
class HGE;
class hgeSprite;
class Synchronizer;
class AABB;

struct TreeNode
{
	TreeNode();
	Vector2f myPosition;
	float myHalfWidth;
	TreeNode *myChildren[4];
	TreeNode *myParentNode;
	DwellerList myDwellers;
	int myDepth;
};



class QuadTree
{
public:
	QuadTree();
	~QuadTree();

	TreeNode* BuildTree(Vector2f aCenter, float aHalfWidth, int aStopDepth);

	void InsertObject(TreeNode* aTreeNode, TreeDweller* anObject, int aCurrentDepth);
	void InsertObject(TreeDweller* anObject);
	void Update(DwellerList& aDwellerList);

	void Render(Synchronizer& aSynchronizer);


	TreeNode* CreateNode(const Vector2f& aCenter, float aHalfWidth, const int aNodeIndex);
	void CreateRoot(const Vector2f& aCenter, float aHalfWidth);

	inline TreeNode* GetRoot();

private:
	void RenderNode(Synchronizer& aSynchronizer, TreeNode* aNode, DWORD aColour = 0xFFFFFFFF);
	void MoveUp(TreeNode* aNode, TreeDweller* anObject, int aDepth);
	void DwellersInNode(TreeNode* aNode, AABB& aBoundingBox, DwellerList& aList);

	bool Remove(TreeNode* aNode);
	bool NodeCollision(TreeNode* aNode, AABB& aBoundingBox);
	bool NodeCollision(TreeNode* aNode, TreeDweller* anObject);



	HGE* myHGE;
	TreeNode* myRoot;
	hgeSprite* myRootSprite;
};


TreeNode* QuadTree::GetRoot()
{
	return myRoot;
}