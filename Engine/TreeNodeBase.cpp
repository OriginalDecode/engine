#include "stdafx.h"
#include "TreeNodeBase.h"
#include <Engine/Line3D.h>
#include <Engine/NodeEntityManager.h>
#include <Engine/Synchronizer.h>
#include <Engine/Octree.h>
#include <Engine/Engine.h>
#include <Engine/profile_defines.h>
#include <algorithm>
TreeNodeBase::TreeNodeBase()
{
	m_Dwellers.Init(16);
}

TreeNodeBase::~TreeNodeBase()
{
	Engine::GetInstance()->GetEntityManager().ReleaseManager(m_NodeEntityManager);
	for (TreeNodeBase* child : m_Children)
	{
		delete child;
		child = nullptr;
	}

}

void TreeNodeBase::Update(float dt, bool paused)
{
	PROFILE_FUNCTION(profiler::colors::Blue);
	m_DwellerCount = m_Dwellers.Size();
	//RenderBox();

	if (m_NodeEntityManager->EntityCount() <= 0)
		return;

	m_NodeEntityManager->Update(dt, m_Dwellers, paused);

	PROFILE_BLOCK("forEachDweller", profiler::colors::LightBlue);
	for (TreeDweller* dweller : m_Dwellers)
	{
		if (!dweller)
			continue;

		if (dweller->GetType() == TreeDweller::eType::STATIC)
			continue;

		bool found = false;
		const ComponentList& list = dweller->GetComponentPairList();

		for (const ComponentPair& pair : list)
		{
			if (!pair.m_Component)
				continue;
			if (pair.m_Type & TreeDweller::TRANSLATION)
			{
				if (m_Parent && !InsideNode(dweller))
				{
					RemoveEntity(dweller);
					m_Octree->MoveUp(this, dweller, m_Depth);
					found = true;
					break;
				}
			}
		}
		if (found)
			break;
	}

	PROFILE_BLOCK_END;


	if (!m_Dwellers.Empty() || HasEntities() || !m_Parent)
		return;

	for (s32 i = 0; i < 8; i++)
	{
		if (m_Parent->GetChildByIndex(i) == this)
		{
			m_Parent->AddChild(nullptr, i);
			m_Octree->ToDelete(this);
			return;
		}
	}
}

void TreeNodeBase::SetDepth(s32 depth)
{
	m_Depth = depth;
}

s32 TreeNodeBase::GetDepth() const
{
	return m_Depth;
}

void TreeNodeBase::AddChild(TreeNodeBase* child_node, s32 index)
{
	if (child_node)
	{
		child_node->AddParent(this);
		child_node->m_MemoryBlockIndex = m_MemoryBlockIndex;
	}
	m_Children[index] = child_node;
}

void TreeNodeBase::AddParent(TreeNodeBase* parent)
{
	m_Parent = parent;
}

TreeNodeBase* TreeNodeBase::GetParent()
{
	return m_Parent;
}

void TreeNodeBase::AddEntity(TreeDweller* dweller)
{
	m_Dwellers.Add(dweller);
	m_NodeEntityManager->AddEntity(dweller);
}

void TreeNodeBase::AddEntity(TreeDweller* dweller, s32 node)
{
	m_Children[node]->AddEntity(dweller);
}

void TreeNodeBase::RemoveEntity(TreeDweller* dweller)
{
	dweller->SetFirstNode(nullptr);

	m_NodeEntityManager->RemoveEntity(dweller);
	m_Dwellers.RemoveCyclic(dweller);
}

void TreeNodeBase::SetPosition(CU::Vector3f position)
{
	m_CenterPosition = position;
}

const CU::Vector3f& TreeNodeBase::GetPosition() const
{
	return m_CenterPosition;
}

TreeNodeBase* TreeNodeBase::GetChildByIndex(s32 index)
{
	return m_Children[index];
}

bool TreeNodeBase::HasEntities()
{
	for (s32 i = 0; i < 8; i++)
	{
		if (m_Children[i])
		{
			if (m_Children[i]->HasEntities())
				return true;
		}
	}

	if (!m_Dwellers.Empty())
		return true;

	return false;
}

float TreeNodeBase::GetHalfWidth() const
{
	return m_HalfWidth;
}

