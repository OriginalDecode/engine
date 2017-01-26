#include "Client.h"
#include "DL_Debug.h"
#include "Misc\Megaton.h"
#include <sstream>
#include "TreeDweller.h"
#include "DataStructures\Stack.h"


#define OFFSET_X 275.f
#define OFFSET_Y 50.f
Client::Client(HGE& aHGE, Synchronizer& aSynchronizer) :
mySynchronizer(aSynchronizer),
myHGE(aHGE)
{
	myQuit = false;
}

Client::~Client()
{
	delete mySprite;
	mySprite = nullptr;

}

bool Client::Initiate(HINSTANCE aHInstance, HWND aHWND, float aWindowsWidth, float aWindowHeight)
{
	INPUT_HANDLER->Initiate(GetActiveWindow());
	myTexture = myHGE.Texture_Load("../Data/Assets/target.png");
	myInvisTexture = myHGE.Texture_Load("../Data/Assets/seen.png");
	mySprite = new hgeSprite(myTexture, 0, 0, 48.f, 48.f);
	mySeenSprite = new hgeSprite(myInvisTexture, 0, 0, 48.f, 48.f);
	mySeenSprite->SetHotSpot(24.f, 24.f);
	mySprite->SetHotSpot(24.f, 24.f);
	myActor.Initiate();
	InitDwellers();
	myQuadTree.CreateRoot({ OFFSET_X + (800 * 0.5f), OFFSET_Y + (800 * 0.5f) }, 800 * 0.5f);

	for (int i = 0; i < myDwellers.Size(); ++i)
	{
		myQuadTree.InsertObject(&myDwellers[i]);
	}

	myDelta = 0.16f;
	myAverageFPS = 0;
	return true;
}

void Client::InitDwellers()
{
	myDwellers.Init(33);
	for (int i = 0; i < myDwellers.Capacity(); ++i)
	{
		TreeDweller dweller;
		myDwellers.Add(dweller);

	}

	myDwellers[0].SetPosition(Vector2f(OFFSET_X + 80, OFFSET_Y + 100));
	myDwellers[1].SetPosition(Vector2f(OFFSET_X + 80, OFFSET_Y + 260));
	myDwellers[2].SetPosition(Vector2f(OFFSET_X + 80, OFFSET_Y + 420));
	myDwellers[3].SetPosition(Vector2f(OFFSET_X + 80, OFFSET_Y + 580));
	myDwellers[4].SetPosition(Vector2f(OFFSET_X + 80, OFFSET_Y + 750));

	myDwellers[5].SetPosition(Vector2f(OFFSET_X + 160, OFFSET_Y + 160));
	myDwellers[6].SetPosition(Vector2f(OFFSET_X + 160, OFFSET_Y + 320));
	myDwellers[7].SetPosition(Vector2f(OFFSET_X + 160, OFFSET_Y + 480));
	myDwellers[8].SetPosition(Vector2f(OFFSET_X + 160, OFFSET_Y + 640));

	myDwellers[9].SetPosition(Vector2f(OFFSET_X + 260, OFFSET_Y + 80));
	myDwellers[10].SetPosition(Vector2f(OFFSET_X + 260, OFFSET_Y + 240));
	myDwellers[11].SetPosition(Vector2f(OFFSET_X + 260, OFFSET_Y + 400));
	myDwellers[12].SetPosition(Vector2f(OFFSET_X + 260, OFFSET_Y + 560));
	myDwellers[13].SetPosition(Vector2f(OFFSET_X + 260, OFFSET_Y + 640));

	myDwellers[14].SetPosition(Vector2f(OFFSET_X + 380, OFFSET_Y + 160));
	myDwellers[15].SetPosition(Vector2f(OFFSET_X + 380, OFFSET_Y + 320));
	myDwellers[16].SetPosition(Vector2f(OFFSET_X + 380, OFFSET_Y + 480));
	myDwellers[17].SetPosition(Vector2f(OFFSET_X + 380, OFFSET_Y + 600));
	myDwellers[18].SetPosition(Vector2f(OFFSET_X + 380, OFFSET_Y + 720));

	myDwellers[19].SetPosition(Vector2f(OFFSET_X + 500, OFFSET_Y + 60));
	myDwellers[20].SetPosition(Vector2f(OFFSET_X + 500, OFFSET_Y + 260));
	myDwellers[21].SetPosition(Vector2f(OFFSET_X + 500, OFFSET_Y + 420));
	myDwellers[22].SetPosition(Vector2f(OFFSET_X + 500, OFFSET_Y + 580));
	myDwellers[23].SetPosition(Vector2f(OFFSET_X + 500, OFFSET_Y + 740));

	myDwellers[24].SetPosition(Vector2f(OFFSET_X + 600, OFFSET_Y + 120));
	myDwellers[25].SetPosition(Vector2f(OFFSET_X + 600, OFFSET_Y + 280));
	myDwellers[26].SetPosition(Vector2f(OFFSET_X + 600, OFFSET_Y + 440));
	myDwellers[27].SetPosition(Vector2f(OFFSET_X + 600, OFFSET_Y + 640));

	myDwellers[28].SetPosition(Vector2f(OFFSET_X + 720, OFFSET_Y + 60));
	myDwellers[29].SetPosition(Vector2f(OFFSET_X + 720, OFFSET_Y + 220));
	myDwellers[30].SetPosition(Vector2f(OFFSET_X + 720, OFFSET_Y + 380));
	myDwellers[31].SetPosition(Vector2f(OFFSET_X + 720, OFFSET_Y + 540));
	myDwellers[32].SetPosition(Vector2f(OFFSET_X + 720, OFFSET_Y + 720));

}

