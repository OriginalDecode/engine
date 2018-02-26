#include "stdafx.h"
#include "TerrainSystem.h"
#include <Engine/Engine.h>
#include <Engine/RenderCommand.h>
#include <Engine/Synchronizer.h>
#include <Engine/Terrain.h>
#include <Engine/Renderer.h>
#include <Engine/RenderContext.h>

#include <engine/profile_defines.h>
#include <Engine/TerrainManager.h>

#define MAX_DEPTH 6

TerrainSystem::TerrainSystem()
{
	//m_Tree = QuadTree(1024.f, { 1024.f,0.f,1024.f, 1.f });


	test::Position pos;
	pos.x = 512;
	pos.y = 512;
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
	PROFILE_FUNCTION(profiler::colors::Red);
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
	p0.position = CU::Vector4f(pos1.x, 0.0f, pos1.z, 1);
	p1.position = CU::Vector4f(pos0.x, 0.0f, pos0.z, 1);
	p2.position = CU::Vector4f(pos1.x, 0.0f, pos0.z, 1);
	p3.position = CU::Vector4f(pos0.x, 0.0f, pos1.z, 1);
	
 

 	if (m_Terrain && !rendered)
 	{

		sync->AddRenderCommand(LineCommand(p0, p2, false));
		sync->AddRenderCommand(LineCommand(p2, p1, false));
		sync->AddRenderCommand(LineCommand(p1, p3, false));
		sync->AddRenderCommand(LineCommand(p3, p0, false));

 		m_Terrain->Render(Engine::GetInstance()->GetRenderer()->GetRenderContext());
		return true;
 	}

	return rendered;
}

void test::Leaf::Reset()
{
	for (int i = 0; i < 4; i++)
	{
		if(m_Children[i])
		{
			m_Children[i]->Reset();
			delete m_Children[i];
			m_Children[i] = nullptr;
		}
	}
}

bool test::Leaf::isNeighbour(test::Leaf* leaf)
{
	return true;
}

static int terrain_id = 0;

void test::Leaf::subdivide()
{
	PROFILE_FUNCTION(profiler::colors::Red500);

	for (int i = 0; i < 4; i++)
	{
		m_Children[i] = new Leaf;
		m_Children[i]->m_Depth = this->m_Depth + 1;
		m_Children[i]->m_Parent = this;
		m_Children[i]->m_Index = i;
	}

	CU::Vector3f color;
	std::string dir;



	CU::Vector2f uv[4];

	PROFILE_BLOCK("create terrain", profiler::colors::Red700);
	AABB bb = m_AABB;
	float halfwidth = bb.m_Halfwidth;
	bb.m_Halfwidth *= 0.5f;

	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;
	m_Children[0]->m_AABB = bb;

	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();
	
	PROFILE_BLOCK("create 1 terrain", profiler::colors::RedA100);
	char temp[100];
	sprintf_s(temp, 100, "%d", ++terrain_id);
	u64 hash0 = Hash(temp);
	Terrain* terrain = manager->GetTerrain(hash0);
	if (!terrain)
	{
		terrain = new Terrain(halfwidth, uv, color);
		manager->AddTerrain(hash0, terrain);

	}
	m_Children[0]->m_Terrain = terrain;
	m_Children[0]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));
	PROFILE_BLOCK_END;

	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;
	m_Children[1]->m_AABB = bb;

	sprintf_s(temp, 100, "%d", ++terrain_id);
	hash0 = Hash(temp);
	terrain = manager->GetTerrain(hash0);
	if (!terrain)
	{
		terrain = new Terrain(halfwidth, uv, color);
		manager->AddTerrain(hash0, terrain);
	}
	m_Children[1]->m_Terrain = terrain;
	m_Children[1]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));


	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;
	m_Children[2]->m_AABB = bb;

	sprintf_s(temp, 100, "%d", ++terrain_id);
	hash0 = Hash(temp);
	terrain = manager->GetTerrain(hash0);
	if (!terrain)
	{
		terrain = new Terrain(halfwidth, uv, color);
		manager->AddTerrain(hash0, terrain);
	}
	m_Children[2]->m_Terrain = terrain;
	m_Children[2]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));


	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;
	m_Children[3]->m_AABB = bb;

	sprintf_s(temp, 100, "%d", ++terrain_id);
	hash0 = Hash(temp);
	terrain = manager->GetTerrain(hash0);
	if (!terrain)
	{
		terrain = new Terrain(halfwidth, uv, color);
		manager->AddTerrain(hash0, terrain);
	}
	m_Children[3]->m_Terrain = terrain;
	m_Children[3]->m_Terrain->SetPosition(CU::Vector2f(bb.m_Pos.x, bb.m_Pos.y));


	PROFILE_BLOCK_END;
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

constexpr float radius = 32.f;
bool test::Leaf::Insert(Position pos)
{
	PROFILE_FUNCTION(profiler::colors::Red800);

	if (!this)
		return false;

	if (!m_AABB.Intersect(pos, radius))
		return false;

	if (m_Depth >= MAX_DEPTH)
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
	PROFILE_FUNCTION(profiler::colors::Red600);
	test::Position pos;
	pos.x = x;
	pos.y = y;

	terrain_id = 0;
	m_Root->Reset();
	if (m_Root->m_AABB.Intersect(pos, radius))
		m_Root->Insert(pos);


}

void test::QuadTree::Init(Position xy)
{
	m_Root = new Leaf;
	m_Root->m_AABB.m_Pos = xy;
	m_Root->m_AABB.m_Halfwidth = 1024 / 2;
}

void test::QuadTree::Insert(Position xy)
{
	PROFILE_FUNCTION(profiler::colors::Red400);
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

bool test::AABB::Intersect(Position position, float radius)
{
	if (position.x - radius > m_Pos.x + m_Halfwidth)
		return false;

	if (position.x + radius < m_Pos.x - m_Halfwidth)
		return false;

	if (position.y - radius > m_Pos.y + m_Halfwidth)
		return false;

	if (position.y + radius< m_Pos.y - m_Halfwidth)
		return false;

	return true;
}
