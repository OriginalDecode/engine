#include "Synchronizer.h"

#include "DL_Debug.h"

#include <thread>

Synchronizer::Synchronizer()
{
	myLogicIsDoneFlag = false;
	myRenderIsDoneFlag = false;
	myQuitFlag = false;
	myCurrentBuffer = 0;
	myCommandBuffer[0].Init(16384);
	myCommandBuffer[1].Init(16384);
}


Synchronizer::~Synchronizer()
{
}


void Synchronizer::WaitForRender()
{
	while (myRenderIsDoneFlag == false);
	myRenderIsDoneFlag = false;
}

void Synchronizer::WaitForLogic()
{
	while (myLogicIsDoneFlag == false);
	myLogicIsDoneFlag = false;
}

void Synchronizer::RenderIsDone()
{
	myRenderIsDoneFlag = true;
}

void Synchronizer::LogicIsDone()
{
	myLogicIsDoneFlag = true;
}

void Synchronizer::SwapBuffer()
{
	myCommandBuffer[myCurrentBuffer].RemoveAll();
	myCurrentBuffer ^= 1; // ^= means xor, which is inequality. 

	/*
		You have A and B
		A and B are both off
		Resulting in a false output
		A starts outputting something
		This will Result in a True output
		If then B starts outputting as well
		The output will end up False again.		
	*/
}

void Synchronizer::Clear()
{
	myCommandBuffer[0].RemoveAll();
	myCommandBuffer[1].RemoveAll();

}

void Synchronizer::Quit()
{
	myLogicIsDoneFlag = true;
	myRenderIsDoneFlag = true;
	myQuitFlag = true;
}

void Synchronizer::AddRenderCommand(const RenderCommand& aRenderCommand)
{
	myCommandBuffer[myCurrentBuffer ^ 1].Add(aRenderCommand);
}

void Synchronizer::AddCamera(const Matrix33f& aCameraMatrix)
{
	myCameraBuffer[myCurrentBuffer ^ 1] = aCameraMatrix;
}