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
		MoveDown(&m_Root, dweller, m_Root.GetDepth() + 1);
	}
}


void Octree::AddDweller(TreeDweller* dweller)
{
	MoveDown(&m_Root, dweller, m_Root.GetDepth() + 1);
}

void Octree::Update(float dt)
{
	for (TreeNode* node : m_GarbageNodes)
	{
		delete node;
		node_count--;
	}
	m_GarbageNodes.RemoveAll();
	m_Root.Update(dt);
}

#define MAX_DEPTH 5
void Octree::MoveDown(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	TreeNode* remve_from = dweller->GetFirstNode();
	if (remve_from)
	{
		assert(false && "should be removed already!");
	}
	// Potentially this function is very very slow
	assert(depth >= 0 && "Depth was lower than 0?");

	s32 index = 0;
	bool straddle = false; 

	const CU::Vector3f node_position = node->GetPosition();
	const CU::Vector3f dweller_position = dweller->GetPosition();
	const CU::Vector3f dweller_bounds = dweller->GetWHD();

	float max_bound = 0.f;
	if (max_bound < dweller_bounds.x)
	{
		max_bound = dweller_bounds.x;
	}

	if (max_bound < dweller_bounds.x)
	{
		max_bound = dweller_bounds.x;
	}

	if (max_bound < dweller_bounds.x)
	{
		max_bound = dweller_bounds.x;
	}


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

		//without halfwidth we can't get more nodes?

		if (abs(delta) + node->GetHalfWidth() /* (1.f - 1.f)*/ <= max_bound)
		{
			straddle = true;
			break;
		}

		if (delta > 0.f)
		{
			index |= ( 1 << i );
		}
	}

	if (straddle == false && node->GetDepth() < MAX_DEPTH - 1)
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
		InsertDweller(node, dweller, depth);
	}
	
}

void Octree::InsertDweller(TreeNode* node, TreeDweller* dweller, s32 /*depth*/)
{
	assert(!dweller->GetFirstNode() && "You fucked up!");
	node->AddEntity(dweller);
	dweller->SetFirstNode(node);
	dweller->SetDepth(node->GetDepth());
}

TreeNode* Octree::CreateNode(const CU::Vector3f& center, float halfwidth, s32 index)
{
	node_count++;
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

void Octree::MoveUp(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	assert(depth >= 0 && "MoveUp : Depth was lower than 0?");
	TreeNode* parent = node->GetParent();
	node->RemoveEntity(dweller);
	if (parent && !node->InsideNode(dweller))
	{
		MoveUp(parent, dweller, depth - 1);
		return;
	}

	MoveDown(node, dweller, depth + 1);
}

void Octree::ToDelete(TreeNode* node)
{
	m_GarbageNodes.Add(node);
}