bool TreeNodeBase::InsideNode(TreeDweller* dweller)
{
	const CU::Vector3f position = m_CenterPosition;
	const float halfwidth = m_HalfWidth;

	const float bRight = position.x + halfwidth;
	const float bLeft = position.x - halfwidth;

	const float bTop = position.y + halfwidth;
	const float bBottom = position.y - halfwidth;

	const float bBack = position.z + halfwidth;
	const float bFront = position.z - halfwidth;

	const CU::Vector3f dweller_position = dweller->GetPosition();
	const CU::Vector3f dweller_bounds = dweller->GetWHD();


	const float cRight = dweller_position.x + dweller_bounds.x;
	const float cLeft = dweller_position.x - dweller_bounds.x;

	const float cTop = dweller_position.y + dweller_bounds.y;
	const float cBottom = dweller_position.y - dweller_bounds.y;

	const float cBack = dweller_position.z + dweller_bounds.z;
	const float cFront = dweller_position.z - dweller_bounds.z;

	bool left = cLeft >= bLeft;
	bool right = cRight <= bRight;
	bool bottom = cBottom >= bBottom;
	bool top = cTop <= bTop;
	bool front = cFront >= bFront;
	bool back = cBack <= bBack;
	return (left && right && bottom && top && front && back);
}

void TreeNodeBase::SetMemoryBlockIndex(s32 index)
{
	m_MemoryBlockIndex = index;
	m_NodeEntityManager->SetMemoryBlockIndex(m_MemoryBlockIndex);
	m_NodeEntityManager->Initiate();
}

#define RED CU::Vector4f(255.f,0.f,0.f,255.f)
#define GREEN CU::Vector4f(0.f,255.f,0.f,255.f)
#define BLUE CU::Vector4f(0.f,0.f,255.f,255.f)
#define YELLOW CU::Vector4f(255.f,255.f,0.f,255.f)
void TreeNodeBase::RenderBox()
{
	LinePoint points[8];


	switch (m_Depth)
	{
	case 0:
		points[0].color = RED;
		break;
	case 1:
		points[0].color = GREEN;
		break;
	case 2:
		points[0].color = BLUE;
		break;
	case 3:
		points[0].color = YELLOW;
		break;
	}

	points[1].color = points[0].color;
	points[2].color = points[0].color;
	points[3].color = points[0].color;
	points[4].color = points[0].color;
	points[5].color = points[0].color;
	points[6].color = points[0].color;
	points[7].color = points[0].color;

	points[0].position = CU::Vector4f(m_CenterPosition.x - m_HalfWidth, m_CenterPosition.y - m_HalfWidth, m_CenterPosition.z - m_HalfWidth, 1);

	points[1].position = points[0].position;
	points[1].position.y = m_CenterPosition.y + m_HalfWidth;


	points[2].position = CU::Vector4f(m_CenterPosition.x - m_HalfWidth, m_CenterPosition.y - m_HalfWidth, m_CenterPosition.z + m_HalfWidth, 1);

	points[3].position = points[2].position;
	points[3].position.y = m_CenterPosition.y + m_HalfWidth;


	points[4].position = CU::Vector4f(m_CenterPosition.x + m_HalfWidth, m_CenterPosition.y - m_HalfWidth, m_CenterPosition.z + m_HalfWidth, 1);

	points[5].position = points[4].position;
	points[5].position.y = m_CenterPosition.y + m_HalfWidth;


	points[6].position = CU::Vector4f(m_CenterPosition.x + m_HalfWidth, m_CenterPosition.y - m_HalfWidth, m_CenterPosition.z - m_HalfWidth, 1);

	points[7].position = points[6].position;
	points[7].position.y = m_CenterPosition.y + m_HalfWidth;

	m_Synchronizer->AddRenderCommand(LineCommand(points[0], points[1], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[0], points[2], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[0], points[6], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[1], points[3], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[1], points[7], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[3], points[5], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[5], points[7], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[6], points[4], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[2], points[4], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[4], points[5], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[2], points[3], true));
	m_Synchronizer->AddRenderCommand(LineCommand(points[6], points[7], true));
}

s32 TreeNodeBase::GetMemoryBlockIndex()
{
	return m_MemoryBlockIndex;
}

void TreeNodeBase::ToggleRenderBox(bool v)
{
	if (v == m_RenderBox)
		return;

	for (TreeNodeBase* c : m_Children)
	{
		if (c) c->ToggleRenderBox(v);
	}

	m_RenderBox = v;

}

void TreeNodeBase::RemoveAllDwellers()
{
	for (TreeNodeBase* c : m_Children)
	{
		if (c) c->RemoveAllDwellers();
	}
	m_Dwellers.RemoveAll();
}
