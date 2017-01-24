#pragma once
#include "Circle.h"
struct TreeNode;
class TreeDweller
{
public:
	TreeDweller();
	~TreeDweller();


	void Update();

	void SetPosition(const Vector2f& aPosition);
	inline Vector2f& GetPosition();
	inline Circle& GetCircle();

	void SetNode(TreeNode* aNode);
	void SetDepth(int aDepth);
	void SetVisible(bool aIsVisible);
	inline bool GetVisible();


	inline TreeNode* GetNode();
	inline const int GetDepth() const;
	inline const float GetRadius() const;

private:

	void ClampToWindow();

	Vector2f myPosition;
	Vector2f myVelocity;
	TreeNode *myNode;
	Circle myCircle;

	bool myIsVisible;
	int myDepth;
	

};


Vector2f& TreeDweller::GetPosition()
{
	return myPosition;
}

Circle& TreeDweller::GetCircle()
{
	return myCircle;
}

TreeNode* TreeDweller::GetNode()
{
	return myNode;
}

const int TreeDweller::GetDepth() const
{
	return myDepth;
}

bool TreeDweller::GetVisible()
{
	return myIsVisible;
}

const float TreeDweller::GetRadius() const
{
	return myCircle.GetRadius();
}