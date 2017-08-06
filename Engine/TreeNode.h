#pragma once
#include "TreeNodeBase.h"


class TreeNode : public TreeNodeBase
{
public:
	void Initiate(float halfwidth, Octree* octree) override;
	void Update(float dt, bool paused) override;
	void SetManager(NodeEntityManager* manager);
	NodeEntityManager* GetManager() { return m_NodeEntityManager; }
private:
	s32 m_Index = 0;


};