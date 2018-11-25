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
#include <Engine/Engine.h>
#include <Engine/IGraphicsContext.h>

#include "TGA32.h"
constexpr int MAX_DEPTH = 4;


static u64 s_RootTerrainHash = 0;
static std::vector<u64> s_HashTerrain;

TerrainSystem::TerrainSystem()
{
	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();
	float width = TERRAIN_HALFWIDTH;
	for (int i = 0; i < MAX_DEPTH; i++)
	{
		char temp[255];
		sprintf_s(temp, "%.0f", width);

		Terrain* terrain = new Terrain(width);
		s_HashTerrain.push_back(cl::Hash(temp));
		manager->AddTerrain(s_HashTerrain[i], terrain);
		width /= 2.f;
	}
	
	s_RootTerrainHash = cl::Hash("2048");
	manager->AddTerrain(s_RootTerrainHash, new Terrain(TERRAIN_HALFWIDTH*2));


	test::Position pos;
	pos.x = TERRAIN_HALFWIDTH;
	pos.y = TERRAIN_HALFWIDTH;
	m_Tree.Init(pos);


	TGA32::Image* image = TGA32::Load("Data/Textures/terrain/britannia.tga");
	m_Heightmap.myData = new u8[image->myWidth * image->myHeight];
	for (int i = 0; i < image->myWidth * image->myHeight; ++i)
	{
		m_Heightmap.myData[i] = image->myImage[i];
	}

	m_Heightmap.myDepth = image->myHeight;
	m_Heightmap.myWidth = image->myWidth;

	SAFE_DELETE(image);



}

void TerrainSystem::Update()
{
	CU::Vector4f pos = Engine::GetInstance()->GetCamera()->GetPos();
	m_Tree.Update(pos.x, pos.z);
}

void TerrainSystem::DrawShadow()
{
	m_Tree.DrawShadow();
}

void TerrainSystem::Draw()
{
	m_Tree.Draw();
}

float TerrainSystem::GetHeight(int x, int y)
{
	return m_Heightmap.myData[y * x];
}

void test::Leaf::Render()
{
	Draw(false);
}

void test::Leaf::DrawShadow()
{
	Draw(true);
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

	if (m_Terrain && !rendered)
	{
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

bool test::Leaf::isNeighbour(test::Leaf* /*leaf*/)
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

	if (!m_AABB.Intersect(pos, 2048.f))
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
	Effect* terrain_shader = m_Root->m_Terrain->GetEffect();
	if (!m_RenderDepth)
	{
		terrain_shader->Use();
	}
	else
	{
		auto& ctx = Engine::GetInstance()->GetRenderer()->GetRenderContext().GetContext();
		ctx.SetVertexShader(m_Shaders[0]);
		ctx.SetDomainShader(m_Shaders[1]);
		ctx.SetHullShader(m_Shaders[2]);
		ctx.SetPixelShader(m_Shaders[3]);
		ctx.DSSetShaderResource(TextureSlot::REGISTER_7, 1, Engine::GetInstance()->GetTexture(HEIGHTMAP)->GetShaderViewRef());
	}

	m_Root->Render();

	if(!m_RenderDepth)
	{
		terrain_shader->Clear();
	}
	m_RenderDepth = !m_RenderDepth;
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
	if (m_Root->m_AABB.Intersect(pos, 2048.f))
		m_Root->Insert(pos);

}

test::QuadTree::QuadTree()
{

	Engine* engine = Engine::GetInstance();

	const u64 vtx	=	engine->LoadShader("gpu_terrain.vs", "main");
	const u64 ds	=	engine->LoadShader("gpu_terrain.ds", "main");
	const u64 hs	=	engine->LoadShader("gpu_terrain.hs", "main");
	const u64 frag	=	engine->LoadShader("gpu_terrain.ps", "depth");

	
	m_Shaders[0] = engine->GetShader(vtx);
	m_Shaders[1] = engine->GetShader(ds);
	m_Shaders[2] = engine->GetShader(hs);
	m_Shaders[3] = engine->GetShader(frag);
}

void test::QuadTree::Init(Position xy)
{
	m_Root = new Leaf;
	m_Root->m_AABB.m_Pos = xy;
	m_Root->m_AABB.m_Halfwidth = TERRAIN_HALFWIDTH;
	TerrainManager* manager = Engine::GetInstance()->GetTerrainManager();
	Terrain* terrain = manager->GetTerrain(s_RootTerrainHash);
	m_Root->m_Terrain = terrain;
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
