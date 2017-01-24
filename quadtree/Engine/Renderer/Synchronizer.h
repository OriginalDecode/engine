#pragma once
#include "RenderCommand.h"
#include "DataStructures\GrowingArray.h"
#include "DataStructures\StaticArray.h"
#include "Math\Matrix\Matrix.h"


class Synchronizer
{
public:
	Synchronizer();
	~Synchronizer();


	void WaitForRender();
	void WaitForLogic();
	void RenderIsDone();
	void LogicIsDone();

	void SwapBuffer();
	void Clear();
	void Quit();

	inline bool HasQuit() const;

	void AddRenderCommand(const RenderCommand& aRenderCommand);
	inline const CU::GrowingArray<RenderCommand>& GetRenderCommands() const;
	void AddCamera(const Matrix33f& aCameraMatrix);
	inline const Matrix33f& GetCamera() const;


private:
	volatile bool myLogicIsDoneFlag;
	volatile bool myRenderIsDoneFlag;
	volatile bool myQuitFlag;

	CU::StaticArray<CU::GrowingArray<RenderCommand>, 2> myCommandBuffer;
	CU::StaticArray<Matrix33f, 2> myCameraBuffer;
	int myCurrentBuffer;

};

inline bool Synchronizer::HasQuit() const
{
	return myQuitFlag;
}

inline const CU::GrowingArray<RenderCommand>& Synchronizer::GetRenderCommands() const
{
	return myCommandBuffer[myCurrentBuffer];
}

inline const Matrix33f& Synchronizer::GetCamera() const
{
	return myCameraBuffer[myCurrentBuffer];
}
