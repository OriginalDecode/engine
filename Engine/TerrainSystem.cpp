#include "stdafx.h"
#include "TerrainSystem.h"


TerrainSystem::TerrainSystem()
{
	m_Tree = QuadTree(1024.f, { 1024.f,0.f,1024.f, 1.f });
}

void TerrainSystem::Update()
{
	m_Tree.Update();
}
