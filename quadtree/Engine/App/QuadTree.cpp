#include "QuadTree.h"
#include "Synchronizer.h"
#include "AABB.h"

TreeNode::TreeNode()
{
	myHalfWidth = 0;
	for (int i = 0; i < 4; ++i)
	{
		myChildren[i] = nullptr;
	}
	myParentNode = nullptr;
	myDepth = 0;
	myDwellers.Init(8);
}


QuadTree::QuadTree()
{
}


QuadTree::~QuadTree()
{
	delete myRoot;
	myRoot = nullptr;
}

void QuadTree::Update(DwellerList& aDwellerList)
{
	for (int i = 0; i < aDwellerList.Size(); ++i)
	{
		TreeDweller* currentDweller = aDwellerList[i];
		currentDweller->GetNode()->myDwellers.RemoveCyclic(currentDweller);

		TreeNode* oldNode = currentDweller->GetNode();

		MoveUp(currentDweller->GetNode(), currentDweller, currentDweller->GetDepth());

		if (currentDweller->GetNode() != oldNode)
		{
			Remove(oldNode);
		}
	}
}

void QuadTree::Render(Synchronizer& aSynchronizer)
{
	RenderNode(aSynchronizer, myRoot);
}

bool QuadTree::Remove(TreeNode* aNode)
{
	if (aNode->myDwellers.Size() > 0) //No need to check an empty node
	{
		return false;
	}

	bool childIsEmpty = true;

	for (int i = 0; i < 4; ++i)
	{
		if (aNode->myChildren[i] != nullptr)
		{
			childIsEmpty = Remove(aNode->myChildren[i]); //Check if the child is empty
			if (childIsEmpty == false)
			{
				return false;
			}
		}
	}

	if (childIsEmpty == true)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (aNode->myParentNode->myChildren[i] == aNode)
			{
				aNode->myParentNode->myChildren[i] = nullptr;
			}
		}

		//Set aNodes parent to nullptr
		//Then delete it
		//Then set it as a nullptr
		//Then return true
		aNode->myParentNode = nullptr;
		delete aNode;
		aNode = nullptr;
		return true;

	}
	//Else false
	return false;

}

void QuadTree::CreateRoot(const Vector2f& aCenter, const float aHalfWidth)
{
	myRoot = new TreeNode;

	myRoot->myHalfWidth = aHalfWidth;
	myRoot->myPosition = aCenter;

	for (int i = 0; i < 4; ++i)
	{
		myRoot->myChildren[i] = nullptr;
	}

	myRoot->myParentNode = nullptr;
}

TreeNode* QuadTree::CreateNode(const Vector2f& aCenter, float aHalfWidth, const int aNodeIndex)
{
	Vector2f newCenter;
	float newHalfWidth = aHalfWidth * 0.5f;
	switch (aNodeIndex)
	{
	case 0:
		newCenter = { aCenter.x - newHalfWidth, aCenter.y - newHalfWidth };
		break;
	case 1:
		newCenter = { aCenter.x + newHalfWidth, aCenter.y - newHalfWidth };
		break;
	case 2:
		newCenter = { aCenter.x - newHalfWidth, aCenter.y + newHalfWidth };
		break;
	case 3:
		newCenter = { aCenter.x + newHalfWidth, aCenter.y + newHalfWidth };
		break;
	default:
		break;
	}

	TreeNode* toAdd = new TreeNode();
	toAdd->myPosition = newCenter;
	toAdd->myHalfWidth = newHalfWidth;

	return toAdd;
}

void QuadTree::InsertObject(TreeDweller* anObject)
{
	InsertObject(myRoot, anObject, 1);
}

void QuadTree::InsertObject(TreeNode* aTreeNode, TreeDweller* anObject, int aCurrentDepth)
{
	if (aCurrentDepth >= 4)
	{
		aTreeNode->myDwellers.Add(anObject);
		anObject->SetNode(aTreeNode);
		anObject->SetDepth(aCurrentDepth);
		return;
	}

	int index = 0;
	bool straddle = false;
	
	for (s32 i = 0;)
	{
	}



	if (straddle == false)
	{
		if (aTreeNode->myChildren[index] == nullptr)
		{
			aTreeNode->myChildren[index] = CreateNode(aTreeNode->myPosition, aTreeNode->myHalfWidth, index);
			aTreeNode->myChildren[index]->myParentNode = aTreeNode;
			aTreeNode->myChildren[index]->myDepth = aCurrentDepth;
			//aTreeNode->myDwellers.Add(anObject);
		}

		InsertObject(aTreeNode->myChildren[index], anObject, aCurrentDepth + 1);
	}
	else
	{
		aTreeNode->myDwellers.Add(anObject);
		anObject->SetNode(aTreeNode);
		anObject->SetDepth(aCurrentDepth);
	}

}

void QuadTree::MoveUp(TreeNode* aNode, TreeDweller* anObject, int aDepth)
{

	if (NodeCollision(aNode, anObject) == true || aNode == myRoot)
	{
		InsertObject(aNode, anObject, aDepth);
	}
	else
	{
		MoveUp(aNode->myParentNode, anObject, aDepth - 1);
	}

}

