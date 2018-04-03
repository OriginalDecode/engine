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

#define MAX_DEPTH 2
constexpr float radius = 64.f;

static const char* s_TerrainLevels[] = {
	"1024",
	"512",
	"256",
	"128",
	"64",
	"32",
	"16",
	"8",
	"4",
	"2"
};

static std::vector<u64> s_HashTerrain;

TerrainSystem::TerrainSystem()
{
	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();
	int width = 512;
	for (int i = 0; i < MAX_DEPTH; i++)
	{
		Terrain* terrain = new Terrain(width);
		s_HashTerrain.push_back(Hash(s_TerrainLevels[i]));
		manager->AddTerrain(s_HashTerrain[i], terrain);
		width *= 0.5f;
	}

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

void TerrainSystem::DrawShadow()
{
	m_Tree.DrawShadow();
}

void test::Leaf::Render()
{
	Draw(false);
	//Draw([&](Terrain* terrain) { terrain ? terrain->Render(Engine::GetInstance()->GetRenderer()->GetRenderContext()) : 0; });
}

void test::Leaf::DrawShadow()
{
	Draw(true);
	//Draw([&](Terrain* terrain) { terrain ? terrain->ShadowRender(Engine::GetInstance()->GetRenderer()->GetRenderContext()) : 0; });

}

bool test::Leaf::Draw(bool shadow)
{
	PROFILE_FUNCTION(profiler::colors::Red);
	bool rendered = false;
	for (int i = 0; i < 4; i++)
	{
		if (m_Children[i])
		{
			rendered |= m_Children[i]->Draw(shadow);
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

		m_Terrain->SetPosition(CU::Vector2f(m_AABB.m_Pos.x, m_AABB.m_Pos.y));

		if (shadow)
			m_Terrain->ShadowRender(Engine::GetInstance()->GetRenderer()->GetRenderContext());
		else
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
	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();
	Terrain* terrain = manager->GetTerrain(s_HashTerrain[m_Depth]);

	PROFILE_BLOCK("create terrain", profiler::colors::Red700);
	AABB bb = m_AABB;
	float halfwidth = bb.m_Halfwidth;
	bb.m_Halfwidth *= 0.5f;

	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;

	m_Children[0]->m_AABB = bb;
	m_Children[0]->m_Terrain = terrain;

	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y + bb.m_Halfwidth;

	m_Children[1]->m_AABB = bb;
	m_Children[1]->m_Terrain = terrain;


	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x + bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;

	m_Children[2]->m_AABB = bb;
	m_Children[2]->m_Terrain = terrain;


	bb.m_Pos = m_AABB.m_Pos;
	bb.m_Pos.x = bb.m_Pos.x - bb.m_Halfwidth;
	bb.m_Pos.y = bb.m_Pos.y - bb.m_Halfwidth;

	m_Children[3]->m_AABB = bb;
	m_Children[3]->m_Terrain = terrain;


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

	return true;
}

void test::QuadTree::Draw()
{
	m_Root->Render();
}

void test::QuadTree::DrawShadow()
{
	m_Root->DrawShadow();
}

void test::QuadTree::Update(float x, float y)
{
	PROFILE_FUNCTION(profiler::colors::Red600);
	test::Position pos;
	pos.x = x;
	pos.y = y;

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
