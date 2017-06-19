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
	m_NodeEntityManager = Engine::GetInstance()->GetEntityManager().RequestManager();

	
	for (TreeNodeBase* child : m_Children)
	{
		child = nullptr;
	}
}

void TreeNode::Update(float dt)
{
	TreeNodeBase::Update(dt);

	for (TreeNodeBase* node : m_Children)
	{
		if (!node)
			continue;

#ifdef _PROFILE
			EASY_BLOCK("Child Node Update");
#endif
			node->Update(dt);
#ifdef _PROFILE
			EASY_END_BLOCK;
#endif
		
	}

}
