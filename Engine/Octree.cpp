#include "stdafx.h"
#include "Octree.h"
#include "RenderCommand.h"
#include "Engine.h"
#include "Synchronizer.h"

void Octree::Initiate(CU::Vector3f world_position, float world_half_width)
{
	m_Position = world_position;
	m_HalfWidth = world_half_width;

	m_Root.Initiate(m_HalfWidth, this);
	m_Root.SetPosition(m_Position);
	m_Root.SetDepth(0);
	m_Timer.CreateTimer();
}

void Octree::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	for (TreeDweller* dweller : dwellers)
	{
		//InsertDweller(&m_Root, dweller, 0);
		MoveDown(&m_Root, dweller, m_Root.GetDepth());
	}
}


void Octree::Update(float dt)
{
	for (TreeNode* node : m_GarbageNodes)
	{
		delete node;
	}
	m_GarbageNodes.RemoveAll();

	m_Root.Update(dt);
}

#define MAX_DEPTH 8
void Octree::MoveDown(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	TreeNode* remve_from = dweller->GetFirstNode();
	if (remve_from)
	{
		remve_from->RemoveEntity(dweller);
	}
	// Potentially this function is very very slow
	if (depth < 0)
		depth = 0;

	s32 index = 0;
	bool straddle = false; 

	const CU::Vector3f node_position = node->GetPosition();
	const CU::Vector3f dweller_position = dweller->GetPosition();
	const CU::Vector3f dweller_bounds = dweller->GetWHD();

	for (s32 i = 0; i < 3; i++)
	{
		float delta = 0.f;
		float bound_value = 0.f;
		if (i == 0)
		{
			delta = dweller_position.x - node_position.x;
			bound_value = dweller_bounds.x;
		}
		else if (i == 1)
		{
			delta = dweller_position.y - node_position.y;
			bound_value = dweller_bounds.y;
		}
		else if (i == 2)
		{
			delta = dweller_position.z - node_position.z;
			bound_value = dweller_bounds.z;
		}

		if (abs(delta) + node->GetHalfWidth() /* * (myLooseness - 1.f)*/ < bound_value)
		{
			straddle = true;
			break;
		}

		if (delta > 0.f)
		{
			index |= ( 1 << i );
		}
	}



	if (straddle == false && node->GetDepth() < MAX_DEPTH)
	{
		if (!node->GetChildByIndex(index))
		{
			TreeNode* child = CreateNode(node->GetPosition(), node->GetHalfWidth(), index);
			node->AddChild(child, index);
			child->SetDepth(depth);
		}

		MoveDown(node->GetChildByIndex(index), dweller, depth + 1);
	}
	else
	{
		InsertDweller(node->GetParent(), dweller, depth - 1);
	}
	
}

void Octree::InsertDweller(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	assert(!dweller->GetFirstNode() && "You fucked up!");
	node->AddEntity(dweller);
	dweller->SetFirstNode(node);
	dweller->SetDepth(node->GetDepth());
}

TreeNode* Octree::CreateNode(const CU::Vector3f& center, float halfwidth, s32 index)
{
	CU::Vector3i dir;

	switch (index)
	{
		case 0:
		dir = CU::Vector3i(-1, -1, -1);
		break;
		case 1:
		dir = CU::Vector3i(+1, -1, -1);
		break;
		case 2:
		dir = CU::Vector3i(-1, +1, -1);
		break;
		case 3:
		dir = CU::Vector3i(+1, +1, -1);
		break;
		case 4:
		dir = CU::Vector3i(-1, -1, +1);
		break;
		case 5:
		dir = CU::Vector3i(+1, -1, +1);
		break;
		case 6:
		dir = CU::Vector3i(-1, +1, +1);
		break;
		case 7:
		dir = CU::Vector3i(+1, +1, +1);
		break;
		default:
		DL_ASSERT("bad index!");
		break;
	}

	float new_halfwidth = halfwidth / 2.f;

	CU::Vector3f pos(center);
	pos.x += dir.x * new_halfwidth;
	pos.y += dir.y * new_halfwidth;
	pos.z += dir.z * new_halfwidth;

	TreeNode* node = new TreeNode;
	node->Initiate(new_halfwidth, this);
	node->SetPosition(pos);

	return node;
}

bool Octree::NodeCollision(TreeNode* node, TreeDweller* dweller)
{
	const CU::Vector3f position = node->GetPosition();
	const float halfwidth = node->GetHalfWidth();

	const float bLeft = position.x - halfwidth;
	const float bRight = position.x + halfwidth;

	const float bTop = position.y + halfwidth;
	const float bBottom = position.y - halfwidth;

	const float bBack = position.z + halfwidth;
	const float bFront = position.z - halfwidth;

	const CU::Vector3f dweller_position = dweller->GetPosition();
	const CU::Vector3f dweller_bounds = dweller->GetWHD();

	const float cLeft = dweller_position.x - dweller_bounds.x;
	const float cRight = dweller_position.x + dweller_bounds.x;

	const float cTop = dweller_position.y + dweller_bounds.y;
	const float cBottom = dweller_position.y - dweller_bounds.y;

	const float cBack = dweller_position.z + dweller_bounds.z;
	const float cFront = dweller_position.z - dweller_bounds.z;

	return (cLeft > bLeft 
			&& cRight < bRight 
			&& cTop > bTop 
			&& cBottom < bBottom 
			&& cBack > bBack 
			&& cFront < bFront);
}

void Octree::MoveUp(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	TreeNode* parent = node->GetParent();
	node->RemoveEntity(dweller);
	if (parent && !node->InsideNode(dweller))
	{
		MoveUp(parent, dweller, depth - 1);
		return;
	}
	else
		MoveDown(node, dweller, depth + 1);

}

void Octree::ToDelete(TreeNode* node)
{
	m_GarbageNodes.Add(node);
}

//bool Octree::RemoveNode(TreeNode* node)
//{
//	if (node->GetEntityCount() > 0)
//		return false;
//
//	bool empty = true;
//	for (s32 i = 0; i < 8; ++i)
//	{
//		if (node->GetChildByIndex(i) != nullptr)
//		{
//			empty = RemoveNode(node->GetChildByIndex(i));
//			if (!empty)
//				return empty;
//		}
//	}
//
//	if (empty)
//	{
//		for (s32 i = 0; i < 8; i++)
//		{
//			if (node->GetParent()->GetChildByIndex(i) == node)
//			{
//				node->GetParent()->AddChild(nullptr, i);
//			}
//		}
//
//		node->AddParent(nullptr);
//		delete node;
//		node = nullptr;
//		return true;
//	}
//	return false;
//}
