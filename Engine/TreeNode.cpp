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
	m_HalfWidth = halfwidth;
	m_Octree = octree;

	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();
	if (m_Depth > 0)
	{
		if (m_Depth == 1)
			m_NodeEntityManager = Engine::GetInstance()->GetEntityManager().RequestManager();
		else if (m_Parent)
			m_NodeEntityManager = static_cast<TreeNode*>(m_Parent)->GetManager();
	}

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

}

void TreeNode::SetManager(NodeEntityManager* manager)
{
	m_NodeEntityManager = manager;
}
