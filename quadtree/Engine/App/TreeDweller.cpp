#include "TreeDweller.h"
#include "Misc\Megaton.h"

TreeDweller::TreeDweller()
{
	
	myCircle.SetRadius(24.f);
	myDepth = 4;
}


TreeDweller::~TreeDweller()
{
}

void TreeDweller::Update()
{

	if (INPUT_HANDLER->KeyClick(S) == true)
	{
		if (rand() % 2 == 0)
		{
			myVelocity = { static_cast<float>(rand() % 50 + 50.f), static_cast<float>(rand() % 50 + 50.f) };
		}
		else
		{
			myVelocity = { static_cast<float>(rand() % 50 - 50.f), static_cast<float>(rand() % 50 - 50.f) };

		}

	}
	if (INPUT_HANDLER->KeyClick(D) == true)
	{
		myVelocity = 0;
	}

	myPosition += myVelocity*TIME_HANDLER->GetDeltaTime();
	myCircle.SetCenterPosition(myPosition);
	if (myPosition.x <= 50.f || myPosition.y <= 50.f)
		myPosition = { 51.f, 51.f };
	if (myPosition.x >= 1075.f || myPosition.y >= 850.f)
		myPosition = { 1074.f, 849.f };
	ClampToWindow(); //Incorrect name of functions, should be ClampToRoot or similar.

}

void TreeDweller::SetPosition(const Vector2f& aPosition)
{
	myPosition = aPosition;
	myCircle.SetCenterPosition(myPosition);
}

void TreeDweller::SetNode(TreeNode* aNode)
{
	myNode = aNode;
}

void TreeDweller::SetDepth(int aDepth)
{
	myDepth = aDepth;
}

void TreeDweller::ClampToWindow()
{
	
	//Clampy clamp
	if (myPosition.x + myCircle.GetRadius() >= 1074.f || myPosition.x - myCircle.GetRadius() <= 276.f)
		myVelocity.x = -myVelocity.x;

	if (myPosition.y + myCircle.GetRadius() >= 849.f || myPosition.y - myCircle.GetRadius() <= 51.f)
		myVelocity.y = -myVelocity.y;



}

void TreeDweller::SetVisible(bool aMode)
{
	myIsVisible = aMode;
}