bool QuadTree::NodeCollision(TreeNode* aNode, TreeDweller* anObject)
{	//bX
	float boxLeft = aNode->myPosition.x - aNode->myHalfWidth;
	float boxRight = aNode->myPosition.x + aNode->myHalfWidth;

	//bY
	float boxTop = aNode->myPosition.y - aNode->myHalfWidth;
	float boxBottom = aNode->myPosition.y + aNode->myHalfWidth;

	//cX
	float circleLeft = anObject->GetPosition().x - anObject->GetCircle().GetRadius();
	float circleRight = anObject->GetPosition().x + anObject->GetCircle().GetRadius();

	//cY
	float circleTop = anObject->GetPosition().y - anObject->GetCircle().GetRadius();
	float circleBottom = anObject->GetPosition().y + anObject->GetCircle().GetRadius();

	//Too many floats...

	if (((circleLeft > boxLeft) && (circleRight < boxRight))
		&& ((circleTop > boxTop) && (circleBottom < boxBottom)))
		return true;

	return false;

}

void QuadTree::RenderNode(Synchronizer &aSynchronizer, TreeNode* aNode, DWORD aColour)
{
	if (aNode == nullptr)
		return;

	Vector4f rect;
	rect.x = aNode->myPosition.x - aNode->myHalfWidth;
	rect.y = aNode->myPosition.y - aNode->myHalfWidth;
	rect.z = aNode->myPosition.x + aNode->myHalfWidth;
	rect.w = aNode->myPosition.y + aNode->myHalfWidth;

	DWORD colour = 0xFFFFFFFF;
	//Split them up, easier to see errors
	switch (aNode->myDepth)
	{
	case 0:
		colour = 0xFFFFFFFF;
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.y), Vector2f(rect.z, rect.y), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.y), Vector2f(rect.z, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.w), Vector2f(rect.x, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.w), Vector2f(rect.x, rect.y), colour));
		aSynchronizer.AddRenderCommand(RenderCommand({ rect.x, rect.y }, "1"));
		break;
	case 1:
		colour = 0xFF00FF00;
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.y), Vector2f(rect.z, rect.y), colour));
		aSynchronizer.AddRenderCommand(RenderCommand({ rect.x, rect.y }, "2"));

		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.y), Vector2f(rect.z, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.w), Vector2f(rect.x, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.w), Vector2f(rect.x, rect.y), colour));

		break;
	case 2:
		colour = 0xFF0000FF;
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.y), Vector2f(rect.z, rect.y), colour));
		aSynchronizer.AddRenderCommand(RenderCommand({ rect.x, rect.y }, "3"));

		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.y), Vector2f(rect.z, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.w), Vector2f(rect.x, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.w), Vector2f(rect.x, rect.y), colour));
		break;
	case 3:
		colour = 0xFF002266;
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.y), Vector2f(rect.z, rect.y), colour));
		aSynchronizer.AddRenderCommand(RenderCommand({ rect.x, rect.y }, "4"));

		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.y), Vector2f(rect.z, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.z, rect.w), Vector2f(rect.x, rect.w), colour));
		aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.w), Vector2f(rect.x, rect.y), colour));
		break;
	}

	//aSynchronizer.AddRenderCommand(RenderCommand(aNode->myPosition, -aNode->myHalfWidth, -aNode->myHalfWidth, 4, colour));
	//aSynchronizer.AddRenderCommand(RenderCommand(aNode->myPosition, aNode->myHalfWidth, -aNode->myHalfWidth, 4, colour));
	//aSynchronizer.AddRenderCommand(RenderCommand(aNode->myPosition, -aNode->myHalfWidth, aNode->myHalfWidth, 4, colour));
	//aSynchronizer.AddRenderCommand(RenderCommand(aNode->myPosition, aNode->myHalfWidth, aNode->myHalfWidth, 4, colour));



	/*aSynchronizer.AddRenderCommand(RenderCommand(Vector2f(rect.x, rect.y), Vector2f(rect.z, rect.y)));
	aSynchronizer.AddRenderCommand(RenderCommand(rect.x, rect.w));
	aSynchronizer.AddRenderCommand(RenderCommand(rect.y, rect.z));
	aSynchronizer.AddRenderCommand(RenderCommand(rect.z, rect.w));*/


	RenderNode(aSynchronizer, aNode->myChildren[0], aColour);
	RenderNode(aSynchronizer, aNode->myChildren[1], aColour);
	RenderNode(aSynchronizer, aNode->myChildren[2], aColour);
	RenderNode(aSynchronizer, aNode->myChildren[3], aColour);

}

bool QuadTree::NodeCollision(TreeNode* aNode, AABB& aBoundingBox)
{
	float width = aBoundingBox.GetMaxPosition().x - aBoundingBox.GetMinPosition().x;
	float height = aBoundingBox.GetMaxPosition().y - aBoundingBox.GetMinPosition().y;

	Vector2f nodeCenter = { aNode->myPosition.x, aNode->myPosition.y };
	Vector2f newCenter = { aBoundingBox.GetMinPosition().x + width  *0.5f, aBoundingBox.GetMinPosition().y + height  *0.5f };
	int absCenterX = abs(nodeCenter.x - newCenter.x);
	int absCenterY = abs(nodeCenter.y - newCenter.y);

	if (absCenterX > aNode->myHalfWidth + width *0.5f)
		return false;
	if (absCenterY > aNode->myHalfWidth + height *0.5f)
		return false;

	return true;
}

void QuadTree::DwellersInNode(TreeNode* aNode, AABB& aBoundingBox, DwellerList& aList)
{
	if (NodeCollision(aNode, aBoundingBox) == true)
	{
		for (int i = 0; i < aNode->myDwellers.Size(); ++i)
		{
			aList.Add(aNode->myDwellers[i]);
		}
		for (int i = 0; i < 4; ++i)
		{
			if (aNode->myChildren[i] != nullptr)
			{
				DwellersInNode(aNode->myChildren[i], aBoundingBox, aList);
			}
		}
	}

}