void Client::Logic()
{

	if (INPUT_HANDLER->MouseDown(LEFT) != true)
		myActor.LookAt(INPUT_HANDLER->GetMousePos());

	if (INPUT_HANDLER->MouseDown(LEFT) == true)
		myActor.MoveTo(INPUT_HANDLER->GetMousePos());

	for (int i = 0; i < myDwellers.Size(); ++i)
	{
		myDwellers[i].Update();
	}

	if (INPUT_HANDLER->KeyClick(R) == true)
	{
		for (int i = 0; i < myDwellers.Size(); ++i)
		{
			myDwellers[i].SetPosition(Vector2f(500.f, 500.f));
		}
	}
	CU::GrowingArray<TreeDweller*> updateDwellers(33);
	for (int i = 0; i < myDwellers.Size(); ++i)
	{
		updateDwellers.Add(&myDwellers[i]);
	}

	myQuadTree.Update(updateDwellers);

	DwellersInsideFrustum(CullNodes(myQuadTree.GetRoot()));

	

}

void Client::Render()
{
	if (GET_DEBUG == false)
	{
		mySynchronizer.AddRenderCommand(RenderCommand("\nPress [S] to Start movement of Dwellers"));
		mySynchronizer.AddRenderCommand(RenderCommand("\nPress [D] to Stop movement of Dwellers"));
		mySynchronizer.AddRenderCommand(RenderCommand("\nPress [R] to set Dwellers position to {500, 500}"));
	}
	else
	{
		mySynchronizer.AddRenderCommand(RenderCommand("Press [S] to Start movement of Dwellers"));
		mySynchronizer.AddRenderCommand(RenderCommand("Press [D] to Stop movement of Dwellers"));
		mySynchronizer.AddRenderCommand(RenderCommand("Press [R] to set Dwellers position to {500, 500}"));
	}

	for (int i = 0; i < myDwellers.Size(); ++i)
	{
		std::stringstream depth;

		if (myDwellers[i].GetVisible() == false)
		{
			mySynchronizer.AddRenderCommand(RenderCommand(myDwellers[i].GetPosition(), mySprite, RenderCommand::eSPACE::SCREEN));
			mySprite->SetColor(0x22FFFFFF);

		}
		else if (myDwellers[i].GetVisible() == true)
		{
			mySynchronizer.AddRenderCommand(RenderCommand(myDwellers[i].GetPosition(), mySeenSprite, RenderCommand::eSPACE::SCREEN));
			mySeenSprite->SetColor(0xFFFFFFFF);

		}
		depth << myDwellers[i].GetDepth();
		mySynchronizer.AddRenderCommand(RenderCommand(myDwellers[i].GetPosition(), depth.str().c_str()));

		if (GET_DEBUG == true)
		{
			mySynchronizer.AddRenderCommand(RenderCommand(myDwellers[i].GetPosition() - 24.f, 24.f, 12));
			mySynchronizer.AddRenderCommand(RenderCommand(myDwellers[i].GetPosition() - 24.f, 48.f, 48.f));
		}
	}
	myActor.Render(mySynchronizer);

	myQuadTree.Render(mySynchronizer);

}

