#pragma once
#include "TreeNodeBase.h"


class TreeNode : public TreeNodeBase
{
public:
	void Initiate(float halfwidth, Octree* octree) override;
	void Update(float dt) override;

private:
	s32 m_Index = 0;


};