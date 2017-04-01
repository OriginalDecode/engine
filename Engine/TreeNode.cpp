#include "stdafx.h"
#include "TreeNode.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Octree.h"
#include <ComponentFilter.h>
#include <TranslationComponent.h>
#include "Engine.h"
#include <EntityManager.h>

TreeNode::~TreeNode()
{

	for (s32 i = 0; i < 8; i++)
	{
		delete m_Children[i];
		m_Children[i] = nullptr;
	}
	Engine::GetInstance()->GetEntityManager().UnRegisterManager(&m_NodeEntityManager);

}

void TreeNode::Initiate(float halfwidth, Octree* octree)
{
	m_HalfWidth = halfwidth;
	m_Synchronizer = Engine::GetInstance()->GetSynchronizer();

	m_NodeEntityManager.Initiate();
	Engine::GetInstance()->GetEntityManager().RegisterManager(&m_NodeEntityManager);

	m_Octree = octree;
	for (s32 i = 0; i < 8; i++)
	{
		m_Children[i] = nullptr;
	}
}

void TreeNode::AddChild(TreeNode* child_node, s32 index)
{
	if (child_node)
		child_node->AddParent(this);

	m_Children[index] = child_node;

}

void TreeNode::AddParent(TreeNode* parent_node)
{
	m_Parent = parent_node;
}

void TreeNode::AddEntity(TreeDweller* dweller)
{
	m_Dwellers.Add(dweller);
	m_NodeEntityManager.AddEntity(dweller);
}

void TreeNode::AddEntity(TreeDweller* dweller, s32 node)
{
	m_Children[node]->AddEntity(dweller);
}

void TreeNode::RemoveEntity(TreeDweller* dweller)
{
	dweller->SetFirstNode(nullptr);

	m_NodeEntityManager.RemoveEntity(dweller);
	m_Dwellers.RemoveCyclic(dweller);
}

void TreeNode::SetPosition(CU::Vector3f position)
{
	m_CenterPosition = position;
}

void TreeNode::Update(float dt)
{
	if (m_Paused)
		return;

	RenderBox();

	Engine::GetInstance()->GetEntityManager().SetActiveNodeManager(&m_NodeEntityManager);
	m_NodeEntityManager.Update(dt);

	//const EntityList& entities = m_NodeEntityManager.GetEntities(CreateFilter<Requires<TranslationComponent>>());

	for (TreeDweller* dweller : m_Dwellers)
	{
		if ( dweller->GetType() == TreeDweller::eType::STATIC )
			continue;

		bool found = false;
		const ComponentList& list = dweller->GetComponentPairList();
		for (const ComponentPair& pair : list)
		{
			if (pair.m_Type & TreeDweller::TRANSLATION)
			{
				if (m_Parent && !InsideNode(dweller))
				{
					m_Octree->MoveUp(this, dweller, m_Depth);
					found = true;
					break;
				}
			}
		}
		if (found)
			break;
	}

	if (m_Dwellers.Size() <= 0)
	{
		if (!SubNodeContainsDwellers())
		{
			if (m_Parent)
			{
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
		}
	}

	for (TreeNode* node : m_Children)
	{
		if (!node)
			continue;

		/*
		if (m_Parent && !m_Parent->GetParent())
		{
			Engine::GetInstance()->GetThreadpool().AddWork(Work([&]() {
				node->Update(dt); }));
		}
		else/**/
		{
			node->Update(dt);
		}
	}
}




TreeNode* TreeNode::GetChildByIndex(s32 index)
{
	return m_Children[index];
}

s32 TreeNode::GetEntityCount()
{
	return m_Dwellers.Size();
}

bool TreeNode::SubNodeContainsDwellers()
{
	for (s32 i = 0; i < 8; i++)
	{
		if (m_Children[i])
		{
			if (m_Children[i]->SubNodeContainsDwellers())
				return true;
		}
	}

	if (m_Dwellers.Size() > 0)
	{
		return true;
	}

	return false;
}

#define RED CU::Vector4f(255.f,0.f,0.f,255.f)
#define GREEN CU::Vector4f(0.f,255.f,0.f,255.f)
#define BLUE CU::Vector4f(0.f,0.f,255.f,255.f)
#define YELLOW CU::Vector4f(255.f,255.f,0.f,255.f)
void TreeNode::RenderBox()
{
	if (!m_RenderBox)
		return;

	SLinePoint points[8];


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


	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[0], points[1]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[0], points[2]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[0], points[6]));

	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[1], points[3]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[1], points[7]));

	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[3], points[5]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[5], points[7]));


	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[6], points[4]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[2], points[4]));

	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[4], points[5]));
	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[2], points[3]));

	m_Synchronizer->AddRenderCommand(RenderCommand(eType::LINE_Z_ENABLE, points[6], points[7]));

}

bool TreeNode::InsideNode(TreeDweller* dweller)
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