bool Client::Update()
{
	UpdateSubsystems();
	if (INPUT_HANDLER->KeyClick(ESCAPE) == true)
	{
		mySynchronizer.Quit();
		myQuit = true;
	}

	if (INPUT_HANDLER->KeyClick(DIK_F2) == true)
	{
		SET_DEBUG(!GET_DEBUG);
	}



	Logic();
	Render();
	Debug();

	mySynchronizer.LogicIsDone();
	mySynchronizer.WaitForRender();

	return myQuit;
}

void Client::UpdateSubsystems()
{
	INPUT_HANDLER->Update();
	TIME_HANDLER->UpdateMaster();

}

void Client::Debug()
{
	if (GET_DEBUG == true)
	{
		std::stringstream cursorPos;
		cursorPos << "X : " << INPUT_HANDLER->GetMousePos().x;
		cursorPos << "	Y : " << INPUT_HANDLER->GetMousePos().y;
		mySynchronizer.AddRenderCommand(RenderCommand("Cursor Position"));
		mySynchronizer.AddRenderCommand(RenderCommand(cursorPos.str()));

		std::stringstream fps;
		fps << "FPS : " << TIME_HANDLER->GetFPS();
		mySynchronizer.AddRenderCommand(RenderCommand(fps.str()));

		std::stringstream averageFPS;
		if (myDelta <= 0.f)
		{
			myAverageFPS = 0;
			myAverageFPS = TIME_HANDLER->GetFPS();
			myDelta = 1.f;
		}
		else
			myDelta -= TIME_HANDLER->GetDeltaTime();
		averageFPS << "aFPS :  " << myAverageFPS;
		mySynchronizer.AddRenderCommand(RenderCommand(averageFPS.str()));
	}
}

void Client::DwellersInsideFrustum(CU::GrowingArray<TreeDweller*>& aDwellerList)
{
	CU::GrowingArray<TreeDweller*> dwellersWithinFrustum;

	for (int i = 0; i < aDwellerList.Size(); ++i)
	{
		
		if (myActor.GetFrustum().GetFrustum().Inside(aDwellerList[i]->GetPosition(), aDwellerList[i]->GetRadius() - 12.f) == false)
			aDwellerList[i]->SetVisible(false);
		else if (myActor.GetFrustum().GetFrustum().Inside(aDwellerList[i]->GetPosition(), aDwellerList[i]->GetRadius()) == true)
			aDwellerList[i]->SetVisible(true);

	}
}


CU::GrowingArray<TreeDweller*> Client::CullNodes(TreeNode* aNode)
{
	CU::GrowingArray<TreeDweller*> dwellersInNode;
	Stack<TreeNode*> childStack;
	childStack.Push(aNode);

	while (childStack.Size() > 0)
	{
		TreeNode* currNode = childStack.Pop();


		if (NodeVsAABB(currNode, myActor.GetFrustum().GetAABB()) == true)
		{
			for (int j = 0; j < currNode->myDwellers.Size(); ++j)
			{
				dwellersInNode.Add(currNode->myDwellers[j]);
			}

			for (int i = 0; i < 4; ++i)
			{
				if (currNode->myChildren[i] != nullptr)
				{
					childStack.Push(currNode->myChildren[i]);
				}
			}
		}
	}

	return dwellersInNode;

}

bool Client::NodeVsAABB(TreeNode* aNode, AABB& aBoundingBox)
{

	float width = aBoundingBox.GetMaxPosition().x - aBoundingBox.GetMinPosition().x;
	float height = aBoundingBox.GetMaxPosition().y - aBoundingBox.GetMinPosition().y;

	Vector2f center = aNode->myPosition;

	Vector2f newCenter = { aBoundingBox.GetMinPosition().x + width  * 0.5f, aBoundingBox.GetMinPosition().y + height * 0.5f };


	if (abs(center.x - newCenter.x) > aNode->myHalfWidth + width * 0.5f) return false;
	if (abs(center.y - newCenter.y) > aNode->myHalfWidth + height * 0.5f) return false;


	return true;
}