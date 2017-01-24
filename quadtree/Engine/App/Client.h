#pragma once

#include <hgefont.h>
#include "Renderer\RenderCommand.h"
#include "Renderer\Renderer.h"
#include "Renderer\Synchronizer.h"
#include "Actor.h"
#include "QuadTree.h"
class TreeDweller;
class Client
{
public:
	Client();
	Client(HGE& aHGE, Synchronizer& aSynchronizer);
	~Client();

	bool Initiate(HINSTANCE aHInstance, HWND aHWND, float aWindowsWidth, float aWindowHeight);
	bool Update();

private:
	void Debug();
	void InitDwellers();

	void Render();
	void Logic();
	void UpdateSubsystems();
	void UpdatePlayerPoV();
	void DwellersInsideFrustum(CU::GrowingArray<TreeDweller*>& aDwellerList);
	CU::GrowingArray<TreeDweller*> CullNodes(TreeNode* aNode);
	bool NodeVsAABB(TreeNode* aNode, AABB& aBoundingBox);

	Client& operator=(const Client&) = delete;
	


	HGE& myHGE;
	Synchronizer& mySynchronizer;
	CU::GrowingArray<TreeDweller> myDwellers;
	bool myQuit;

	Actor myActor;
	
	QuadTree myQuadTree;

	hgeSprite* mySprite;
	hgeSprite* mySeenSprite;
	HTEXTURE myTexture;
	HTEXTURE myInvisTexture;


	float myDelta;
	float myAverageFPS;
};

