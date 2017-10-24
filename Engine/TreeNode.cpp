#include "stdafx.h"
#include "TreeNode.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Octree.h"
#include <ComponentFilter.h>
#include <TranslationComponent.h>
#include "Engine.h"
#include <Engine/NodeEntityManager.h>

void TreeNode::Initiate(float halfwidth, Octree* octree)
{
	static int tree_node = 0;
	std::stringstream ss;
	ss << "TreeNode : " << tree_node++;
	m_Name = ss.str();
	m_HalfWidth = halfwidth;
	m_Octree = octree;

#if !defined(_PROFILE) && !defined(_FINAL)
	debug::DebugHandle::GetInstance()->AddValueToPrint(&m_DwellerCount);
#endif
	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	if (m_Depth > 0)
		m_NodeEntityManager = Engine::GetInstance()->GetEntityManager().RequestManager(this);


	

	for (TreeNodeBase* child : m_Children)
	{
		child = nullptr;
	}
}

void TreeNode::Update(float dt, bool paused)
{
	TreeNodeBase::Update(dt, paused);

	for (TreeNodeBase* node : m_Children)
	{
		if (!node)
			continue;

		PROFILE_BLOCK("Child Node Update");
		node->Update(dt, paused);
		PROFILE_BLOCK_END;

	}
	m_Parent->CopyToParent(m_Lines);

}

void TreeNode::SetManager(NodeEntityManager* manager)
{
	m_NodeEntityManager = manager;
}
