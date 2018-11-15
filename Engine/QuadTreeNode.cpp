#include "stdafx.h"
#include "QuadTreeNode.h"


QuadTreeNode::QuadTreeNode(float halfwidth, CU::Vector4f pos)
	: m_Position(pos)
	, m_HalfWidth(halfwidth)
{
	m_AABB = cl::AABB(halfwidth, halfwidth, halfwidth, cl::AsVector3(pos));

	for (s32 i = 0; i < 4; i++)
	{
		m_Children[i] = nullptr;
	}

}

QuadTreeNode::QuadTreeNode(float halfwidth, CU::Vector4f pos, s32 index)
	: m_Position(pos)
	, m_HalfWidth(halfwidth)
	, m_Index(index)
{
	m_AABB = cl::AABB(halfwidth, halfwidth, halfwidth, cl::AsVector3(pos));

	for (s32 i = 0; i < 4; i++)
	{
		m_Children[i] = nullptr;
	}
}

QuadTreeNode::~QuadTreeNode()
{
	m_Children.DeleteAll();
}

void QuadTreeNode::Update(const CU::Matrix44f& camera_pos)
{
	if (!m_AABB.Intersect(camera_pos.GetPosition()))
	{
		MoveUp(camera_pos.GetPosition());
	}

	for (QuadTreeNode* node : m_Children)
	{
		if(!node)
			continue;

		node->Update(camera_pos);
	}
}

void QuadTreeNode::CreateNode(CU::Vector4f pos, s32 index)
{
	if (m_Children[index])
		return;

	const float hw = m_HalfWidth * 0.5f;
	CU::Vector4f new_pos = m_Position;
	switch (index)
	{
		case 0:
		{
			new_pos.x = new_pos.x - hw;
			new_pos.z = new_pos.z - hw;
		} break;
		case 1:
		{
			new_pos.x = new_pos.x + hw;
			new_pos.z = new_pos.z - hw;
		} break;
		case 2:
		{
			new_pos.x = new_pos.x - hw;
			new_pos.z = new_pos.z + hw;
		} break;
		case 3:
		{
			new_pos.x = new_pos.x + hw;
			new_pos.z = new_pos.z + hw;
		} break;

		default:
			break;

	}
	
	m_Children[index] = new QuadTreeNode(hw, new_pos, index);
	m_Children[index]->Insert(pos);

}

void QuadTreeNode::Insert(CU::Vector4f pos)
{
	if (m_Index+1 >= 4)
		return;

	CreateNode(pos , m_Index + 1);
}

void QuadTreeNode::MoveUp(CU::Vector4f pos)
{
	if (m_Index > 0)
		MoveUp(pos);
	else
		Insert(pos);


}

