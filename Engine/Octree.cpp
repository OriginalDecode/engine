#include "stdafx.h"
#include "Octree.h"
void Octree::Initiate(CU::Vector3f world_position, float world_half_width)
{
	m_Position = world_position;
	m_HalfWidth = world_half_width;

	m_Root.Initiate(m_HalfWidth);
	m_Root.SetPosition(m_Position);

	/*TreeNode node;
	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(128.f, 0.f, 128.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(384.f, 0.f, 128.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(128.f, 0.f, 384.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());

	node.Initiate(128.f);
	node.SetPosition(CU::Vector3f(384.f, 0.f, 384.f));
	tempchildern.Add(node);
	m_Root.AddChild(tempchildern.GetLast());*/
}

void Octree::AddDwellers(const CU::GrowingArray<TreeDweller*>& dwellers)
{
	for (TreeDweller* dweller : dwellers)
	{
		InsertDweller(&m_Root, dweller, 1);
	}

	/*m_Root.AddEntity(dwellers[0], 0);
	m_Root.AddEntity(dwellers[1], 1);
	m_Root.AddEntity(dwellers[2], 2);
	m_Root.AddEntity(dwellers[3], 3);
*/
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
