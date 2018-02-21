#include "stdafx.h"
#include "QuadTreeNode.h"


QuadTreeNode::QuadTreeNode(float halfwidth, CU::Vector4f pos)
	: m_Position(pos)
	, m_HalfWidth(halfwidth)
{
	m_AABB = cl::AABB(halfwidth, halfwidth, halfwidth, pos);
}

QuadTreeNode::~QuadTreeNode()
{
	m_Children.DeleteAll();
}

void QuadTreeNode::Update(const CU::Matrix44f& camera_pos)
{
	for (QuadTreeNode* node : m_Children)
	{
		if(!node)
			continue;

		node->Update(camera_pos);
	}

	if (m_AABB.Intersect(camera_pos.GetPosition()))
	{

	}

}

void QuadTreeNode::CreateNode(s32 index)
{
	if (m_Children[index])
		return;

	const float hw = m_HalfWidth * 0.5f;
	CU::Vector4f pos = m_Position;
	switch (index)
	{
		case 1:
		{
			pos.x = pos.x - hw;
			pos.z = pos.z - hw;
		} break;
		case 2:
		{
			pos.x = pos.x + hw;
			pos.z = pos.z - hw;
		} break;
		case 3:
		{
			pos.x = pos.x - hw;
			pos.z = pos.z + hw;
		} break;
		case 4:
		{
			pos.x = pos.x + hw;
			pos.z = pos.z + hw;
		} break;

		default:
			break;

	}
	
	m_Children[index] = new QuadTreeNode(hw, pos);
}

void QuadTreeNode::Insert(CU::Vector4f pos)
{
	if (m_Index >= 4)
		return;

	CreateNode(m_Index + 1);


}

