#include "stdafx.h"
#include "TerrainSystem.h"
#include <Engine/Engine.h>
#include <Engine/RenderCommand.h>
#include <Engine/Synchronizer.h>
#include <Engine/Terrain.h>
#include <Engine/Renderer.h>
#include <Engine/RenderContext.h>

TerrainSystem::TerrainSystem()
{
	//m_Tree = QuadTree(1024.f, { 1024.f,0.f,1024.f, 1.f });


	test::Position pos;
	pos.x = 256;
	pos.y = 256;
	m_Tree.Init(pos);


}

void TerrainSystem::Update()
{
	CU::Vector4f pos = Engine::GetInstance()->GetCamera()->GetPos();
	m_Tree.Update(pos.x, pos.z);
	m_Tree.Draw();
}

bool test::Leaf::Render()
{
	bool rendered = false;
	for (int i = 0; i < 4; i++)
	{
		if (m_Children[i])
		{
			rendered |= m_Children[i]->Render();
		}
	}


	Synchronizer* sync = Engine::GetInstance()->GetSynchronizer();

	CU::Vector3f pos0 = { m_AABB.m_Pos.x, 0, m_AABB.m_Pos.y };
	pos0 += m_AABB.m_Halfwidth;

	CU::Vector3f pos1 = { m_AABB.m_Pos.x, 0, m_AABB.m_Pos.y };
	pos1 -= m_AABB.m_Halfwidth;

	LinePoint p0, p1, p2, p3;
	p0.position = CU::Vector4f(pos1.x, 0.f, pos1.z, 1);
	p1.position = CU::Vector4f(pos0.x, 0.f, pos0.z, 1);
	p2.position = CU::Vector4f(pos1.x, 0.f, pos0.z, 1);
	p3.position = CU::Vector4f(pos0.x, 0.f, pos1.z, 1);

 	
 	sync->AddRenderCommand(LineCommand(p0, p2, false));
 	sync->AddRenderCommand(LineCommand(p2, p1, false));
 	sync->AddRenderCommand(LineCommand(p1, p3, false));
 	sync->AddRenderCommand(LineCommand(p3, p0, false));

 	if (m_Terrain && !rendered)
 	{
 		m_Terrain->Render(Engine::GetInstance()->GetRenderer()->GetRenderContext());
 		m_Terrain->Wireframe(Engine::GetInstance()->GetRenderer()->GetRenderContext());
		return true;
 	}

	return rendered;
}

void test::Leaf::Reset()
{
	for (int i = 0; i < 4; i++)
	{
		if(m_Children[i])
			m_Children[i]->Reset();
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_Children[i])
		{
			delete m_Children[i];
			m_Children[i] = nullptr;
		}
	}

	delete m_Terrain;
	m_Terrain = nullptr;
}

static int previous_index = 0;

void test::Leaf::subdivide()
{
	m_Children[0] = new Leaf;
	m_Children[1] = new Leaf;
	m_Children[2] = new Leaf;
	m_Children[3] = new Leaf;



	m_Children[0]->m_Index = this->m_Index + 1;
	m_Children[1]->m_Index = this->m_Index + 1;
	m_Children[2]->m_Index = this->m_Index + 1;
	m_Children[3]->m_Index = this->m_Index + 1;

	m_Children[0]->m_Parent = this;
	m_Children[1]->m_Parent = this;
	m_Children[2]->m_Parent = this;
	m_Children[3]->m_Parent = this;


	CU::Vector3f color;

	switch(m_Index + 1)
	{
	case 1:
		color = CU::Vector3f(1, 0, 0);
		break;
	case 2: 
		color = CU::Vector3f(1, 1, 0);
		break;
	case 3:
		color = CU::Vector3f(0, 1, 0);
		break;
	case 4:
		color = CU::Vector3f(0, 0, 1);
		break;
	}



	AABB bb = m_AABB;
	float halfwidth = bb.m_Halfwidth;
	bb.m_Halfwidth *= 0.5f;

	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;
	m_Children[0]->m_AABB = bb;
	m_Children[0]->m_Terrain = new Terrain(halfwidth, color);
	m_Children[0]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));

	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;
	m_Children[1]->m_AABB = bb;
	m_Children[1]->m_Terrain = new Terrain(halfwidth, color);
	m_Children[1]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));

	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;
	m_Children[2]->m_AABB = bb;
	m_Children[2]->m_Terrain = new Terrain(halfwidth, color);
	m_Children[2]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));

	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;
	m_Children[3]->m_AABB = bb;
	m_Children[3]->m_Terrain = new Terrain(halfwidth, color);
	m_Children[3]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));


}

test::Leaf::Leaf()
{
	m_Children[0] = nullptr;
	m_Children[1] = nullptr;
	m_Children[2] = nullptr;
	m_Children[3] = nullptr;
}

test::Leaf::~Leaf()
{
	for (int i = 0; i < 4; i++)
		delete m_Children[i];
}

bool test::Leaf::Insert(Position pos)
{
	if (!this)
		return false;

	if (!m_AABB.Intersect(pos))
		return false;

	if (m_Index >= 4)
		return false;

	if (m_Children[0] == nullptr )
		subdivide();

	for (int i = 0; i < 4; i++)
	{
		m_Children[i]->Insert(pos);
	}
	return false;
}

void test::QuadTree::Draw()
{
	m_Root->Render();
}

void test::QuadTree::Update(float x, float y)
{
	test::Position pos;
	pos.x = x;
	pos.y = y;

	m_Root->Reset();
	if (m_Root->m_AABB.Intersect(pos))
		m_Root->Insert(pos);


}

void test::QuadTree::Init(Position xy)
{
	m_Root = new Leaf;
	m_Root->m_AABB.m_Pos = xy;
	m_Root->m_AABB.m_Halfwidth = xy.x / 2;
}

void test::QuadTree::Insert(Position xy)
{
	m_Root->Insert(xy);
}

bool test::AABB::Intersect(Position position)
{
	if (position.x > m_Pos.x + m_Halfwidth)
		return false;

	if (position.x < m_Pos.x - m_Halfwidth)
		return false;

	if (position.y > m_Pos.y + m_Halfwidth)
		return false;

	if (position.y < m_Pos.y - m_Halfwidth)
		return false;

	return true;
}
