#include "stdafx.h"
#include "TreeNode.h"
#include "Synchronizer.h"
#include "RenderCommand.h"
#include "Octree.h"
TreeNode::~TreeNode()
{

	for (s32 i = 0; i < 8; i++)
	{
		delete m_Children[i];
		m_Children[i] = nullptr;
	}
}

void TreeNode::Initiate(float halfwidth, Octree* octree)
{
	m_HalfWidth = halfwidth;
	m_Synchronizer = Hex::Engine::GetInstance()->GetSynchronizer();
	m_NodeEntityManager.Initiate();
	m_Octree = octree;
	for (s32 i = 0; i < 8; i++)
	{
		m_Children[i] = nullptr;
	}
	//if (!m_Parent)
		//m_DebugName == "Octree : Root Node";
}

void TreeNode::AddChild(TreeNode* child_node, s32 index)
{
	if(child_node)
		child_node->AddParent(this);

	m_Children[index] = child_node;

	/*std::stringstream ss;
	ss << "parent : " << m_DebugName << " | child : ";

	switch (index)
	{
			case 0:
			ss << "bottom_left";
			break;
			case 1:
			ss << "bottom_right";
			break;
			case 2:
			ss << "top_left";
			break;
			case 3:
			ss << "top_right";
			break;
			case 4:
			ss << "bottom_far_left";
			break;
			case 5:
			ss << "bottom_far_right";
			break;
			case 6:
			ss << "top_far_left";
			break;
			case 7:
			ss << "top_far_right";
			break;
	}
	child_node->SetDebugName(ss.str());*/
}

void TreeNode::AddParent(TreeNode* parent_node)
{
	m_Parent = parent_node;
}

void TreeNode::AddEntity(TreeDweller* dweller)
{
	m_Entities.Add(dweller->GetEntity());
	m_Dwellers.Add(dweller);
	m_NodeEntityManager.AddEntity(dweller);
}

void TreeNode::AddEntity(TreeDweller* dweller, s32 node)
{
	m_Children[node]->AddEntity(dweller);
}

void TreeNode::RemoveEntity(TreeDweller* dweller)
{
	m_NodeEntityManager.RemoveEntity(dweller);
	m_Dwellers.RemoveCyclic(dweller);
}

void TreeNode::SetPosition(CU::Vector3f position)
{
	m_CenterPosition = position;
}

#include <ComponentFilter.h>
#include <TranslationComponent.h>
#include "Engine.h"
#include <EntityManager.h>
void TreeNode::Update(float dt)
{
	if (m_Paused)
		return;
	RenderBox();

	Hex::Engine::GetInstance()->GetEntityManager().SetActiveNodeManager(&m_NodeEntityManager);
	m_NodeEntityManager.Update(dt);

	const EntityList& entities = m_NodeEntityManager.GetEntities(CreateFilter<Requires<TranslationComponent>>());
	TreeDweller* to_remove = nullptr;
	for (TreeDweller* dweller : m_Dwellers)
	{
		bool found = false;
		const ComponentList& list = dweller->GetComponentPairList();
		for (const ComponentPair& pair : list)
		{
			if (pair.m_Type & TreeDweller::TRANSLATION)
			{
				if (!InsideNode(static_cast<TranslationComponent*>(pair.m_Component)->myOrientation.GetPosition()))
				{
					if (m_Parent)
					{
						m_Octree->MoveUp(m_Parent, dweller, m_Depth - 1);
						RemoveEntity(dweller);
						found = true;
						break;
					}
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
		if (!node) continue;
		node->Update(dt);
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
	if (m_Dwellers.Size() > 0)
	{
		return true;
	}

	for (s32 i = 0; i < 8; i++)
	{
		if (m_Children[i])
		{
			if (m_Children[i]->SubNodeContainsDwellers())
				return true;
		}
	}
	return false;
}

void TreeNode::SetDebugName(std::string name)
{
	m_DebugName = name;
}

#define RED CU::Vector4f(255.f,0.f,0.f,255.f)
#define GREEN CU::Vector4f(0.f,255.f,0.f,255.f)
#define BLUE CU::Vector4f(0.f,0.f,255.f,255.f)
#define YELLOW CU::Vector4f(255.f,255.f,0.f,255.f)
void TreeNode::RenderBox()
{
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

bool TreeNode::InsideNode(CU::Vector3f pos)
{
	const CU::Vector3f position = m_CenterPosition;
	const float halfwidth = m_HalfWidth;

	const float bLeft = position.x - halfwidth;
	const float bRight = position.x + halfwidth;

	const float bTop = position.y + halfwidth;
	const float bBottom = position.y - halfwidth;

	const float bBack = position.z + halfwidth;
	const float bFront = position.z - halfwidth;

	const CU::Vector3f dweller_position = pos;

	const float cLeft = position.x - 4.f;
	const float cRight = position.x + 4.f;

	const float cTop = position.y + 4.f;
	const float cBottom = position.y - 4.f;

	const float cBack = position.z + 4.f;
	const float cFront = position.z - 4.f;

	return (cLeft < bLeft && cRight > bRight && cTop < bTop && cBottom > bBottom && cBottom < bBack && cFront > bFront);
}
