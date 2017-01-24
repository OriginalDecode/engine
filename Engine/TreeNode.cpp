#include "stdafx.h"
#include "TreeNode.h"


void TreeNode::Initiate(float halfwidth)
{
	m_HalfWidth = halfwidth;
}

void TreeNode::AddChild(TreeNode& child_node)
{
	child_node.AddParent(*this);
	m_Children.Add(&child_node);
}

void TreeNode::AddParent(TreeNode& parent_node)
{
	m_Parent = &parent_node;
}

void TreeNode::AddEntity(Entity entity)
{
	//Check if the entity is in a deeper level than parent
}

void TreeNode::SetPosition(CU::Vector3f position)
{
	m_CenterPosition = position;
}

void TreeNode::Update(float dt)
{
	for (TreeNode* node : m_Children)
	{
		node->Update(dt);
	}
}
