#include "stdafx.h"
#include "QuadTree.h"

#include <Engine/Camera.h>
#include <Engine/Engine.h>

#include "QuadTreeNode.h"

QuadTree::QuadTree(float halfwidth, CU::Vector4f pos)
{
	m_Root = new QuadTreeNode(halfwidth, pos);

}


QuadTree::~QuadTree()
{
	delete m_Root;
	m_Root = nullptr;

}

void QuadTree::Update()
{
	CU::Matrix44f camera_pos = Engine::GetInstance()->GetCamera()->GetOrientation();
	m_Root->Update(camera_pos);
}
