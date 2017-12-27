#pragma once
#include "TreeNodeBase.h"
#include <CommonLib/Threadpool.h>
#include <Engine/LinearAllocator.h>
class RootTreeNode : public TreeNodeBase
{
public:
	~RootTreeNode() override;
	void Initiate(float halfwidth, Octree* octree) override;
	void Update(float dt, bool paused) override;
	bool IsWorking() { return m_Pool.CurrentWorkFinished(); }
private:
	Threadpool m_Pool;
	

};

