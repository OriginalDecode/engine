#include "stdafx.h"
#include "RootTreeNode.h"
#include <Engine/Synchronizer.h>
void RootTreeNode::Initiate(float halfwidth, Octree* octree)
{
	m_HalfWidth = halfwidth;
	m_Octree = octree;

	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	m_NodeEntityManager = Engine::GetInstance()->GetEntityManager().RequestManager();

	m_Pool.Initiate("RootNode - Worker");

	for (TreeNodeBase* child : m_Children)
	{
		child = nullptr;
	}
}

void RootTreeNode::Update(float dt, bool paused)
{
#ifdef _PROFILE
	EASY_FUNCTION();
#endif
	if (paused)
		return;
	TreeNodeBase::Update(dt, paused);

	for (TreeNodeBase* node : m_Children)
	{
		if (!node)
			continue;

		m_Pool.AddWork(Work([=]() {
#ifdef _PROFILE
			EASY_BLOCK("Node Update Thread");
#endif
			node->Update(dt, paused);
#ifdef _PROFILE
			EASY_END_BLOCK;
#endif
	}));
}

	do
	{
		m_Pool.Update();
	} while (!m_Pool.CurrentWorkFinished()); //This cannot work if we start work and pop at the same time
}
