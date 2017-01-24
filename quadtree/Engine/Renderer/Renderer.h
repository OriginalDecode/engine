#pragma once
#include <hgefont.h>
#include "DataStructures\GrowingArray.h"
#include "Math\Vector\Vector.h"

#include "RenderCommand.h"

class HGE;
class Synchronizer;

class Renderer
{
public:
	Renderer(HGE& aHGE, Synchronizer& aSychronizer);
	Renderer(const Renderer &);
	~Renderer();
	void Initiate();
	void Render();



private:

	void DrawLine(Vector2f aStart, Vector2f aEnd, DWORD aColour = 0xFFFFFFFF);
	void DrawLine(CU::Line<float>& aLine, DWORD aColour = 0xFFFFFFFF);
	void DrawCircle(Vector2f anOriginPosition, float aRadius, int aSegmentAmount, DWORD aColour = 0xFFFFFFFF);
	void DrawBox(Vector2f anOriginPosition, float aWidth, float aHeight, int aPointCount, DWORD aColour = 0xFFFFFFFF);
	void DrawTriangle(Vector2f anOriginPosition, float aDistance, float aWidth, DWORD aColour = 0xFFFFFFFF);
	
	
	
	
	
	
	HGE& myHGE;
	Synchronizer& mySynchronizer;
	hgeFont myFont;
	Vector2f myResolution;
	CU::GrowingArray<RenderCommand> myTextBuffer;
	CU::GrowingArray<RenderCommand> myForcedTextBuffer;
	int myMultiplier;


};

