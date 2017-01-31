#include "stdafx.h"
#include "Octree.h"
void Octree::Initiate(CU::Vector3f world_position, float world_half_width)
{
	m_Position = world_position;
	m_HalfWidth = world_half_width;

	m_Root.Initiate(m_HalfWidth);
	m_Root.SetPosition(m_Position);

}

void Octree::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	for (TreeDweller* dweller : dwellers)
	{
		InsertDweller(&m_Root, dweller, 1);
	}
}


void Octree::Update(float dt)
{
	m_Root.Update(dt);
}

#define MAX_DEPTH 8
void Octree::InsertDweller(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	if (depth >= MAX_DEPTH)
	{
		node->AddEntity(dweller);
		dweller->SetFirstNode(node);
		dweller->SetDepth(depth);
		return;
	}

	int index = 0;
	bool straddle = false; //what is straddle?

	for (s32 i = 0; i < 3; i++)
	{
		float delta = 0;

		if (i == 0)
		{
			delta = dweller->GetPosition().x - node->GetPosition().x;
		}
		else if (i == 1)
		{
			delta = dweller->GetPosition().y - node->GetPosition().y;
		}
		else if (i == 2)
		{
			delta = dweller->GetPosition().z - node->GetPosition().z;
		}

		if (abs(delta) + node->GetHalfWidth() /** (myLooseness - 1.f)*/ <= 5.f/*dweller->GetObjectCullingRadius()*/)
		{
			straddle = true;
			break;
		}

		if (delta > 0.f)
		{
			index |= (1 << i);
		}
	}

	if (straddle == false && depth < MAX_DEPTH - 1)
	{
		if (!node->GetChildByIndex(index))
		{
			node->AddChild(CreateNode(node->GetPosition(), node->GetHalfWidth(), index), index);
			node->SetDepth(depth);

		}

		InsertDweller(node->GetChildByIndex(index), dweller, depth + 1);

	}
	else
	{
		node->AddEntity(dweller);
		dweller->SetFirstNode(node);
		dweller->SetDepth(depth);
	}
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

	CU::Vector3f pos(center);
	pos.x += dir.x * halfwidth / 2.f;
	pos.y += dir.y * halfwidth / 2.f;
	pos.z += dir.z * halfwidth / 2.f;

	TreeNode* node = new TreeNode;
	node->Initiate(halfwidth / 2.f);
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

	const float cLeft = position.x - 4.f;
	const float cRight = position.x + 4.f;

	const float cTop = position.y + 4.f;
	const float cBottom = position.y - 4.f;

	const float cBack = position.z + 4.f;
	const float cFront = position.z - 4.f;

	return (cLeft > bLeft && cRight < bRight && cTop > bTop && cBottom < bBottom && cBottom > bBack && cFront < bFront);
}

void Octree::MoveUp(TreeNode* node, TreeDweller* dweller, s32 depth)
{
	if (NodeCollision(node, dweller) || node == &m_Root)
	{
		InsertDweller(node, dweller, depth);
		return;
	}

	MoveUp(node->GetParent(), dweller, depth - 1);

}

bool Octree::RemoveNode(TreeNode* node)
{
	if (node->GetEntityCount() > 0)
		return false;

	bool empty = true;
	for (s32 i = 0; i < 8; ++i)
	{
		if (node->GetChildByIndex(i) != nullptr)
		{
			empty = RemoveNode(node->GetChildByIndex(i));
			if (!empty)
				return empty;
		}
	}

	if (empty)
	{
		for (s32 i = 0; i < 8; i++)
		{
			if (node->GetParent()->GetChildByIndex(i) == node)
			{
				node->GetParent()->AddChild(nullptr, i);
			}
		}

		node->AddParent(nullptr);
		delete node;
		node = nullptr;
		return true;
	}
	return false;
}
